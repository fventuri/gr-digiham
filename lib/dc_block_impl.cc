/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "dc_block_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace digiham {

dc_block::sptr dc_block::make() {
    return gnuradio::make_block_sptr<dc_block_impl>();
}

dc_block_impl::dc_block_impl()
    : gr::sync_block("dc_block",
                     gr::io_signature::make(1, 1, sizeof(float)),
                     gr::io_signature::make(1, 1, sizeof(float)))
{
    module = new Csdr::DcBlock();
}

dc_block_impl::~dc_block_impl()
{
    delete module;
    module = nullptr;
}

int dc_block_impl::work(int noutput_items,
                        gr_vector_const_void_star& input_items,
                        gr_vector_void_star& output_items)
{
    auto in = static_cast<const float*>(input_items[0]);
    auto out = static_cast<float*>(output_items[0]);
    module->process(const_cast<float*>(in), out, noutput_items);
    return noutput_items;
}

} /* namespace digiham */
} /* namespace gr */
