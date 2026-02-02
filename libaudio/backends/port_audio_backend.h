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

    AudioDevice getDefaultAudioDevice() override;
    AudioDevice getAudioDevice(int id) override;
  private:
    PaError error;
  };
}