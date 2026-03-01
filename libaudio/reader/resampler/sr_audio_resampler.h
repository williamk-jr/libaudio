#pragma once
#include "iaudio_resampler.h"
#include "../decoder/iaudio_decoder.h"
#include <samplerate.h>

namespace iamaprogrammer {
  class SRAudioResampler: public IAudioResampler {
  public:
    SRAudioResampler(double deviceSampleRate);
  
  protected:
    void open(AudioFileDescriptor& fileDescriptor, int readSize, float* readBuffer) override;
    void resample(void* writeBuffer) override;
    double getSampleRateConversionRatio() override;
    void close() override;
  private:
    SRC_STATE* srcState; // Samplerate Converter state
    SRC_DATA srcData; // Samplerate Converter data

    double sampleRateConversionRatio;
    double deviceSampleRate;
    int error = 0;
  };
}
