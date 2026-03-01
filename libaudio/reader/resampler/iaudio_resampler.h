#pragma once
#include "../../audio_file_descriptor.h"

namespace iamaprogrammer {
  class AudioReader;

  class IAudioResampler {
  friend class AudioReader;
  
  public:
    virtual ~IAudioResampler() = default;

    virtual void open(AudioFileDescriptor& fileDescriptor, int readSize, float* readBuffer) = 0;
    virtual void resample(void* buffer) = 0;
    virtual double getSampleRateConversionRatio() = 0;
    virtual void close() = 0;
  };
}
