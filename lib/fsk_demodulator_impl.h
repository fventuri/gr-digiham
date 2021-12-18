/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_FSK_DEMODULATOR_IMPL_H
#define INCLUDED_DIGIHAM_FSK_DEMODULATOR_IMPL_H

#include "reader.h"
#include "writer.h"
#include <digiham/fsk_demodulator.hpp>
#include <gnuradio/digiham/fsk_demodulator.h>

namespace gr {
namespace digiham {

class fsk_demodulator_impl : public fsk_demodulator
{
private:
    unsigned int samplesPerSymbol;
    Digiham::Fsk::FskDemodulator* module;
    Reader<float>* reader;
    Writer<unsigned char>* writer;

public:
    fsk_demodulator_impl(unsigned int samplesPerSymbol, bool invert);
    ~fsk_demodulator_impl();

    void forecast(int noutput_items, gr_vector_int& ninput_items_required);

    int general_work(int noutput_items,
                     gr_vector_int& ninput_items,
                     gr_vector_const_void_star& input_items,
                     gr_vector_void_star& output_items);
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_FSK_DEMODULATOR_IMPL_H */
