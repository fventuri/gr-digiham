/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_GFSK_DEMODULATOR_IMPL_H
#define INCLUDED_DIGIHAM_GFSK_DEMODULATOR_IMPL_H

#include "reader.h"
#include "writer.h"
#include <digiham/gfsk_demodulator.hpp>
#include <gnuradio/digiham/gfsk_demodulator.h>

namespace gr {
namespace digiham {

class gfsk_demodulator_impl : public gfsk_demodulator
{
private:
    unsigned int samplesPerSymbol;
    Digiham::Fsk::GfskDemodulator* module;
    Reader<float>* reader;
    Writer<unsigned char>* writer;

public:
    gfsk_demodulator_impl(unsigned int samplesPerSymbol);
    ~gfsk_demodulator_impl();

    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_GFSK_DEMODULATOR_IMPL_H */
