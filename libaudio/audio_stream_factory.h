#pragma once
#include "audio_stream.h"
#include "audio_stream_settings.h"

#include "reader/audio_reader.h"

#include "reader/decoder/sndlib_audio_decoder.h"

#include "reader/resampler/sr_audio_resampler.h"

#include "stream/port_audio_stream.h"

#include "backends/iaudio_backend.h"

namespace iamaprogrammer {
  

  class AudioStreamFactory {
    public:
      static AudioStream fromFile(const IAudioBackend& backend, const std::string& filePath);
      static AudioStream fromFile(const IAudioBackend& backend, const std::string& filePath, AudioStreamSettings settings);

    private:
      static std::unique_ptr<IAudioDecoder> createDecoder(AudioStreamSettings settings);
      static std::unique_ptr<IAudioResampler> createResampler(AudioStreamSettings settings, const IAudioBackend& backend);
      static std::unique_ptr<IBasicAudioStream> createAudioStream(AudioStreamSettings settings, AudioReader& reader);
  };
}
