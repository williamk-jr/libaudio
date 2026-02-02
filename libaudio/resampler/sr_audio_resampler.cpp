#include "sr_audio_resampler.h"

namespace iamaprogrammer {
  SRAudioResampler::SRAudioResampler(IAudioReader* reader, double deviceSampleRate) {
    std::cout << "RESAMPLER" << std::endl;

    AudioFileDescriptor* fileDescriptor = reader->getAudioFileDescriptor();
    int readSize = reader->getFrameReadCount();
    this->sampleRateConversionRatio = deviceSampleRate / fileDescriptor->sampleRate;

    std::cout << "\tAttempting to create resampler." << std::endl;
    this->srcState = src_new(SRC_SINC_FASTEST, fileDescriptor->channels, &this->error);
    this->srcData.data_in = (float*)reader->getReadBuffer();
    this->srcData.input_frames = readSize;
    this->srcData.output_frames = readSize * this->sampleRateConversionRatio;
    this->srcData.src_ratio = this->sampleRateConversionRatio;
    this->srcData.end_of_input = 0;

    std::cout << "\tResampler created: " << std::endl;
    std::cout << "\t\tRead Buffer Address: " << this->srcData.data_in << std::endl;
    std::cout << "\t\tInput Frame Count: " << this->srcData.input_frames << std::endl;
    std::cout << "\t\tOutput Frame Count: " << this->srcData.output_frames << std::endl;
    std::cout << "\t\tConversion Ratio: " << this->srcData.src_ratio << std::endl;
  }

  void SRAudioResampler::resample(void* buffer) {
    this->srcData.data_out = (float*)buffer;
    src_process(this->srcState, &this->srcData);
  }

  void SRAudioResampler::close() {
    src_delete(this->srcState);
  }

  double SRAudioResampler::getSampleRateConversionRatio() {
    return this->sampleRateConversionRatio;
  }
}