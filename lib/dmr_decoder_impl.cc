/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "dmr_decoder_impl.h"
#include "meta.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace digiham {

dmr_decoder::sptr dmr_decoder::make(unsigned char slotFilter)
{
    return gnuradio::make_block_sptr<dmr_decoder_impl>(slotFilter);
}

dmr_decoder_impl::dmr_decoder_impl(unsigned char slotFilter)
    : gr::block("dmr_decoder",
                gr::io_signature::make(1, 1, sizeof(unsigned char)),
                gr::io_signature::make(1, 1, sizeof(unsigned char)))
{
    module = new Digiham::Dmr::Decoder();
    module->setSlotFilter(slotFilter);
    reader = new Reader<unsigned char>();
    module->setReader(reader);
    writer = new Writer<unsigned char>();
    module->setWriter(writer);

    // see: https://github.com/jketterl/digiham/blob/develop/src/dmr_decoder/dmr_phase.cpp#L226
    set_output_multiple(27);

    module->setMetaWriter(new MessageMetaWriter(this, "metadata"));
}

dmr_decoder_impl::~dmr_decoder_impl()
{
    delete module;
    module = nullptr;
    delete reader;
    reader = nullptr;
    delete writer;
    writer = nullptr;
}

void dmr_decoder_impl::set_slot_filter(unsigned char slotFilter)
{
    module->setSlotFilter(slotFilter);
}

void dmr_decoder_impl::forecast(int noutput_items,
                                gr_vector_int& ninput_items_required)
{
    // input: 4800 dibits/s
    // output: 3600 bit/s (DMR) -> 3600/8=450 bytes/s
    ninput_items_required[0] = noutput_items * 4800 / 450;
}

int dmr_decoder_impl::general_work(int noutput_items,
                                   gr_vector_int& ninput_items,
                                   gr_vector_const_void_star& input_items,
                                   gr_vector_void_star& output_items)
{
    auto in = static_cast<const unsigned char*>(input_items[0]);
    auto out = static_cast<unsigned char*>(output_items[0]);

    reader->set(ninput_items[0], in);
    writer->set(noutput_items, out);
    while (module->canProcess())
        module->process();
    consume(0, ninput_items[0] - reader->available());
    return noutput_items - writer->writeable();
}

} /* namespace digiham */
} /* namespace gr */
