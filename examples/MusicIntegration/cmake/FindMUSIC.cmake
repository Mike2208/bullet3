# FindMUSIC.cmake
#
# Finds the MUSIC library
#
# This will define the following variables
#
#    MUSIC_FOUND
#    MUSIC_INCLUDE_DIRS
#    MUSIC_LIBRARY_DIRS
#
# and the following imported targets
#
#     MUSIC::MUSIC
#
# Adapted from https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/

# Find Music header files
find_path(MUSIC_INCLUDE_DIR
    NAMES music-c.h
    HINTS ${CMAKE_INSTALL_INCLUDEDIR} ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include
)

# Get version information from version.hh
file(STRINGS "${MUSIC_INCLUDE_DIR}/music/version.hh" MUSIC_VERSION REGEX "[0-9]+\.[0-9]+\.[0-9]+")
string(REGEX MATCH "[0-9]+\.[0-9]+\.[0-9]+" MUSIC_VERSION ${MUSIC_VERSION})

# Find Music library directories
find_path(MUSIC_LIBRARY_DIR
    NAMES libmusic.so "libmusic.so.${MUSIC_VERSION}"
    HINTS ${CMAKE_INSTALL_LIBDIR} ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES lib
)

# Find OpenMPI
find_package(PkgConfig)
pkg_check_modules(PC_OpenMPI QUIET IMPORTED_TARGET ompi-cxx)

# Handle standard args
mark_as_advanced(MUSIC_FOUND
    PC_OpenMPI_FOUND MUSIC_INCLUDE_DIR MUSIC_LIBRARY_DIR MUSIC_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MUSIC
    REQUIRED_VARS MUSIC_INCLUDE_DIR MUSIC_LIBRARY_DIR PC_OpenMPI_FOUND
    VERSION_VAR MUSIC_VERSION
)

if(MUSIC_FOUND)
    set(MUSIC_INCLUDE_DIRS ${MUSIC_INCLUDE_DIR})
    set(MUSIC_LIBRARY_DIRS ${MUSIC_LIBRARY_DIR})

    add_library(MUSIC::MUSIC INTERFACE IMPORTED)
    target_include_directories(MUSIC::MUSIC INTERFACE
        ${MUSIC_INCLUDE_DIR})
    target_link_directories(MUSIC::MUSIC INTERFACE
        ${MUSIC_LIBRARY_DIR})
    target_link_libraries(MUSIC::MUSIC INTERFACE
        music
        PkgConfig::PC_OpenMPI)
endif()
