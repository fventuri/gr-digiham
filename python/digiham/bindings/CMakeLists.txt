# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT digiham_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT digiham_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND digiham_python_files
    fsk_demodulator_python.cc
    gfsk_demodulator_python.cc
    dc_block_python.cc
    rrc_filter_python.cc
    digitalvoice_filter_python.cc
    dmr_decoder_python.cc
    dstar_decoder_python.cc
    nxdn_decoder_python.cc
    pocsag_decoder_python.cc
    ysf_decoder_python.cc
    mbe_synthesizer_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(digiham
   ../../..
   gr::digiham
   "${digiham_python_files}")

# copy in bindings .so file for use in QA test module
add_custom_target(
  copy_bindings_for_tests ALL
  COMMAND
    ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/*.so"
    ${CMAKE_BINARY_DIR}/test_modules/gnuradio/digiham/
  DEPENDS digiham_python)

install(TARGETS digiham_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/digiham COMPONENT pythonapi)
