#include "audio_stream.h"

#include <string>

namespace iamaprogrammer {
  int AudioStream::MAX_LOADED_CHUNKS = 10;

  AudioStream::AudioStream() {};

  // AudioStream::AudioStream(IAudioReader* reader, IAudioResampler* resampler, IBasicAudioStream* stream):
  //   reader(reader),
  //   resampler(resampler),
  //   basicAudioStream(stream)
  // {};

  AudioStream::AudioStream(AudioStreamSettings settings) {
    switch (settings.readerType) {
      case ReaderType::SNDLIB:
        //this->reader = std::make_unique<
        break;
    }
  }

  void AudioStream::setup() {
    if (this->streamState == StreamState::OPEN) {
      throw std::runtime_error("Stream is already open.");
    }

    this->streamState = StreamState::OPEN;
    AudioFileDescriptor& audioData = *this->reader->getAudioFileDescriptor();

    this->audioReaderThread = std::thread([this](){ audioReaderThreadCallback(); }); // Start reader thread.

    this->basicAudioStream->openStream();
    this->handleError();
  }

  void AudioStream::start() {
    if (this->basicAudioStream->isStreamStopped()) {
      this->basicAudioStream->startStream();
      this->handleError();

      this->playingState = PlayingState::PLAYING;
    }
  }

  void AudioStream::seek(float seconds) {
    this->playingState = PlayingState::SEEKING;

    while (this->basicAudioStream->getAudioBuffer().size() > 0) { // Clear chunk buffer
      this->basicAudioStream->getAudioBuffer().pop();
    }

    long frames = seconds * this->reader->getAudioFileDescriptor()->sampleRate;
    this->reader->seek(frames, SEEK_CUR);
    this->basicAudioStream->seekStream(frames);

    this->playingState = PlayingState::PLAYING;
  }

  void AudioStream::stop() {
    if (this->basicAudioStream->isStreamActive()) {
      this->basicAudioStream->stopStream();
      this->handleError();
      this->playingState = PlayingState::STOPPED;
    }
  }

  void AudioStream::end() {
    if (this->streamState == StreamState::CLOSED) {
      throw std::runtime_error("Stream is already closed.");
    }
    this->streamState = StreamState::CLOSED;

    this->basicAudioStream->closeStream();
    this->handleError();

    if (this->audioReaderThread.joinable()) {
      this->audioReaderThread.join();
    }
  }

  long AudioStream::position() {
    if (this->isStopped()) {
      return -1;
    }
    return this->basicAudioStream->streamPosition() / this->reader->getAudioFileDescriptor()->sampleRate;
  }

  long AudioStream::duration() {
    return this->basicAudioStream->streamDuration() / this->reader->getAudioFileDescriptor()->sampleRate;
  }

  bool AudioStream::isFinished() {
    return this->basicAudioStream->isStreamFinished();
  }

  bool AudioStream::isActive() {
    return this->basicAudioStream->isStreamActive();
  }

  bool AudioStream::isStopped() {
    return this->basicAudioStream->isStreamStopped();
  }

  // Private
  void AudioStream::handleError() {
    if (this->basicAudioStream->hasError()) {
      throw std::runtime_error("PortAudio error: " + this->basicAudioStream->getError());
    }
  }

  void AudioStream::audioReaderThreadCallback() {
    while (true) {
      if (this->streamState == StreamState::CLOSED) {
        break;
      }

      if (this->basicAudioStream->getAudioBuffer().size() >= this->MAX_LOADED_CHUNKS || this->playingState == PlayingState::SEEKING) {
        continue;
      }

      size_t readSize = this->reader->read(*this->resampler, this->basicAudioStream->getAudioBuffer());
    }
    this->reader->close();
  }
}