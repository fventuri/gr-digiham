/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "rrc_filter_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace digiham {

rrc_filter::sptr rrc_filter::make(unsigned int nZeros, double gain, const float coeffs[])
{
    return gnuradio::make_block_sptr<rrc_filter_impl>(nZeros, gain, coeffs);
}

rrc_filter::sptr rrc_filter::make(RrcFilterType type)
{
    return gnuradio::make_block_sptr<rrc_filter_impl>(type);
}

rrc_filter_impl::rrc_filter_impl(unsigned int nZeros, double gain, const float coeffs[])
    : gr::sync_block("rrc_filter",
                     gr::io_signature::make(1, 1, sizeof(float)),
                     gr::io_signature::make(1, 1, sizeof(float)))
{
    module = new Digiham::RrcFilter::RrcFilter(nZeros, gain, coeffs);
}

rrc_filter_impl::rrc_filter_impl(RrcFilterType type)
    : gr::sync_block("rrc_filter",
                     gr::io_signature::make(1, 1, sizeof(float)),
                     gr::io_signature::make(1, 1, sizeof(float)))
{
    switch (type) {
        case Narrow:
            module = new Digiham::RrcFilter::NarrowRrcFilter();
            break;
        case Wide:
            module = new Digiham::RrcFilter::WideRrcFilter();
            break;
    }
}

rrc_filter_impl::~rrc_filter_impl()
{
    delete module;
    module = nullptr;

}

int rrc_filter_impl::work(int noutput_items,
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
