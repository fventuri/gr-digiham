/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "pocsag_decoder_impl.h"
#include "meta.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace digiham {

pocsag_decoder::sptr pocsag_decoder::make()
{
    return gnuradio::make_block_sptr<pocsag_decoder_impl>();
}

pocsag_decoder_impl::pocsag_decoder_impl()
    : gr::sync_block("pocsag_decoder",
                     gr::io_signature::make(1, 1, sizeof(unsigned char)),
                     gr::io_signature::make(0, 0, 0))
{
    module = new Digiham::Pocsag::Decoder();
    reader = new Reader<unsigned char>();
    module->setReader(reader);

    module->setMetaWriter(new MessageMetaWriter(this, "metadata"));
}

pocsag_decoder_impl::~pocsag_decoder_impl()
{
    delete module;
    module = nullptr;
    delete reader;
    reader = nullptr;
}

int pocsag_decoder_impl::work(int noutput_items,
                              gr_vector_const_void_star& input_items,
                              gr_vector_void_star& output_items)
{
    auto in = static_cast<const unsigned char*>(input_items[0]);

    reader->set(noutput_items, in);
    while (module->canProcess())
        module->process();
    return noutput_items - reader->available();
}

} /* namespace digiham */
} /* namespace gr */
