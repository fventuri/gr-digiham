/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "gfsk_demodulator_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace digiham {

gfsk_demodulator::sptr gfsk_demodulator::make(unsigned int samplesPerSymbol)
{
    return gnuradio::make_block_sptr<gfsk_demodulator_impl>(samplesPerSymbol);
}

gfsk_demodulator_impl::gfsk_demodulator_impl(unsigned int samplesPerSymbol)
    : gr::block("gfsk_demodulator",
                gr::io_signature::make(1, 1, sizeof(float)),
                gr::io_signature::make(1, 1, sizeof(unsigned char))),
      samplesPerSymbol(samplesPerSymbol)
{
    module = new Digiham::Fsk::GfskDemodulator(samplesPerSymbol);
    reader = new Reader<float>();
    module->setReader(reader);
    writer = new Writer<unsigned char>();
    module->setWriter(writer);
}

gfsk_demodulator_impl::~gfsk_demodulator_impl()
{
    delete module;
    module = nullptr;
    delete reader;
    reader = nullptr;
    delete writer;
    writer = nullptr;
}

void gfsk_demodulator_impl::forecast(int noutput_items,
                                     gr_vector_int& ninput_items_required)
{
    ninput_items_required[0] = noutput_items * samplesPerSymbol;
}

int gfsk_demodulator_impl::general_work(int noutput_items,
                                        gr_vector_int& ninput_items,
                                        gr_vector_const_void_star& input_items,
                                        gr_vector_void_star& output_items)
{
    auto in = static_cast<const float*>(input_items[0]);
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
