/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_MBE_SYNTHESIZER_H
#define INCLUDED_DIGIHAM_MBE_SYNTHESIZER_H

#include <gnuradio/block.h>
#include <gnuradio/digiham/api.h>

namespace gr {
namespace digiham {

/*!
 * \brief <+description of block+>
 * \ingroup digiham
 *
 */
class DIGIHAM_API mbe_synthesizer : virtual public gr::block
{
public:
    typedef std::shared_ptr<mbe_synthesizer> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of digiham::mbe_synthesizer.
     *
     * To avoid accidental use of raw pointers, digiham::mbe_synthesizer's
     * constructor is in a private implementation
     * class. digiham::mbe_synthesizer::make is the public interface for
     * creating new instances.
     */
    typedef enum {DMR=1, DStar=2, YSF=3} DVMbeType;
    static sptr make(DVMbeType type);

    static sptr make(unsigned int index, unsigned int bitrate);

    static sptr make(short* cwds);

    /*!
     * Set the MBE mode
     *
     * \param type DV MBE type (one of: DMR, DStar, YSF)
     */
    virtual void set_mode(DVMbeType type) = 0;

    /*!
     * Set the MBE mode
     *
     * \param index DV MBE index
     * \param bitrate DV bitrate
     */
    virtual void set_mode(unsigned int index, unsigned int bitrate) = 0;

    /*!
     * Set the MBE mode
     *
     * \param cwds DV MBE control words (array of six shorts)
     */
    virtual void set_mode(short* cwds) = 0;
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_MBE_SYNTHESIZER_H */
