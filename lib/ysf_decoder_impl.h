/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_YSF_DECODER_IMPL_H
#define INCLUDED_DIGIHAM_YSF_DECODER_IMPL_H

#include "reader.h"
#include "writer.h"
#include <digiham/ysf_decoder.hpp>
#include <gnuradio/digiham/ysf_decoder.h>

namespace gr {
namespace digiham {

class ysf_decoder_impl : public ysf_decoder
{
private:
    Digiham::Ysf::Decoder* module;
    Reader<unsigned char>* reader;
    Writer<unsigned char>* writer;

public:
    ysf_decoder_impl();
    ~ysf_decoder_impl();

    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_YSF_DECODER_IMPL_H */
