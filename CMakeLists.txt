cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(libaudio)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # Required by clangd

add_library(libaudio STATIC
	"libaudio/audio_stream.cpp"
	"libaudio/audio_stream.h"
	"libaudio/audio_chunk.cpp"
	"libaudio/audio_chunk.h"
  "libaudio/audio_buffer.cpp"
  "libaudio/audio_buffer.h"
  "libaudio/audio_stream_factory.cpp"
  "libaudio/audio_stream_factory.h"

	"libaudio/stream/port_audio_stream.cpp" 
	"libaudio/stream/port_audio_stream.h" 
	"libaudio/backends/port_audio_backend.cpp" 
	"libaudio/backends/port_audio_backend.h" 
  
	"libaudio/reader/resampler/sr_audio_resampler.cpp" 
	"libaudio/reader/resampler/sr_audio_resampler.h" 
	"libaudio/reader/decoder/sndlib_audio_decoder.cpp"
	"libaudio/reader/decoder/sndlib_audio_decoder.h"
  "libaudio/reader/audio_reader.cpp"
  "libaudio/reader/audio_reader.h"
)

find_package(PortAudio)
find_package(LibSndFile)
find_package(LibSamplerate)

target_include_directories(libaudio
  PUBLIC ${CMAKE_SOURCE_DIR}/src/api/libaudio

  PUBLIC ${PORTAUDIO_INCLUDE_DIR}
  PUBLIC ${LIBSAMPLERATE_INCLUDE_DIR}
  PUBLIC ${LIBSNDFILE_INCLUDE_DIR}
)

target_link_directories(libaudio 
  PUBLIC ${PORTAUDIO_LIBRARIES}
  PUBLIC ${LIBSAMPLERATE_LIBRARIES}
  PUBLIC ${LIBSNDFILE_LIBRARIES}
)

target_link_libraries(libaudio 
  PUBLIC portaudio
  PUBLIC sndfile 
  PUBLIC samplerate
)

if(DEFINED LIBAUDIO_DEBUG_PROGRAM)
  add_executable(test_audio main.cpp)
  target_link_libraries(test_audio libaudio)
  target_include_directories(test_audio PRIVATE ${PROJECT_SOURCE_DIR}/libaudio)
endif()


