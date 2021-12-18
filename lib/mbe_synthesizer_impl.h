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
#include <digiham/mbe_synthesizer.hpp>
#include <gnuradio/digiham/mbe_synthesizer.h>

namespace gr {
namespace digiham {

class mbe_synthesizer_impl : public mbe_synthesizer
{
private:
    Digiham::Mbe::MbeSynthesizer* module;
    Reader<unsigned char>* reader;
    Writer<short>* writer;
    unsigned int bitrate;
    unsigned int bytes_per_frame;  // ceil(bitrate / 50 / 8)

public:
    mbe_synthesizer_impl(DVMbeType type);
    mbe_synthesizer_impl(unsigned int index, unsigned int bitrate);
    mbe_synthesizer_impl(short* cwds);
    ~mbe_synthesizer_impl();

    void set_mode(DVMbeType type);
    void set_mode(unsigned int index, unsigned int bitrate);
    void set_mode(short* cwds);

    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_MBE_SYNTHESIZER_IMPL_H */
