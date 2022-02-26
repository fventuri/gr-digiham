/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "mbe_synthesizer_codecserver_impl.h"
#include <csdr/ringbuffer.hpp>
#include <gnuradio/io_signature.h>
#include <time.h>

constexpr size_t T_BUFSIZE = 32768;
//constexpr size_t T_BUFSIZE = 65536;
// output is always 8ks/s where each frame is always 20ms (i.e. 50fps)
// 8000 * 0.020 = 160
constexpr int AUDIO_SAMPLES_PER_FRAME = 160;
constexpr struct timespec MBE_WAIT_TIME_PER_FRAME = { 0, 2000000 };  // 2ms

namespace gr {
namespace digiham {

mbe_synthesizer::sptr mbe_synthesizer::make(DVMbeType type)
{
    return gnuradio::make_block_sptr<mbe_synthesizer_impl>(type);
}

mbe_synthesizer::sptr mbe_synthesizer::make(unsigned int index, unsigned int bitrate)
{
    return gnuradio::make_block_sptr<mbe_synthesizer_impl>(index, bitrate);
}

mbe_synthesizer::sptr mbe_synthesizer::make(short* cwds)
{
    return gnuradio::make_block_sptr<mbe_synthesizer_impl>(cwds);
}

mbe_synthesizer_impl::mbe_synthesizer_impl(DVMbeType type)
    : gr::block("mbe_synthesizer",
                gr::io_signature::make(1, 1, sizeof(unsigned char)),
                gr::io_signature::make(1, 1, sizeof(short)))
{
    init();
    set_mode(type);
}

mbe_synthesizer_impl::mbe_synthesizer_impl(unsigned int index, unsigned int bitrate)
    : gr::block("mbe_synthesizer",
                gr::io_signature::make(1, 1, sizeof(unsigned char)),
                gr::io_signature::make(1, 1, sizeof(short)))
{
    init();
    set_mode(index, bitrate);
}

mbe_synthesizer_impl::mbe_synthesizer_impl(short* cwds)
    : gr::block("mbe_synthesizer",
                gr::io_signature::make(1, 1, sizeof(unsigned char)),
                gr::io_signature::make(1, 1, sizeof(short)))
{
    init();
    set_mode(cwds);
}

mbe_synthesizer_impl::~mbe_synthesizer_impl()
{
    delete module;
    module = nullptr;
    delete inreader;
    inreader = nullptr;
    delete outreader;
    outreader = nullptr;
    delete outwriter;
    outwriter = nullptr;
}

void mbe_synthesizer_impl::set_mode(DVMbeType type)
{
    auto dmr_mode = new Digiham::Mbe::TableMode(33);

    short dstar_control_words[] = { 0x3001, 0x6307, 0x0040, 0x0000, 0x0000, 0x4800 };
    auto dstar_mode = new Digiham::Mbe::ControlWordMode(dstar_control_words);

    auto ysf_mode = new Digiham::Mbe::DynamicMode([](auto code) -> Digiham::Mbe::Mode* {
        unsigned short ysf_vw_control_words[] = { 0x5805, 0x6b08, 0x3010, 0x0000, 0x0000, 0x9001 };
        switch (code) {
            case 0: return new Digiham::Mbe::TableMode(33);
            case 2: return new Digiham::Mbe::TableMode(34);
            case 3:
                return new Digiham::Mbe::ControlWordMode(reinterpret_cast<short*>(ysf_vw_control_words));
            default: return nullptr;
        }
    });

    switch (type) {
        case DMR:
            module->setMode(dmr_mode);
            bitrate = 3600;
            bytes_per_frame = 9;
            break;
        case DStar:
            module->setMode(dstar_mode);
            bitrate = 3600;
            bytes_per_frame = 9;
            break;
        case YSF:
            module->setMode(ysf_mode);
            // YSF - DN
            bitrate = 2450;
            // add 1 byte for the dynamic code
            bytes_per_frame = 1 + 7;
            break;
    }
}

void mbe_synthesizer_impl::set_mode(unsigned int index, unsigned int bitrate)
{
    module->setMode(new Digiham::Mbe::TableMode(index));
    this->bitrate = bitrate;
    bytes_per_frame = ceil(static_cast<double>(bitrate) / 50 / 8);
}

void mbe_synthesizer_impl::set_mode(short* cwds)
{
    module->setMode(new Digiham::Mbe::ControlWordMode(cwds));
    short frame_bits = cwds[5] & 0xff;
    // each frame is 20ms -> 50 frames/s
    bitrate = 50 * frame_bits;
    bytes_per_frame = ceil(static_cast<double>(frame_bits) / 8);
}

void mbe_synthesizer_impl::forecast(int noutput_items,
                                    gr_vector_int& ninput_items_required)
{
    ninput_items_required[0] = noutput_items / AUDIO_SAMPLES_PER_FRAME * bytes_per_frame;
}

int mbe_synthesizer_impl::general_work(int noutput_items,
                                       gr_vector_int& ninput_items,
                                       gr_vector_const_void_star& input_items,
                                       gr_vector_void_star& output_items)
{
    auto in = static_cast<const unsigned char*>(input_items[0]);
    auto out = static_cast<short*>(output_items[0]);

    int frames_out = std::min(noutput_items, (int) outwriter->writeable()) / AUDIO_SAMPLES_PER_FRAME;
    int frames = std::min((int) (ninput_items[0] / bytes_per_frame), frames_out);

    int nin_items = frames * bytes_per_frame;
    inreader->set(nin_items, in);
    while (module->canProcess())
        module->process();
    consume(0, nin_items - inreader->available());

    // wait for MBE to produce items back
    for (int i = 0; i < frames; i++) {
        clock_nanosleep(CLOCK_REALTIME, 0, &MBE_WAIT_TIME_PER_FRAME, nullptr);
    }

    int nout_items = std::min(noutput_items, (int) outreader->available());
    memcpy(out, outreader->getReadPointer(), nout_items * sizeof(short));
    outreader->advance(nout_items);
    return nout_items;
}


/* internal functions */
void mbe_synthesizer_impl::init()
{
    module = new Digiham::Mbe::MbeSynthesizer();
    inreader = new Reader<unsigned char>();
    module->setReader(inreader);
    auto buffer = new Csdr::Ringbuffer<short>(T_BUFSIZE);
    outwriter = buffer;
    module->setWriter(outwriter);
    outreader = new Csdr::RingbufferReader<short>(buffer);

    set_output_multiple(AUDIO_SAMPLES_PER_FRAME);
}

} /* namespace digiham */
} /* namespace gr */
