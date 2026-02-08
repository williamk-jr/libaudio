#pragma once
#include "ibasic_audio_stream.h"  

#include <queue>
#include <stdexcept>
#include <portaudio.h>
#include "../audio_chunk.h"
#include "../audio_file_descriptor.h"

namespace iamaprogrammer {
  class PortAudioStream : public IBasicAudioStream {
  public:
    PortAudioStream(IAudioReader& reader, IAudioResampler& resampler);

    void openStream() override;
    void closeStream() override;

    void startStream() override;
    void seekStream(int frames) override;
    void stopStream() override;

    bool isStreamFinished() override;
    bool isStreamStopped() override;
    bool isStreamActive() override;

    long streamPosition() override;
    long streamDuration() override;

    //double getSampleRate() override;
    int getChannelCount() override;
    AudioBuffer& getAudioBuffer() override;

    std::string getError() override;
    bool hasError() override;
  private:

    PaStream* stream;
    PaError error;

    static int getDeviceIndex();
    static const PaDeviceInfo* getDeviceInfo();

    static int paCallback(
      const void* inputBuffer, void* outputBuffer,
      unsigned long framesPerBuffer,
      const PaStreamCallbackTimeInfo* timeInfo,
      PaStreamCallbackFlags statusFlags,
      void* userData
    ) {
      float* out = static_cast<float*>(outputBuffer);
      std::unique_ptr<AudioStreamData>& streamData = *static_cast<std::unique_ptr<AudioStreamData>*>(userData);

      // Audio reader implementations split signals into chunks. Get the most recent chunk from the reader.
      if (streamData->buffer.isEmpty()) {
        return paComplete;
      }

      AudioChunk& chunk = streamData->buffer.front();

      // Offest current start position when seeking.
      if (streamData->seeking) {
        streamData->start += streamData->seekOffset * streamData->data.channels;
        streamData->seeking = false;
      }

      long pos = 0;
      for (unsigned long i = 0; i < framesPerBuffer; i++) {
        for (int channelOffset = 0; channelOffset < streamData->data.channels; channelOffset++) {
          *out++ = chunk.data()->at(pos + channelOffset);
        }

        pos += streamData->data.channels;
        streamData->start += streamData->data.channels;
      }

      streamData->buffer.pop();

      if (streamData->start >= streamData->data.frames * streamData->data.channels) {
        return paComplete;
      }

      return paContinue;
    }

    static void paStreamFinishedCallback(void* userData) {
      std::unique_ptr<AudioStreamData>& streamData = *static_cast<std::unique_ptr<AudioStreamData>*>(userData);
      streamData->streamFinished = true;
    }
  };
}