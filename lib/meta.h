/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_META_H
#define INCLUDED_DIGIHAM_META_H

#include <digiham/meta.hpp>
#include <gnuradio/basic_block.h>

namespace gr {
namespace digiham {

class MessageMetaWriter : public Digiham::MetaWriter
{
private:
    basic_block* _block;
    pmt::pmt_t _port_id;

public:
    MessageMetaWriter(basic_block* block, const std::string& port_id):
       _block(block),
       _port_id(pmt::intern(port_id))
    { block->message_port_register_out(_port_id); }
    ~MessageMetaWriter() {}
    void sendMetaData(std::map<std::string, std::string> metadata) {
        if (serializer == nullptr) {
           pmt::pmt_t metaDict = pmt::make_dict();
           for (auto const& entry : metadata) {
                metaDict = pmt::dict_add(metaDict, pmt::string_to_symbol(entry.first), pmt::string_to_symbol(entry.second));
           }
           _block->message_port_pub(_port_id, metaDict);
        } else {
           std::string metaString = serializer->serializeMetaData(metadata);
           _block->message_port_pub(_port_id, pmt::string_to_symbol(metaString));
        }
    }
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_META_H */
