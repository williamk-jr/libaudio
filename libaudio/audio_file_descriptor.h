#pragma once

#include <vector>

namespace iamaprogrammer {
  struct AudioFileDescriptor {
    size_t frames;
    int sampleRate;
    int channels;
  };
}