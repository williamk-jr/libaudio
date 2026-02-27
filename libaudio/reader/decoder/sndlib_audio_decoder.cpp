#include "sndlib_audio_decoder.h"

namespace iamaprogrammer {
  SndlibAudioDecoder::SndlibAudioDecoder() {}

  SndlibAudioDecoder::SndlibAudioDecoder(int frameReadCount): 
    readSize(readSize), frameReadCount(frameReadCount) {}

  void SndlibAudioDecoder::open(std::filesystem::path filePath) {
    std::cout << "READER" << std::endl;

    SF_INFO info;

    // Format file path
    std::string pathStr = filePath.string();
    const char* pathCStr = pathStr.c_str();

    // Open file
    std::cout << "\tAttempting to open file: " << filePath << std::endl;
    this->file = sf_open(pathCStr, SFM_READ, &info);
    if (this->file == nullptr) {
      std::cout << "\tError opening file: " << pathCStr << std::endl;
      std::cout << "\t" << sf_strerror(this->file) << std::endl;
      return;
    }

    this->audioFileDescriptor.frames = info.frames;
    this->audioFileDescriptor.channels = info.channels;
    this->audioFileDescriptor.sampleRate = info.samplerate;

    std::cout << "\tOpened file: " << filePath << std::endl;
    std::cout << "\t\tFrame Count: " << info.frames << std::endl;
    std::cout << "\t\tChannels: " << info.channels << std::endl;
    std::cout << "\t\tSample Rate: " << info.samplerate << std::endl;
  }

  size_t SndlibAudioDecoder::read(float* buffer) {
    return sf_readf_float(this->file, buffer, this->frameReadCount);
  }

  void SndlibAudioDecoder::seek(size_t frames, int whence) {
    sf_seek(this->file, frames, whence);
  }

  AudioFileDescriptor& SndlibAudioDecoder::getAudioFileDescriptor() {
    return this->audioFileDescriptor;
  }

  int SndlibAudioDecoder::getFrameReadCount() {
    return this->frameReadCount;
  }

  void SndlibAudioDecoder::close() {
    sf_close(file);
  }
}