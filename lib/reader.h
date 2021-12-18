/* -*- c++ -*- */
/*
 * Copyright 2021 Franco Venturi.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DIGIHAM_READER_H
#define INCLUDED_DIGIHAM_READER_H

#include <csdr/reader.hpp>

namespace gr {
namespace digiham {

template <class T>
class Reader : public Csdr::Reader<T>
{
private:
    size_t _available;
    const T* _read_pointer;

public:
    Reader():
       _available(0),
       _read_pointer(nullptr)
    {}
    ~Reader() {}
    size_t available() { return _available; }
    void advance(size_t how_much) { _read_pointer += how_much; _available -= how_much; }
    void wait() {}
    void unblock() {}
    T* getReadPointer() { return const_cast<T*>(_read_pointer); }

    void set(int ninput_items, const T* in) {
        _available = ninput_items;
        _read_pointer = in;
    }
};

} // namespace digiham
} // namespace gr

#endif /* INCLUDED_DIGIHAM_READER_H */
