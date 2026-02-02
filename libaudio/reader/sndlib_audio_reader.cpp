#include "sndlib_audio_reader.h"

namespace iamaprogrammer {
  SndlibAudioReader::SndlibAudioReader() {

  }

  SndlibAudioReader::SndlibAudioReader(int frameReadCount): 
    readSize(readSize), frameReadCount(frameReadCount) {}

  void SndlibAudioReader::open(std::filesystem::path filePath) {
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

    // Set up read buffer and initialize sample rate converter.
    this->readBuffer = std::vector<float>(this->frameReadCount * info.channels);
  }

  size_t SndlibAudioReader::read(IAudioResampler& resampler, AudioBuffer& buffer) {
    int srConvertionRatio = resampler.getSampleRateConversionRatio();
    AudioChunk chunk((this->frameReadCount * srConvertionRatio) * this->audioFileDescriptor.channels);

    long long readCount = sf_readf_float(this->file, this->readBuffer.data(), this->frameReadCount);

    // Transfer data from read buffer to chunk buffer.
    if (srConvertionRatio == 1.0) { // The ratio is the same, no need to convert.
      *(chunk.data()) = this->readBuffer;
    } else {
      resampler.resample(chunk.data()->data());
    }

    if (readCount > 0) {
      buffer.push(chunk);
    }
    return readCount;
  }

  void SndlibAudioReader::seek(size_t frames, int whence) {
    sf_seek(this->file, frames, whence);
  }

  void* SndlibAudioReader::getReadBuffer() {
    return this->readBuffer.data();
  }

  AudioFileDescriptor* SndlibAudioReader::getAudioFileDescriptor() {
    return &this->audioFileDescriptor;
  }

  int SndlibAudioReader::getFrameReadCount() {
    return this->frameReadCount;
  }

  void SndlibAudioReader::close() {
    sf_close(file);
  }
}