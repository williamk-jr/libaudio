#pragma once
#include "audio_stream.h"
#include "audio_stream_settings.h"

#include "reader/sndlib_audio_reader.h"

#include "resampler/sr_audio_resampler.h"

#include "stream/port_audio_stream.h"

#include "backends/iaudio_backend.h"

namespace iamaprogrammer {
  

  class AudioStreamFactory {
    public:
      static AudioStream fromFile(const IAudioBackend& backend, const std::string& filePath);
      static AudioStream fromFile(const IAudioBackend& backend, const std::string& filePath, AudioStreamSettings settings);
  };
}