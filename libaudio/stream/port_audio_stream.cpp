#include "port_audio_stream.h"

namespace iamaprogrammer {

  PortAudioStream::PortAudioStream(AudioReader& reader) : IBasicAudioStream(reader) {}

  void PortAudioStream::openStream() {
    std::cout << "AUDIO STREAM" << std::endl;
    
    // this->audioStreamData.data = this->audioBuffer->getAudioFileDescriptor();
    // this->audioStreamData.buffer = *this->audioBuffer;

    std::cout << "\tGetting Output Device." << std::endl;

    PaDeviceIndex devicesCount = Pa_GetDeviceCount();
    std::cout << "\tDevice Count: " << devicesCount << std::endl;
    for (PaDeviceIndex device = 0; device < devicesCount; device++) {
      const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(device);

      std::cout << "\tDevice " << device << ": " << deviceInfo->name << std::endl;
      std::cout << "\t\tDefault Samplerate: " << deviceInfo->defaultSampleRate << std::endl;
      std::cout << "\t\tDefault Samplerate: " << deviceInfo->defaultSampleRate << std::endl;
    }

    PaDeviceIndex device = Pa_GetDefaultOutputDevice();
    const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(device);
    
    PaStreamParameters outputParameters;
    outputParameters.device = device;
    outputParameters.channelCount = this->audioBuffer->getAudioFileDescriptor().channels;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = deviceInfo->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    std::cout << "\tStream Parameters: " << std::endl;
    std::cout << "\t\tDevice Index: " << outputParameters.device << std::endl;
    std::cout << "\t\tChannel Count: " << outputParameters.channelCount << std::endl;
    std::cout << "\t\tSample Format: " << outputParameters.sampleFormat << std::endl;
    std::cout << "\t\tLatency: " << outputParameters.suggestedLatency << std::endl;

    // reader->getReadSize() * resampler->getSampleRateConversionRatio()
    this->error = Pa_OpenStream(
      &this->stream,
      NULL,
      &outputParameters,
      deviceInfo->defaultSampleRate,
      this->audioBuffer->getFrameReadCount(), // paFramesPerBufferUnspecified
      paNoFlag,
      paCallback,
      &this->audioStreamData
    );

    Pa_SetStreamFinishedCallback(this->stream, paStreamFinishedCallback);
    std::cout << "\tCreated Audio Stream." << std::endl;
  }

  void PortAudioStream::closeStream() {
    this->error = Pa_CloseStream(this->stream);
  }

  void PortAudioStream::startStream() {
    this->audioStreamData->streamFinished = false;
    this->error = Pa_StartStream(this->stream);
  }

  void PortAudioStream::seekStream(int frames) {
    this->audioStreamData->seekOffset = frames;
    this->audioStreamData->seeking = true;
  }

  void PortAudioStream::stopStream() {
    this->error = Pa_StopStream(this->stream);
  }

  bool PortAudioStream::isStreamFinished() {
    return this->audioStreamData->streamFinished;
  }

  bool PortAudioStream::isStreamStopped() {
    return Pa_IsStreamStopped(this->stream);
  }

  bool PortAudioStream::isStreamActive() {
    return Pa_IsStreamActive(this->stream);
  }

  long PortAudioStream::streamPosition() {
    return this->audioStreamData->start / this->getChannelCount();
  }

  long PortAudioStream::streamDuration() {
    return this->audioStreamData->data.frames;
  }

  int PortAudioStream::getChannelCount() {
    return this->audioStreamData->data.channels;
  }

  AudioBuffer& PortAudioStream::getAudioBuffer() {
    return *this->audioBuffer;
  }

  std::string PortAudioStream::getError() {
    return std::string(Pa_GetErrorText(this->error));
  }

  bool PortAudioStream::hasError() {
    return this->error != paNoError;
  }

  int PortAudioStream::getDeviceIndex() {
    PaDeviceIndex device = Pa_GetDefaultOutputDevice();
    if (device == paNoDevice) {
      throw std::runtime_error("No default output device found.");
    }
    return device;
  }

  const PaDeviceInfo* PortAudioStream::getDeviceInfo() {
    return Pa_GetDeviceInfo(getDeviceIndex());
  }
}