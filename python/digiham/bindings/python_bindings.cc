/*
 * Copyright 2020 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

#include <pybind11/pybind11.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

namespace py = pybind11;

// Headers for binding functions
/**************************************/
// The following comment block is used for
// gr_modtool to insert function prototypes
// Please do not delete
/**************************************/
// BINDING_FUNCTION_PROTOTYPES(
    void bind_fsk_demodulator(py::module& m);
    void bind_gfsk_demodulator(py::module& m);
    void bind_dc_block(py::module& m);
    void bind_rrc_filter(py::module& m);
    void bind_digitalvoice_filter(py::module& m);
    void bind_dmr_decoder(py::module& m);
    void bind_dstar_decoder(py::module& m);
    void bind_nxdn_decoder(py::module& m);
    void bind_pocsag_decoder(py::module& m);
    void bind_ysf_decoder(py::module& m);
    void bind_mbe_synthesizer(py::module& m);
// ) END BINDING_FUNCTION_PROTOTYPES


// We need this hack because import_array() returns NULL
// for newer Python versions.
// This function is also necessary because it ensures access to the C API
// and removes a warning.
void* init_numpy()
{
    import_array();
    return NULL;
}

PYBIND11_MODULE(digiham_python, m)
{
    // Initialize the numpy C API
    // (otherwise we will see segmentation faults)
    init_numpy();

    // Allow access to base block methods
    py::module::import("gnuradio.gr");

    /**************************************/
    // The following comment block is used for
    // gr_modtool to insert binding function calls
    // Please do not delete
    /**************************************/
    // BINDING_FUNCTION_CALLS(
    bind_fsk_demodulator(m);
    bind_gfsk_demodulator(m);
    bind_dc_block(m);
    bind_rrc_filter(m);
    bind_digitalvoice_filter(m);
    bind_dmr_decoder(m);
    bind_dstar_decoder(m);
    bind_nxdn_decoder(m);
    bind_pocsag_decoder(m);
    bind_ysf_decoder(m);
    bind_mbe_synthesizer(m);
    // ) END BINDING_FUNCTION_CALLS
}