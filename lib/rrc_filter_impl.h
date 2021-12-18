/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_RRC_FILTER_IMPL_H
#define INCLUDED_DIGIHAM_RRC_FILTER_IMPL_H

#include <digiham/rrc_filter.hpp>
#include <gnuradio/digiham/rrc_filter.h>

namespace gr {
namespace digiham {

class rrc_filter_impl : public rrc_filter
{
private:
    Digiham::RrcFilter::RrcFilter* module;

public:
    rrc_filter_impl(unsigned int nZeros, double gain, const float coeffs[]);
    rrc_filter_impl(RrcFilterType type);
    ~rrc_filter_impl();

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_RRC_FILTER_IMPL_H */
