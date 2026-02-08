#include "audio_stream.h"

#include <string>

namespace iamaprogrammer {
  int AudioStream::MAX_LOADED_CHUNKS = 20;

  AudioStream::AudioStream() {};

  AudioStream::AudioStream(std::unique_ptr<IAudioReader> reader, std::unique_ptr<IAudioResampler> resampler, std::unique_ptr<IBasicAudioStream> stream) :
    reader(std::move(reader)),
    resampler(std::move(resampler)),
    basicAudioStream(std::move(stream))
  {};
  
  void AudioStream::setup() {
    if (this->streamState == StreamState::OPEN) {
      throw std::runtime_error("Stream is already open.");
    }

    this->streamState = StreamState::OPEN;
    AudioFileDescriptor& audioData = this->reader->getAudioFileDescriptor();

    this->audioReaderThread = std::thread([this](){ audioReaderThreadCallback(); }); // Start reader thread.

    this->basicAudioStream->openStream();
    this->handleError();
  }

  void AudioStream::start() {
    if (this->basicAudioStream->isStreamStopped()) {
      std::cout << "Starting stream." << std::endl;
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

    long frames = seconds * this->reader->getAudioFileDescriptor().sampleRate;
    this->reader->seek(frames, SEEK_CUR);
    this->basicAudioStream->seekStream(frames);

    this->playingState = PlayingState::PLAYING;
  }

  void AudioStream::stop() {
    //std::cout << "Ending stream." << std::endl;
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

    //if (this->audioReaderThread.joinable()) {
      std::cout << "Joining audio reader thread." << std::endl;
      this->audioReaderThread.join();
      std::cout << "Audio reader thread joined." << std::endl;
    //}

    this->basicAudioStream->closeStream();
    this->handleError();
  }

  long AudioStream::position() {
    if (this->isStopped()) {
      return -1;
    }
    return this->basicAudioStream->streamPosition() / this->reader->getAudioFileDescriptor().sampleRate;
  }

  long AudioStream::duration() {
    return this->basicAudioStream->streamDuration() / this->reader->getAudioFileDescriptor().sampleRate;
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
    while (this->streamState != StreamState::CLOSED) {

      if (this->basicAudioStream->getAudioBuffer().size() >= this->MAX_LOADED_CHUNKS || this->playingState == PlayingState::SEEKING) {
        continue;
      }

      size_t readSize = this->reader->read(*this->resampler, this->basicAudioStream->getAudioBuffer());
    }
    this->reader->close();
  }
}