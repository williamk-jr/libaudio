#include "audio_buffer.h"

namespace iamaprogrammer {
  AudioBuffer::AudioBuffer(AudioFileDescriptor* audioFileDescriptor, int framesReadCount):
    audioFileDescriptor(audioFileDescriptor),
    framesReadCount(framesReadCount)
  {
    std::cout << "BUFFER" << std::endl;
    std::cout << "\tCreated Shared Buffer: " << std::endl;
    std::cout << "\t\tFrames Per Buffer: " << this->framesReadCount << std::endl;
  }

  void AudioBuffer::push(AudioChunk& chunk) {
    if (chunk.getSize() != this->framesReadCount * this->audioFileDescriptor->channels) {
      throw std::runtime_error("Mismatch between chunk size and frames per buffer. " + std::to_string(chunk.getSize()) + " vs " + std::to_string(this->framesReadCount * this->audioFileDescriptor->channels));
    }
    this->buffer.push(chunk);
  }

  AudioChunk& AudioBuffer::front() {
    return this->buffer.front();
  }

  void AudioBuffer::pop() {
    this->buffer.pop();
  }

  size_t AudioBuffer::size() {
    return this->buffer.size();
  }

  const AudioFileDescriptor& AudioBuffer::getAudioFileDescriptor() {
    return *this->audioFileDescriptor;
  }

  const int AudioBuffer::getFrameReadCount() {
    return this->framesReadCount;
  }
}