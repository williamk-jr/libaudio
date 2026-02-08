#include "port_audio_backend.h"

namespace iamaprogrammer {
  PortAudioBackend::PortAudioBackend() : error(paNoError) {
    // Constructor initializes the backend with no error
  }

  void PortAudioBackend::initialize() {
    this->error = Pa_Initialize();
    if (this->error != paNoError) {
      throw std::runtime_error("Failed to initialize PortAudio: " + std::string(Pa_GetErrorText(this->error)));
    }
  }

  void PortAudioBackend::terminate() {
    Pa_Terminate();
  }

  const AudioDevice PortAudioBackend::getDefaultAudioDevice() const {
    return this->getAudioDevice(Pa_GetDefaultOutputDevice());
  }

  const AudioDevice PortAudioBackend::getAudioDevice(int device) const {
    if (device == paNoDevice) {
      throw std::runtime_error("No default output device found.");
    }

    const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(device);
    return { deviceInfo->name, deviceInfo->defaultSampleRate };
  }
}