/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_DC_BLOCK_IMPL_H
#define INCLUDED_DIGIHAM_DC_BLOCK_IMPL_H

#include <csdr/dcblock.hpp>
#include <gnuradio/digiham/dc_block.h>

namespace gr {
namespace digiham {

class dc_block_impl : public dc_block
{
private:
    Csdr::DcBlock* module;

public:
    dc_block_impl();
    ~dc_block_impl();

    int work(int noutput_items,
             gr_vector_const_void_star& input_items,
             gr_vector_void_star& output_items);
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_DC_BLOCK_IMPL_H */
