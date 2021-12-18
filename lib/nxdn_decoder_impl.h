/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_NXDN_DECODER_IMPL_H
#define INCLUDED_DIGIHAM_NXDN_DECODER_IMPL_H

#include "reader.h"
#include "writer.h"
#include <digiham/nxdn_decoder.hpp>
#include <gnuradio/digiham/nxdn_decoder.h>

namespace gr {
namespace digiham {

class nxdn_decoder_impl : public nxdn_decoder
{
private:
    Digiham::Nxdn::Decoder* module;
    Reader<unsigned char>* reader;
    Writer<unsigned char>* writer;

public:
    nxdn_decoder_impl();
    ~nxdn_decoder_impl();

    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_NXDN_DECODER_IMPL_H */
