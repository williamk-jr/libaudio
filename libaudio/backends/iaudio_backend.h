#pragma once
#include <memory>
#include "../stream/ibasic_audio_stream.h"
#include "../audio_device.h"

namespace iamaprogrammer {
  typedef std::unique_ptr<IBasicAudioStream> AudioStreamPtr;

  class IAudioBackend {
  public:
    virtual void initialize() = 0;
    virtual void terminate() = 0;

    virtual const AudioDevice getDefaultAudioDevice() const = 0;
    virtual const AudioDevice getAudioDevice(int id) const = 0;
  };
}