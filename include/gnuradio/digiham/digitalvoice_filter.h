/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_DIGITALVOICE_FILTER_H
#define INCLUDED_DIGIHAM_DIGITALVOICE_FILTER_H

#include <gnuradio/digiham/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace digiham {

/*!
 * \brief <+description of block+>
 * \ingroup digiham
 *
 */
class DIGIHAM_API digitalvoice_filter : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<digitalvoice_filter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of digiham::digitalvoice_filter.
     *
     * To avoid accidental use of raw pointers, digiham::digitalvoice_filter's
     * constructor is in a private implementation
     * class. digiham::digitalvoice_filter::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_DIGITALVOICE_FILTER_H */
