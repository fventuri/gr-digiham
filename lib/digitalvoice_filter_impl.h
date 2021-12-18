/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_DIGITALVOICE_FILTER_IMPL_H
#define INCLUDED_DIGIHAM_DIGITALVOICE_FILTER_IMPL_H

#include <digiham/digitalvoice_filter.hpp>
#include <gnuradio/digiham/digitalvoice_filter.h>

namespace gr {
namespace digiham {

class digitalvoice_filter_impl : public digitalvoice_filter
{
private:
    Digiham::DigitalVoice::DigitalVoiceFilter* module;

public:
    digitalvoice_filter_impl();
    ~digitalvoice_filter_impl();

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_DIGITALVOICE_FILTER_IMPL_H */
