#pragma once
#include "iaudio_reader.h"
#include <sndfile.h>
#include <queue>
#include <filesystem>
#include <iostream>



namespace iamaprogrammer {
  class SndlibAudioReader: public IAudioReader {
  public:
    SndlibAudioReader();
    SndlibAudioReader(int readSize);

    void open(std::filesystem::path filePath) override;
    size_t read(IAudioResampler& resampler, AudioBuffer& buffer) override;
    void seek(size_t frames, int whence) override;

    void* getReadBuffer() override;
    AudioFileDescriptor* getAudioFileDescriptor() override;
    int getFrameReadCount() override;

    void close() override;
  private:
    std::filesystem::path path;
    SNDFILE* file;

    AudioFileDescriptor audioFileDescriptor;
    std::vector<float> readBuffer;

    int frameReadCount = 0;
    int readSize = 0;
  };
}