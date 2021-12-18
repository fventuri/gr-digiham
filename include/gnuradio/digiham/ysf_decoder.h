/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_YSF_DECODER_H
#define INCLUDED_DIGIHAM_YSF_DECODER_H

#include <gnuradio/block.h>
#include <gnuradio/digiham/api.h>

namespace gr {
namespace digiham {

/*!
 * \brief <+description of block+>
 * \ingroup digiham
 *
 */
class DIGIHAM_API ysf_decoder : virtual public gr::block
{
public:
    typedef std::shared_ptr<ysf_decoder> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of digiham::ysf_decoder.
     *
     * To avoid accidental use of raw pointers, digiham::ysf_decoder's
     * constructor is in a private implementation
     * class. digiham::ysf_decoder::make is the public interface for
     * creating new instances.
     */
    static sptr make();
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_YSF_DECODER_H */
