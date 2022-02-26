/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_MBE_SYNTHESIZER_IMPL_H
#define INCLUDED_DIGIHAM_MBE_SYNTHESIZER_IMPL_H

#include "reader.h"
#include "writer.h"
#include <gnuradio/digiham/mbe_synthesizer_mbelib.h>
extern "C" {
#include <mbelib.h>
}

namespace gr {
namespace digiham {

class mbe_synthesizer_impl : public mbe_synthesizer
{

private:
    // 50ms x 8kHz sampling rate = 160 samples
    // 160 sample x 16 bit (2 bytes) = 320 bytes
    static int constexpr AudioSamples = 160;
    static int constexpr AudioSize = 320;

    typedef enum {
        MBECodecUnknown,
        Ambe3600x2400,              // D-Star
        Ambe3600x2450,              // DMR, dPMR, YSF V/D type 1 (DN), NXDN
        Ambe2450,                   // YSF V/D type 2 (does not use FEC in AMBE codec)
        Imbe7200x4400               // YSF VW
    } MBECodec;

public:
    mbe_synthesizer_impl(DVMbeType type, unsigned int unvoiced_quality);
    mbe_synthesizer_impl(unsigned int index, unsigned int bitrate,
                         unsigned int unvoiced_quality);
    mbe_synthesizer_impl(unsigned short* cwds, unsigned int unvoiced_quality);
    ~mbe_synthesizer_impl();

    void set_codec(DVMbeType type);
    void set_codec(unsigned int index, unsigned int bitrate, unsigned int extra_bytes);
    void set_codec(unsigned short* cwds, unsigned int extra_bytes);
    void set_ysf_mode(unsigned char mode);
    void set_unvoiced_quality(unsigned int unvoiced_quality);

    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);

private:
    void init();
    size_t decodeAmbe3600x2400(const unsigned char* frame, char* output);
    size_t decodeAmbe3600x2450(const unsigned char* frame, char* output);
    size_t decodeAmbe2450(const unsigned char* frame, char* output);
    size_t decodeImbe7200x4400(const unsigned char* frame, char* output);

    unsigned int unvoiced_quality;
    mbe_parms *curr_mp;
    mbe_parms *prev_mp;
    mbe_parms *prev_mp_enhanced;
    MBECodec mbeCodec;
    DVMbeType type;
    unsigned char ysf_mode;
    size_t frameBits;
    size_t dataBits;
    size_t frameSize;
    size_t audioSize;
    unsigned int bitrate;
    unsigned int bytes_per_frame;  // extra_bytes + ceil(bitrate / 50 / 8)
    unsigned int extra_bytes;
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_MBE_SYNTHESIZER_IMPL_H */
