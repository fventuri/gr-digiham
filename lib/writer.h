/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_WRITER_H
#define INCLUDED_DIGIHAM_WRITER_H

#include <csdr/writer.hpp>

namespace gr {
namespace digiham {

template <class T>
class Writer : public Csdr::Writer<T>
{
private:
    size_t _writeable;
    T* _write_pointer;

public:
    Writer():
       _writeable(0),
       _write_pointer(nullptr)
    {}
    ~Writer() {}
    size_t writeable() { return _writeable; }
    void advance(size_t how_much) { _write_pointer += how_much; _writeable -= how_much; }
    T* getWritePointer() { return _write_pointer; }

    void set(int noutput_items, T* out) {
        _writeable = noutput_items;
        _write_pointer = out;
    }
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_WRITER_H */
