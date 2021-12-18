/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_POCSAG_DECODER_H
#define INCLUDED_DIGIHAM_POCSAG_DECODER_H

#include <gnuradio/digiham/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace digiham {

/*!
 * \brief <+description of block+>
 * \ingroup digiham
 *
 */
class DIGIHAM_API pocsag_decoder : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<pocsag_decoder> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of digiham::pocsag_decoder.
     *
     * To avoid accidental use of raw pointers, digiham::pocsag_decoder's
     * constructor is in a private implementation
     * class. digiham::pocsag_decoder::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_POCSAG_DECODER_H */
