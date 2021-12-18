/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_DMR_DECODER_H
#define INCLUDED_DIGIHAM_DMR_DECODER_H

#include <gnuradio/block.h>
#include <gnuradio/digiham/api.h>

namespace gr {
namespace digiham {

/*!
 * \brief <+description of block+>
 * \ingroup digiham
 *
 */
class DIGIHAM_API dmr_decoder : virtual public gr::block
{
public:
    typedef std::shared_ptr<dmr_decoder> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of digiham::dmr_decoder.
     *
     * To avoid accidental use of raw pointers, digiham::dmr_decoder's
     * constructor is in a private implementation
     * class. digiham::dmr_decoder::make is the public interface for
     * creating new instances.
     */
    static sptr make(unsigned char slotFilter = 3);

    /*!
     * Set the TDMA slot filter
     *
     * \param slotFilter TDMA slot filter
     */
    virtual void set_slot_filter(unsigned char slotFilter) = 0;
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_DMR_DECODER_H */
