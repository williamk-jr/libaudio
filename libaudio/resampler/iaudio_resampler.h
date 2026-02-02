#pragma once

namespace iamaprogrammer {
  class IAudioResampler {
  public:
    virtual void resample(void* buffer) = 0;
    virtual double getSampleRateConversionRatio() = 0;
    virtual void close() = 0;
  };
}