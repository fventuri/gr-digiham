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
    typedef enum {None, DMR, DStar, YSF} DVMbeType;
    static sptr make(DVMbeType type, unsigned int unvoiced_quality = 3);

    static sptr make(unsigned int index, unsigned int bitrate, unsigned int unvoiced_quality = 3);

    static sptr make(unsigned short* cwds, unsigned int unvoiced_quality = 3);

    /*!
     * Set the MBE codec
     *
     * \param type DV MBE type (one of: DMR, DStar, YSF)
     */
    virtual void set_codec(DVMbeType type) = 0;

    /*!
     * Set the MBE codec
     *
     * \param index DV MBE index
     * \param bitrate DV bitrate
     * \param extra_bytes number of additional bytes to compute the frame rate
     */
    virtual void set_codec(unsigned int index, unsigned int bitrate,
                           unsigned int extra_bytes = 0) = 0;

    /*!
     * Set the MBE codec
     *
     * \param cwds DV MBE control words (array of six shorts)
     * \param extra_bytes number of additional bytes to compute the frame rate
     */
    virtual void set_codec(unsigned short* cwds, unsigned int extra_bytes = 0) = 0;

    /*!
     * Set the YSF mode
     *
     * \param mode YSF mode (0=V1, 2=V2/DN, 3=WV)
     */
    virtual void set_ysf_mode(unsigned char mode) = 0;

    /*!
     * Set unvoiced quality
     *
     * \param unvoiced_quality unvoiced quality
     */
    virtual void set_unvoiced_quality(unsigned int unvoiced_quality) = 0;
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_MBE_SYNTHESIZER_H */
