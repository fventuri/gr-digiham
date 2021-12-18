find_package(PkgConfig)

PKG_CHECK_MODULES(PC_DIGIHAM digiham)

FIND_PATH(
    DIGIHAM_INCLUDE_DIRS
    NAMES digiham/api.h
    HINTS $ENV{DIGIHAM_DIR}/include
        ${PC_DIGIHAM_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DIGIHAM_LIBRARIES
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

include("${CMAKE_CURRENT_LIST_DIR}/digihamTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DIGIHAM DEFAULT_MSG DIGIHAM_LIBRARIES DIGIHAM_INCLUDE_DIRS)
MARK_AS_ADVANCED(DIGIHAM_LIBRARIES DIGIHAM_INCLUDE_DIRS)
