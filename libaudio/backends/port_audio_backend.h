#pragma once
#include "iaudio_backend.h"
#include "../stream/port_audio_stream.h"
#include "../audio_device.h"
#include <portaudio.h>
#include <memory>

namespace iamaprogrammer {
  class PortAudioBackend : public IAudioBackend {
  public:
    PortAudioBackend();

    void initialize() override;
    void terminate() override;

    const AudioDevice getDefaultAudioDevice() const override;
    const AudioDevice getAudioDevice(int id) const override;
  private:
    PaError error;
  };
}