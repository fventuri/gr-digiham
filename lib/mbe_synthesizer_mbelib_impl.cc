/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "mbe_synthesizer_mbelib_impl.h"
#include <gnuradio/io_signature.h>


namespace gr {
namespace digiham {

mbe_synthesizer::sptr mbe_synthesizer::make(DVMbeType type,
                                            unsigned int unvoiced_quality)
{
    return gnuradio::make_block_sptr<mbe_synthesizer_impl>(type, unvoiced_quality);
}

mbe_synthesizer::sptr mbe_synthesizer::make(unsigned int index, unsigned int bitrate,
                                            unsigned int unvoiced_quality)
{
    return gnuradio::make_block_sptr<mbe_synthesizer_impl>(index, bitrate, unvoiced_quality);
}

mbe_synthesizer::sptr mbe_synthesizer::make(unsigned short* cwds,
                                            unsigned int unvoiced_quality)
{
    return gnuradio::make_block_sptr<mbe_synthesizer_impl>(cwds, unvoiced_quality);
}

mbe_synthesizer_impl::mbe_synthesizer_impl(DVMbeType type,
                                           unsigned int unvoiced_quality)
    : gr::block("mbe_synthesizer",
                gr::io_signature::make(1, 1, sizeof(unsigned char)),
                gr::io_signature::make(1, 1, sizeof(short)))
{
    init();
    set_codec(type);
    set_unvoiced_quality(unvoiced_quality);
}

mbe_synthesizer_impl::mbe_synthesizer_impl(unsigned int index, unsigned int bitrate,
                                           unsigned int unvoiced_quality)
    : gr::block("mbe_synthesizer",
                gr::io_signature::make(1, 1, sizeof(unsigned char)),
                gr::io_signature::make(1, 1, sizeof(short)))
{
    init();
    set_codec(index, bitrate, 0);
    set_unvoiced_quality(unvoiced_quality);
}

mbe_synthesizer_impl::mbe_synthesizer_impl(unsigned short* cwds,
                                           unsigned int unvoiced_quality)
    : gr::block("mbe_synthesizer",
                gr::io_signature::make(1, 1, sizeof(unsigned char)),
                gr::io_signature::make(1, 1, sizeof(short)))
{
    init();
    set_codec(cwds, 0);
    set_unvoiced_quality(unvoiced_quality);
}

mbe_synthesizer_impl::~mbe_synthesizer_impl()
{
    // free everything
    free(curr_mp);
    curr_mp = nullptr;
    free(prev_mp);
    prev_mp = nullptr;
    free(prev_mp_enhanced);
    prev_mp_enhanced = nullptr;
}

void mbe_synthesizer_impl::set_codec(DVMbeType type)
{
    unsigned int dmr_table_mode = 33;
    unsigned int dmr_bitrate = 3600;
    unsigned int dmr_extra_bytes = 0;

    unsigned short dstar_control_words[] = { 0x3001, 0x6307, 0x0040, 0x0000, 0x0000, 0x4800 };
    unsigned int dstar_extra_bytes = 0;

    switch (type) {
        case DMR:
            set_codec(dmr_table_mode, dmr_bitrate, dmr_extra_bytes);
            break;
        case DStar:
            set_codec(dstar_control_words, dstar_extra_bytes);
            break;
        case YSF:
            set_ysf_mode(2);
            break;
        default:
            throw std::runtime_error("invalid DV MBE type");
    }

    this->type = type;
}

void mbe_synthesizer_impl::set_codec(unsigned int index, unsigned int bitrate,
                                     unsigned int extra_bytes)
{
    switch (index) {
        case 33:
            frameBits = 72;
            dataBits = 49;
            mbeCodec = Ambe3600x2450;
            break;
        case 34:
            frameBits = 49;
            dataBits = 49;
            mbeCodec = Ambe2450;
            break;
        case 59:
            frameBits = 144;
            dataBits = 88;
            mbeCodec = Imbe7200x4400;
            break;
        default:
            throw std::runtime_error("invalid DV MBE index");
    }
    frameSize = (frameBits + 7) / 8;
    audioSize = AudioSize;
    this->bitrate = bitrate;
    bytes_per_frame = extra_bytes + ceil(static_cast<double>(bitrate) / 50 / 8);
    this->extra_bytes = extra_bytes;
}

void mbe_synthesizer_impl::set_codec(unsigned short* cwds, unsigned int extra_bytes)
{
    frameBits = (cwds[5] >> 8) & 0xff;
    dataBits = (cwds[0] >> 8) & 0xff;
    if (frameBits == 72 && dataBits == 48) {
        mbeCodec = Ambe3600x2400;
    } else if (frameBits == 144 && dataBits == 88) {
        mbeCodec = Imbe7200x4400;
    } else {
        throw std::runtime_error("invalid DV MBE control words");
    }
    frameSize = (frameBits + 7) / 8;
    audioSize = AudioSize;
    // each frame is 20ms -> 50 frames/s
    bitrate = 50 * frameBits;
    bytes_per_frame = extra_bytes + ceil(static_cast<double>(frameBits) / 8);
    this->extra_bytes = extra_bytes;
}

void mbe_synthesizer_impl::set_ysf_mode(unsigned char mode)
{
    unsigned int ysf_v1_table_mode = 33;
    unsigned int ysf_v1_bitrate = 3600;
    unsigned int ysf_dn_table_mode = 34;
    unsigned int ysf_dn_bitrate = 2450;
    unsigned short ysf_vw_control_words[] = { 0x5805, 0x6b08, 0x3010, 0x0000, 0x0000, 0x9001 };
    unsigned int ysf_extra_bytes = 1;   // 1 extra byte for YSF mode

    switch (mode) {
        case 0:
            set_codec(ysf_v1_table_mode, ysf_v1_bitrate, ysf_extra_bytes);
            break;
        case 2:
            set_codec(ysf_dn_table_mode, ysf_dn_bitrate, ysf_extra_bytes);
            break;
        case 3:
            set_codec(ysf_vw_control_words, ysf_extra_bytes);
            break;
        default:
            throw std::runtime_error("invalid YSF mode");
    }

    this->type = YSF;
    this->ysf_mode = mode;
}

void mbe_synthesizer_impl::set_unvoiced_quality(unsigned int unvoiced_quality)
{
    this->unvoiced_quality = unvoiced_quality;
}

void mbe_synthesizer_impl::forecast(int noutput_items,
                                    gr_vector_int& ninput_items_required)
{
    ninput_items_required[0] = noutput_items / AudioSamples * bytes_per_frame;
}

int mbe_synthesizer_impl::general_work(int noutput_items,
                                       gr_vector_int& ninput_items,
                                       gr_vector_const_void_star& input_items,
                                       gr_vector_void_star& output_items)
{
    auto inbegin = static_cast<const unsigned char*>(input_items[0]);
    auto inend = inbegin + ninput_items[0];
    auto outbegin = static_cast<short*>(output_items[0]);
    auto outend = outbegin + noutput_items;

    auto in = inbegin;
    auto out = outbegin;
    while (true) {
        if (type == YSF) {
            unsigned int new_ysf_mode = *in;
            if (new_ysf_mode != ysf_mode)
                set_ysf_mode(new_ysf_mode);
        }
        auto innext = in + bytes_per_frame;
        auto outnext = out + AudioSamples;
        if (innext >= inend || outnext >= outend)
            break;

        auto indecode = in + extra_bytes;
        switch (mbeCodec) {
            case Ambe3600x2400:
                decodeAmbe3600x2400(indecode, reinterpret_cast<char*>(out));
                break;
            case Ambe3600x2450:
                decodeAmbe3600x2450(indecode, reinterpret_cast<char*>(out));
                break;
            case Ambe2450:
                decodeAmbe2450(indecode, reinterpret_cast<char*>(out));
                break;
            case Imbe7200x4400:
                decodeImbe7200x4400(indecode, reinterpret_cast<char*>(out));
                break;
            default:
                throw std::runtime_error("unknown MBE mode");
        }
        in = innext;
        out = outnext;
    }

    consume(0, in - inbegin);
    return out - outbegin;
}


/* internal functions */
void mbe_synthesizer_impl::init()
{
    curr_mp = (mbe_parms*) malloc(sizeof(mbe_parms));
    prev_mp = (mbe_parms*) malloc(sizeof(mbe_parms));
    prev_mp_enhanced = (mbe_parms*) malloc(sizeof(mbe_parms));
    mbe_initMbeParms(curr_mp, prev_mp, prev_mp_enhanced);

    mbeCodec = MBECodecUnknown;
    type = None;
    ysf_mode = 0;

    frameBits = 0;
    dataBits = 0;
    frameSize = 0;
    audioSize = 0;

    set_output_multiple(AudioSamples);
}

// D-Star
size_t mbe_synthesizer_impl::decodeAmbe3600x2400(const unsigned char *frame,
                                                 char* output)
{
    const int dW[9][8] = {
        { 0, 0, 3, 2, 1, 1, 0, 0 }, { 1, 1, 0, 0, 3, 2, 1, 1 },
        { 3, 2, 1, 1, 0, 0, 3, 2 }, { 0, 0, 3, 2, 1, 1, 0, 0 },
        { 1, 1, 0, 0, 3, 2, 1, 1 }, { 3, 2, 1, 1, 0, 0, 3, 2 },
        { 0, 0, 3, 2, 1, 1, 0, 0 }, { 1, 1, 0, 0, 3, 2, 1, 1 },
        { 3, 3, 2, 1, 0, 0, 3, 3 }
    };
    const int dX[9][8] = {
        { 10, 22, 11, 9, 10, 22, 11, 23 }, { 8, 20, 9, 21, 10, 8, 9, 21 },
        { 8, 6, 7, 19, 8, 20, 9, 7}, { 6, 18, 7, 5, 6, 18, 7, 19 },
        { 4, 16, 5, 17, 6, 4, 5, 17 }, { 4, 2, 3, 15, 4, 16, 5, 3 },
        { 2, 14, 3, 1, 2, 14, 3, 15 }, { 0, 12, 1, 13, 2, 0, 1, 13 },
        { 0, 12, 10, 11, 0, 12, 1, 13 }
    };


    char ambe_fr[4][24] = { 0 };
    char ambe_d[49] = { 0 };

    // deinterleave
    for (int i = 0; i < 9; ++i) {
        const int *w = dW[i];
        const int *x = dX[i];
        unsigned char frameByte = frame[i];
        ambe_fr[w[0]][x[0]] = (frameByte >> 0) & 0x01;
        ambe_fr[w[1]][x[1]] = (frameByte >> 1) & 0x01;
        ambe_fr[w[2]][x[2]] = (frameByte >> 2) & 0x01;
        ambe_fr[w[3]][x[3]] = (frameByte >> 3) & 0x01;
        ambe_fr[w[4]][x[4]] = (frameByte >> 4) & 0x01;
        ambe_fr[w[5]][x[5]] = (frameByte >> 5) & 0x01;
        ambe_fr[w[6]][x[6]] = (frameByte >> 6) & 0x01;
        ambe_fr[w[7]][x[7]] = (frameByte >> 7) & 0x01;
    }

    int errs = 0;
    int errs2 = 0;
    char err_str[64];
    mbe_processAmbe3600x2400Frame((short*) output, &errs, &errs2, err_str, ambe_fr, ambe_d, curr_mp, prev_mp, prev_mp_enhanced, unvoiced_quality);
//    if (errs || errs2 || err_str[0])
//        std::cerr << "mbe_processAmbe3600x2400Frame - errs=" << errs << " - errs2=" << errs2 << " - err_str=\"" << err_str << "\"\n";
    return audioSize;
}

// DMR, NXDN
size_t mbe_synthesizer_impl::decodeAmbe3600x2450(const unsigned char *frame,
                                                 char* output)
{
    const int dW[9][8] = {
        { 2, 1, 0, 0, 2, 1, 0, 0 }, { 2, 1, 0, 0, 2, 1, 0, 0 },
        { 3, 1, 0, 0, 3, 1, 0, 0 }, { 3, 1, 1, 0, 3, 1, 1, 0 },
        { 3, 1, 1, 0, 3, 1, 1, 0 }, { 3, 2, 1, 0, 3, 1, 1, 0 },
        { 3, 2, 1, 0, 3, 2, 1, 0 }, { 3, 2, 1, 0, 3, 2, 1, 0 },
        { 3, 2, 1, 0, 3, 2, 1, 0 }
    };
    const int dX[9][8] = {
        { 2, 9, 4, 22, 3, 10, 5, 23 }, { 0, 7, 2, 20, 1, 8, 3, 21 },
        { 12, 5, 0, 18, 13, 6, 1, 19 }, { 10, 3, 21, 16, 11, 4, 22, 17 },
        { 8, 1, 19, 14, 9, 2, 20, 15 }, { 6, 10, 17, 12, 7, 0, 18, 13 },
        { 4, 8, 15, 10, 5, 9, 16, 11 }, { 2, 6, 13, 8, 3, 7, 14, 9 },
        { 0, 4, 11, 6, 1, 5, 12, 7 }
    };


    char ambe_fr[4][24] = { 0 };
    char ambe_d[49] = { 0 };

    // deinterleave
    for (int i = 0; i < 9; ++i) {
        const int *w = dW[i];
        const int *x = dX[i];
        unsigned char frameByte = frame[i];
        ambe_fr[w[0]][x[0]] = (frameByte >> 0) & 0x01;
        ambe_fr[w[1]][x[1]] = (frameByte >> 1) & 0x01;
        ambe_fr[w[2]][x[2]] = (frameByte >> 2) & 0x01;
        ambe_fr[w[3]][x[3]] = (frameByte >> 3) & 0x01;
        ambe_fr[w[4]][x[4]] = (frameByte >> 4) & 0x01;
        ambe_fr[w[5]][x[5]] = (frameByte >> 5) & 0x01;
        ambe_fr[w[6]][x[6]] = (frameByte >> 6) & 0x01;
        ambe_fr[w[7]][x[7]] = (frameByte >> 7) & 0x01;
    }

    int errs = 0;
    int errs2 = 0;
    char err_str[64];
    mbe_processAmbe3600x2450Frame((short*) output, &errs, &errs2, err_str, ambe_fr, ambe_d, curr_mp, prev_mp, prev_mp_enhanced, unvoiced_quality);
//    if (errs || errs2 || err_str[0])
//        std::cerr << "mbe_processAmbe3600x2450Frame - errs=" << errs << " - errs2=" << errs2 << " - err_str=\"" << err_str << "\"\n";
    return audioSize;
}

// YSF - DN (Digital Narrow or V/D - Voice+Digital)
size_t mbe_synthesizer_impl::decodeAmbe2450(const unsigned char *frame,
                                            char* output)
{
    const int dX[7][8] = {
        { 20, 2, 37, 19, 1, 36, 18, 0 }, { 5, 40, 22, 4, 39, 21, 3, 38 },
        { 43, 25, 7, 42, 24, 6, 41, 23 }, { 28, 10, 45, 27, 9, 44, 26, 8 },
        { 13, 48, 30, 12, 47, 29, 11, 46 }, { 17, 34, 16, 33, 15, 32, 14, 31 },
        { 0, 0, 0, 0, 0, 0, 0, 35, }
    };


    char ambe_d[49] = { 0 };

    // deinterleave
    for (int i = 0; i < 6; ++i) {
        const int *x = dX[i];
        unsigned char frameByte = frame[i];
        ambe_d[x[0]] = (frameByte >> 0) & 0x01;
        ambe_d[x[1]] = (frameByte >> 1) & 0x01;
        ambe_d[x[2]] = (frameByte >> 2) & 0x01;
        ambe_d[x[3]] = (frameByte >> 3) & 0x01;
        ambe_d[x[4]] = (frameByte >> 4) & 0x01;
        ambe_d[x[5]] = (frameByte >> 5) & 0x01;
        ambe_d[x[6]] = (frameByte >> 6) & 0x01;
        ambe_d[x[7]] = (frameByte >> 7) & 0x01;
    }
    ambe_d[dX[6][7]] = (frame[6] >> 7) & 0x01;

    int errs = 0;
    int errs2 = 0;
    char err_str[64];
    mbe_processAmbe2450Data((short*) output, &errs, &errs2, err_str, ambe_d, curr_mp, prev_mp, prev_mp_enhanced, unvoiced_quality);
//    if (errs || errs2 || err_str[0])
//        std::cerr << "mbe_processAmbe2450Data - errs=" << errs << " - errs2=" << errs2 << " - err_str=\"" << err_str << "\"\n";
    return audioSize;
}

// YSF VW (Voice Wide or Voice FR)
size_t mbe_synthesizer_impl::decodeImbe7200x4400(const unsigned char *frame,
                                                 char* output)
{
    const int dW[18][8] = {
        { 0, 1, 5, 4, 3, 2, 1, 0 }, { 3, 2, 1, 0, 4, 5, 2, 3 },
        { 4, 6, 2, 3, 0, 1, 6, 4 }, { 0, 1, 6, 4, 3, 2, 1, 0 },
        { 3, 2, 1, 0, 4, 6, 2, 3 }, { 4, 6, 2, 3, 0, 1, 6, 4 },
        { 0, 1, 6, 4, 3, 2, 1, 0 }, { 3, 2, 1, 0, 4, 6, 2, 3 },
        { 5, 6, 2, 3, 0, 1, 6, 4 }, { 0, 1, 6, 5, 3, 2, 1, 0 },
        { 3, 2, 1, 0, 5, 6, 2, 3 }, { 5, 6, 2, 3, 0, 1, 6, 5 },
        { 0, 1, 6, 5, 3, 2, 1, 0 }, { 3, 2, 1, 0, 5, 7, 2, 3 },
        { 5, 7, 2, 3, 0, 1, 7, 5 }, { 0, 1, 7, 5, 4, 2, 1, 0 },
        { 4, 3, 2, 0, 5, 7, 3, 4 }, { 5, 7, 3, 4, 1, 2, 7, 5 }
    };
    const int dX[18][8] = {
        { 21, 20, 1, 10, 19, 20, 21, 22 }, { 17, 18, 19, 20, 9, 0, 19, 18 },
        { 7, 13, 17, 16, 19, 18, 14, 8 }, { 17, 16, 12, 6, 15, 16, 17, 18 },
        { 13, 14, 15, 16, 5, 11, 15, 14 }, { 3, 9, 13, 12, 15, 14, 10, 4 },
        { 13, 12, 8, 2, 11, 12, 13, 14 }, { 9, 10, 11, 12, 1, 7, 11, 10 },
        { 14, 5, 9, 8, 11, 10, 6, 0 }, { 9, 8, 4, 13, 7, 8, 9, 10 },
        { 5, 6, 7, 8, 12, 3, 7, 6 }, { 10, 1, 5, 4, 7, 6, 2, 11 },
        { 5, 4, 0, 9, 3, 4, 5, 6 }, { 1, 2, 3, 4, 8, 6, 3, 2 },
        { 6, 4, 1, 0, 3, 2, 5, 7 }, { 1, 0, 3, 5, 14, 0, 1, 2 },
        { 12, 21, 22, 0, 4, 2, 22, 13 }, { 2, 0, 20, 11, 22, 21, 1, 3 }
    };


    char imbe_fr[8][23] = { 0 };
    char imbe_d[88] = { 0 };

    // deinterleave
    for (int i = 0; i < 18; ++i) {
        const int *w = dW[i];
        const int *x = dX[i];
        unsigned char frameByte = frame[i];
        imbe_fr[w[0]][x[0]] = (frameByte >> 0) & 0x01;
        imbe_fr[w[1]][x[1]] = (frameByte >> 1) & 0x01;
        imbe_fr[w[2]][x[2]] = (frameByte >> 2) & 0x01;
        imbe_fr[w[3]][x[3]] = (frameByte >> 3) & 0x01;
        imbe_fr[w[4]][x[4]] = (frameByte >> 4) & 0x01;
        imbe_fr[w[5]][x[5]] = (frameByte >> 5) & 0x01;
        imbe_fr[w[6]][x[6]] = (frameByte >> 6) & 0x01;
        imbe_fr[w[7]][x[7]] = (frameByte >> 7) & 0x01;
    }

    int errs = 0;
    int errs2 = 0;
    char err_str[64];
    mbe_processImbe7200x4400Frame((short*) output, &errs, &errs2, err_str, imbe_fr, imbe_d, curr_mp, prev_mp, prev_mp_enhanced, unvoiced_quality);
//    if (errs || errs2 || err_str[0])
//        std::cerr << "mbe_processImbe7200x4400Frame - errs=" << errs << " - errs2=" << errs2 << " - err_str=\"" << err_str << "\"\n";
    return audioSize;
}

} /* namespace digiham */
} /* namespace gr */
