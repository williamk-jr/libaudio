# - Try to find Portaudio
# Once done this will define
#
#  PORTAUDIO_FOUND - system has Portaudio
#  PORTAUDIO_INCLUDE_DIRS - the Portaudio include directory
#  PORTAUDIO_LIBRARIES - Link these to use Portaudio
#  PORTAUDIO_DEFINITIONS - Compiler switches required for using Portaudio
#  PORTAUDIO_VERSION - Portaudio version

if (PORTAUDIO_LIBRARIES AND PORTAUDIO_INCLUDE_DIRS) # Check if lib has already been found.
  set(PORTAUDIO_FOUND TRUE)
else()
  if (NOT WIN32) # If not windows, try using pkg_config.
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
      pkg_check_modules(PC_PORTAUDIO portaudio-2.0)
    endif()
  endif()

  if (PC_PORTAUDIO_FOUND)
    set(PORTAUDIO_INCLUDE_DIRS ${PC_PORTAUDIO_INCLUDE_DIRS})
    set(PORTAUDIO_LIBRARIES ${PC_PORTAUDIO_LIBRARIES})
    set(PORTAUDIO_FOUND TRUE)
  else() # If pkg_config fails, manually search standard locations.
    find_path(PORTAUDIO_INCLUDE_DIR
      NAMES
        portaudio.h
      PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
        /sw/include
    )

    find_library(PORTAUDIO_LIBRARY
      NAMES
        portaudio
      PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
    )

    find_path(PORTAUDIO_LIBRARY_DIR
      NAMES
        portaudio
      PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
    )

    set(PORTAUDIO_INCLUDE_DIRS ${PORTAUDIO_INCLUDE_DIR})
    set(PORTAUDIO_LIBRARIES ${PORTAUDIO_LIBRARY})
    set(PORTAUDIO_LIBRARY_DIRS ${PORTAUDIO_LIBRARY_DIR})

    if (PORTAUDIO_INCLUDE_DIRS AND PORTAUDIO_LIBRARIES)
       set(PORTAUDIO_FOUND TRUE)
    endif()

    if (PORTAUDIO_FOUND) # Logging
      if (NOT Portaudio_FIND_QUIETLY)
        message(STATUS "Found Portaudio: ${PORTAUDIO_LIBRARIES}")
      endif()
    else()
      if (Portaudio_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find Portaudio")
      endif()
    endif()
  endif()

  # show the PORTAUDIO_INCLUDE_DIRS and PORTAUDIO_LIBRARIES variables only in the advanced view
  mark_as_advanced(PORTAUDIO_INCLUDE_DIRS PORTAUDIO_LIBRARIES)
endif()