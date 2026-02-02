#pragma once
#include <vector>
#include <queue>

namespace iamaprogrammer {
  class AudioChunk {
  public:
    AudioChunk();
    AudioChunk(long size);

    std::vector<float>* data();
    long getSize();

  private:
    long size = 0;
    std::vector<float> buffer;
  };

  //typedef std::queue<AudioChunk> AudioBuffer;
};