/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "ysf_decoder_impl.h"
#include "meta.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace digiham {

ysf_decoder::sptr ysf_decoder::make()
{
    return gnuradio::make_block_sptr<ysf_decoder_impl>();
}

ysf_decoder_impl::ysf_decoder_impl()
    : gr::block("ysf_decoder",
                gr::io_signature::make(1, 1, sizeof(unsigned char)),
                gr::io_signature::make(1, 1, sizeof(unsigned char)))
{
    module = new Digiham::Ysf::Decoder();
    reader = new Reader<unsigned char>();
    module->setReader(reader);
    writer = new Writer<unsigned char>();
    module->setWriter(writer);

    // YSF - DN
    // see: https://github.com/jketterl/digiham/blob/develop/src/ysf_decoder/ysf_phase.cpp#L95
    set_output_multiple(8);

    module->setMetaWriter(new MessageMetaWriter(this, "metadata"));
}

ysf_decoder_impl::~ysf_decoder_impl() {
    delete module;
    module = nullptr;
    delete reader;
    reader = nullptr;
    delete writer;
    writer = nullptr;
}

void ysf_decoder_impl::forecast(int noutput_items, gr_vector_int& ninput_items_required)
{
    // input: 4800 dibits/s
    // output: 2450 bit/s (YSF - DN) -> 2450->3200/8=400 bytes/s
    ninput_items_required[0] = noutput_items * 4800 / 400;
}

int ysf_decoder_impl::general_work(int noutput_items,
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
