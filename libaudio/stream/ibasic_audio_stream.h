#pragma once

#include <string>
#include <atomic>
#include <memory>
#include "../reader/iaudio_reader.h"
#include "../resampler/iaudio_resampler.h"
#include "../audio_buffer.h"

namespace iamaprogrammer {

  struct AudioStreamData {
    const AudioFileDescriptor& data;
    AudioBuffer& buffer;

    std::atomic<bool> seeking = false;
    std::atomic<long> seekOffset = 0;
    std::atomic<long> start = 0;

    std::atomic<bool> streamFinished = true;

    AudioStreamData(const AudioFileDescriptor& audioDesriptor, AudioBuffer& audioBuffer) : data(audioDesriptor), buffer(audioBuffer) {}
  };

  /*
  Holds data important to audio streams.

  - data:       File information related to timing.
  - buffer:     File read buffer typically provided by file reader.
  - seeking:    Whether a "seeking" action should happen. (eg. fast-forward/backward)
  - seekOffest: How much to offset audio stream pointer when seeking.
  - start:      The place in which to start reading audio
  */

  class IBasicAudioStream {
  public:
    enum PlayingState {
      PLAYING,
      STOPPED
    };

    IBasicAudioStream(IAudioReader& reader, IAudioResampler& resampler) {
      this->audioBuffer = std::make_unique<AudioBuffer>(
        reader.getAudioFileDescriptor(), 
        reader.getFrameReadCount() * resampler.getSampleRateConversionRatio()
      );

      this->audioStreamData = std::make_unique<AudioStreamData>(
        this->audioBuffer->getAudioFileDescriptor(),
        *this->audioBuffer
      );
    }

    virtual void openStream() = 0;
    virtual void closeStream() = 0;

    virtual void startStream() = 0;
    virtual void seekStream(int frames) = 0;
    virtual void stopStream() = 0;

    virtual bool isStreamFinished() = 0;
    virtual bool isStreamStopped() = 0;
    virtual bool isStreamActive() = 0;

    virtual long streamPosition() = 0;
    virtual long streamDuration() = 0;

    //virtual double getSampleRate() = 0;
    virtual int getChannelCount() = 0;
    virtual AudioBuffer& getAudioBuffer() = 0;

    virtual std::string getError() = 0;
    virtual bool hasError() = 0;

  protected:
    std::unique_ptr<AudioBuffer> audioBuffer;
    std::unique_ptr<AudioStreamData> audioStreamData;
    //std::unique_ptr<IAudioReader> audioReader;
    //std::unique_ptr<IAudioResampler> audioResampler;

    std::atomic<PlayingState> playingState = PlayingState::STOPPED;
  };
}