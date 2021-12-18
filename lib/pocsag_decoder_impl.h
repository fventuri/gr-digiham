/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_POCSAG_DECODER_IMPL_H
#define INCLUDED_DIGIHAM_POCSAG_DECODER_IMPL_H

#include "reader.h"
#include "writer.h"
#include <digiham/pocsag_decoder.hpp>
#include <gnuradio/digiham/pocsag_decoder.h>

namespace gr {
namespace digiham {

class pocsag_decoder_impl : public pocsag_decoder
{
private:
    Digiham::Pocsag::Decoder* module;
    Reader<unsigned char>* reader;

public:
    pocsag_decoder_impl();
    ~pocsag_decoder_impl();

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_POCSAG_DECODER_IMPL_H */
