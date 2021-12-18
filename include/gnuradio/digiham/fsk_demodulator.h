/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_FSK_DEMODULATOR_H
#define INCLUDED_DIGIHAM_FSK_DEMODULATOR_H

#include <gnuradio/block.h>
#include <gnuradio/digiham/api.h>

namespace gr {
namespace digiham {

/*!
 * \brief <+description of block+>
 * \ingroup digiham
 *
 */
class DIGIHAM_API fsk_demodulator : virtual public gr::block
{
public:
    typedef std::shared_ptr<fsk_demodulator> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of digiham::fsk_demodulator.
     *
     * To avoid accidental use of raw pointers, digiham::fsk_demodulator's
     * constructor is in a private implementation
     * class. digiham::fsk_demodulator::make is the public interface for
     * creating new instances.
     */
    static sptr make(unsigned int samplesPerSymbol, bool invert = false);
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_FSK_DEMODULATOR_H */
