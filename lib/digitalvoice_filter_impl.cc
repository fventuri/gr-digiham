/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "digitalvoice_filter_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace digiham {

digitalvoice_filter::sptr digitalvoice_filter::make()
{
    return gnuradio::make_block_sptr<digitalvoice_filter_impl>();
}

digitalvoice_filter_impl::digitalvoice_filter_impl()
    : gr::sync_block("digitalvoice_filter",
                     gr::io_signature::make(1, 1, sizeof(short)),
                     gr::io_signature::make(1, 1, sizeof(short)))
{
    module = new Digiham::DigitalVoice::DigitalVoiceFilter();
}

digitalvoice_filter_impl::~digitalvoice_filter_impl()
{
    delete module;
    module = nullptr;
}

int digitalvoice_filter_impl::work(int noutput_items,
                                   gr_vector_const_void_star& input_items,
                                   gr_vector_void_star& output_items)
{
    auto in = static_cast<const short*>(input_items[0]);
    auto out = static_cast<short*>(output_items[0]);
    module->process(const_cast<short*>(in), out, noutput_items);
    return noutput_items;
}

} /* namespace digiham */
} /* namespace gr */
