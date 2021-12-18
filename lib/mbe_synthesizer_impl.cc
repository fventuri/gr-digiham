/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "mbe_synthesizer_impl.h"
#include <gnuradio/io_signature.h>

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
    module = new Digiham::Mbe::MbeSynthesizer();
    set_mode(type);
    reader = new Reader<unsigned char>();
    module->setReader(reader);
    writer = new Writer<short>();
    module->setWriter(writer);

    // output is always 8ks/s where each frame is always 20ms
    // 8000 * 0.020 = 160
    set_output_multiple(160);
}

mbe_synthesizer_impl::mbe_synthesizer_impl(unsigned int index, unsigned int bitrate)
    : gr::block("mbe_synthesizer",
                gr::io_signature::make(1, 1, sizeof(unsigned char)),
                gr::io_signature::make(1, 1, sizeof(short)))
{
    module = new Digiham::Mbe::MbeSynthesizer();
    set_mode(index, bitrate);
    reader = new Reader<unsigned char>();
    module->setReader(reader);
    writer = new Writer<short>();
    module->setWriter(writer);

    // output is always 8ks/s where each frame is always 20ms
    // 8000 * 0.020 = 160
    set_output_multiple(160);
}

mbe_synthesizer_impl::mbe_synthesizer_impl(short* cwds)
    : gr::block("mbe_synthesizer",
                gr::io_signature::make(1, 1, sizeof(unsigned char)),
                gr::io_signature::make(1, 1, sizeof(short)))
{
    module = new Digiham::Mbe::MbeSynthesizer();
    set_mode(cwds);
    reader = new Reader<unsigned char>();
    module->setReader(reader);
    writer = new Writer<short>();
    module->setWriter(writer);

    // output is always 8ks/s where each frame is always 20ms
    // 8000 * 0.020 = 160
    set_output_multiple(160);
}

mbe_synthesizer_impl::~mbe_synthesizer_impl()
{
    delete module;
    module = nullptr;
    delete reader;
    reader = nullptr;
    delete writer;
    writer = nullptr;
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
            bytes_per_frame = 7;
    }
}

void mbe_synthesizer_impl::set_mode(unsigned int index, unsigned int bitrate)
{
    module->setMode(new Digiham::Mbe::TableMode(index));
    this->bitrate = bitrate;
    bytes_per_frame = ceil(bitrate / 50 / 8);
}

void mbe_synthesizer_impl::set_mode(short* cwds)
{
    module->setMode(new Digiham::Mbe::ControlWordMode(cwds));
    short frame_bits = cwds[5] & 0xff;
    // each frame is 20ms -> 50 frames/s
    bitrate = 50 * frame_bits;
    bytes_per_frame = ceil(frame_bits / 8);
}

void mbe_synthesizer_impl::forecast(int noutput_items,
                                    gr_vector_int& ninput_items_required)
{
    // input: bytes_per_frame * 50
    // output: 8000 samples/s = 160 * frames/s
    ninput_items_required[0] = noutput_items / 160 * bytes_per_frame;
}

int mbe_synthesizer_impl::general_work(int noutput_items,
                                       gr_vector_int& ninput_items,
                                       gr_vector_const_void_star& input_items,
                                       gr_vector_void_star& output_items)
{
    auto in = static_cast<const unsigned char*>(input_items[0]);
    auto out = static_cast<short*>(output_items[0]);

    reader->set(ninput_items[0], in);
    writer->set(noutput_items, out);
    while (module->canProcess())
        module->process();
    consume(0, ninput_items[0] - reader->available());
    return noutput_items - writer->writeable();
}

} /* namespace digiham */
} /* namespace gr */
