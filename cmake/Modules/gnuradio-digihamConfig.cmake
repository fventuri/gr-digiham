find_package(PkgConfig)

PKG_CHECK_MODULES(PC_GR_DIGIHAM gnuradio-digiham)

FIND_PATH(
    GR_DIGIHAM_INCLUDE_DIRS
    NAMES gnuradio/digiham/api.h
    HINTS $ENV{DIGIHAM_DIR}/include
        ${PC_DIGIHAM_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GR_DIGIHAM_LIBRARIES
    NAMES gnuradio-digiham
    HINTS $ENV{DIGIHAM_DIR}/lib
        ${PC_DIGIHAM_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/gnuradio-digihamTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GR_DIGIHAM DEFAULT_MSG GR_DIGIHAM_LIBRARIES GR_DIGIHAM_INCLUDE_DIRS)
MARK_AS_ADVANCED(GR_DIGIHAM_LIBRARIES GR_DIGIHAM_INCLUDE_DIRS)
