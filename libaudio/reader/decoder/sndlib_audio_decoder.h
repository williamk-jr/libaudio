#pragma once
#include "iaudio_decoder.h"
#include <sndfile.h>
#include <queue>
#include <filesystem>
#include <iostream>



namespace iamaprogrammer {
  class SndlibAudioDecoder: public IAudioDecoder {
  public:
    SndlibAudioDecoder();
    SndlibAudioDecoder(int readSize);

    void open(std::filesystem::path filePath) override;
    size_t read(float* buffer) override;
    void seek(size_t frames, int whence) override;

    AudioFileDescriptor& getAudioFileDescriptor() override;
    int getFrameReadCount() override;

    void close() override;
  private:
    std::filesystem::path path;
    SNDFILE* file;

    AudioFileDescriptor audioFileDescriptor;

    int frameReadCount = 0;
    int readSize = 0;
  };
}