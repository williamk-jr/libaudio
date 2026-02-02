if (LIBSAMPLERATE_LIBRARIES AND LIBSAMPLERATE_INCLUDE_DIRS) # Check if lib has already been found.
  set(LIBSAMPLERATE_FOUND TRUE)
else()
  if (NOT WIN32) # If not windows, try using pkg_config.
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
      pkg_check_modules(PC_LIBSAMPLERATE samplerate)
    endif()
  endif()

  if (PC_LIBSAMPLERATE_FOUND)
    set(LIBSAMPLERATE_INCLUDE_DIRS ${PC_LIBSAMPLERATE_INCLUDE_DIRS})
    set(LIBSAMPLERATE_LIBRARIES ${PC_LIBSAMPLERATE_LIBRARIES})
    set(LIBSAMPLERATE_FOUND TRUE)
  else() # If pkg_config fails, manually search standard locations.
    find_path(LIBSAMPLERATE_INCLUDE_DIR
      NAMES
        samplerate.h
      PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
        /sw/include
    )

    find_library(LIBSAMPLERATE_LIBRARY
      NAMES
        samplerate
      PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
    )

    find_path(LIBSAMPLERATE_LIBRARY_DIR
      NAMES
        samplerate
      PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
    )

    set(LIBSAMPLERATE_INCLUDE_DIRS ${LIBSAMPLERATE_INCLUDE_DIR})
    set(LIBSAMPLERATE_LIBRARIES ${LIBSAMPLERATE_LIBRARY})
    set(LIBSAMPLERATE_LIBRARY_DIRS ${LIBSAMPLERATE_LIBRARY_DIR})

    if (LIBSAMPLERATE_INCLUDE_DIRS AND LIBSAMPLERATE_LIBRARIES)
       set(LIBSAMPLERATE_FOUND TRUE)
    endif()

    if (LIBSAMPLERATE_FOUND) # Logging
      #if (NOT Portaudio_FIND_QUIETLY)
        message(STATUS "Found LibSamplerate: ${LIBSAMPLERATE_LIBRARIES}")
      #endif()
    else()
      #if (Portaudio_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find LibSamplerate")
      #endif()
    endif()
  endif()

  # show the LIBSAMPLERATE_INCLUDE_DIRS and LIBSAMPLERATE_LIBRARIES variables only in the advanced view
  mark_as_advanced(LIBSAMPLERATE_INCLUDE_DIRS LIBSAMPLERATE_LIBRARIES)
endif()