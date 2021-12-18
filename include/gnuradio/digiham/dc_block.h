/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_DC_BLOCK_H
#define INCLUDED_DIGIHAM_DC_BLOCK_H

#include <gnuradio/digiham/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace digiham {

/*!
 * \brief <+description of block+>
 * \ingroup digiham
 *
 */
class DIGIHAM_API dc_block : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<dc_block> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of digiham::dc_block.
     *
     * To avoid accidental use of raw pointers, digiham::dc_block's
     * constructor is in a private implementation
     * class. digiham::dc_block::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_DC_BLOCK_H */
