if (LIBSNDFILE_LIBRARIES AND LIBSNDFILE_INCLUDE_DIRS) # Check if lib has already been found.
  set(LIBSNDFILE_FOUND TRUE)
else()
  if (NOT WIN32) # If not windows, try using pkg_config.
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
      pkg_check_modules(PC_LIBSNDFILE sndfile)
    endif()
  endif()

  if (PC_LIBSNDFILE_FOUND)
    set(LIBSNDFILE_INCLUDE_DIRS ${PC_LIBSNDFILE_INCLUDE_DIRS})
    set(LIBSNDFILE_LIBRARIES ${PC_LIBSNDFILE_LIBRARIES})
    set(LIBSNDFILE_FOUND TRUE)
  else() # If pkg_config fails, manually search standard locations.
    find_path(LIBSNDFILE_INCLUDE_DIR
      NAMES
        sndfile.h
      PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
        /sw/include
    )

    find_library(LIBSNDFILE_LIBRARY
      NAMES
        sndfile
      PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
    )

    find_path(LIBSNDFILE_LIBRARY_DIR
      NAMES
        sndfile
      PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
    )

    set(LIBSNDFILE_INCLUDE_DIRS ${LIBSNDFILE_INCLUDE_DIR})
    set(LIBSNDFILE_LIBRARIES ${LIBSNDFILE_LIBRARY})
    set(LIBSNDFILE_LIBRARY_DIRS ${LIBSNDFILE_LIBRARY_DIR})

    if (LIBSNDFILE_INCLUDE_DIRS AND LIBSNDFILE_LIBRARIES)
       set(LIBSNDFILE_FOUND TRUE)
    endif()

    if (LIBSNDFILE_FOUND) # Logging
      #if (NOT Portaudio_FIND_QUIETLY)
        message(STATUS "Found LibSndFile: ${LIBSNDFILE_LIBRARIES}")
      #endif()
    else()
      #if (Portaudio_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find LibSndFile")
      #endif()
    endif()
  endif()

  # show the LIBSNDFILE_INCLUDE_DIRS and LIBSNDFILE_LIBRARIES variables only in the advanced view
  mark_as_advanced(LIBSNDFILE_INCLUDE_DIRS LIBSNDFILE_LIBRARIES)
endif()