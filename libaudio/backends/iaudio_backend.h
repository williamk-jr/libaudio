#pragma once
#include <memory>
#include "../stream/ibasic_audio_stream.h"
#include "../audio_device.h"

namespace iamaprogrammer {
  typedef std::unique_ptr<IBasicAudioStream> AudioStreamPtr;

  class IAudioBackend {
  public:
    /*
    @brief Intializes audio backend. This should be called before the usage of this library.
    */
    virtual void initialize() = 0;

    /*
    @brief Terminates audio backend.
    */
    virtual void terminate() = 0;

    /*
    @brief Gets the default audio device.
    */
    virtual const AudioDevice getDefaultAudioDevice() const = 0;

    /*
    @brief Gets the audio device via the index. Index representation can vary between backends.
    */
    virtual const AudioDevice getAudioDevice(int id) const = 0;
  };
}