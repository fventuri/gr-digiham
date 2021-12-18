/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_RRC_FILTER_H
#define INCLUDED_DIGIHAM_RRC_FILTER_H

#include <gnuradio/digiham/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace digiham {

/*!
 * \brief <+description of block+>
 * \ingroup digiham
 *
 */
class DIGIHAM_API rrc_filter : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<rrc_filter> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of digiham::rrc_filter.
     *
     * To avoid accidental use of raw pointers, digiham::rrc_filter's
     * constructor is in a private implementation
     * class. digiham::rrc_filter::make is the public interface for
     * creating new instances.
     */
    static sptr make(unsigned int nZeros, double gain, const float coeffs[]);

    typedef enum {Narrow=1, Wide=2} RrcFilterType;
    static sptr make(RrcFilterType type);
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_RRC_FILTER_H */
