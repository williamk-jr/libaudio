#include "audio_reader.h"

namespace iamaprogrammer {

    AudioReader::AudioReader(std::unique_ptr<IAudioDecoder> decoder, std::unique_ptr<IAudioResampler> resampler) {
        this->decoder = std::move(decoder);
        this->resampler = std::move(resampler);
    }

    void AudioReader::open(std::filesystem::path filePath) {
        this->decoder->open(filePath);
        this->resampler->open(
            this->decoder->getAudioFileDescriptor(), 
            this->decoder->getFrameReadCount(), 
            this->readBuffer.data()
        );

        this->readBuffer = std::vector<float>(
            this->decoder->getFrameReadCount() * this->decoder->getAudioFileDescriptor().channels
        );
    }

    size_t AudioReader::read(AudioBuffer& buffer) {
        double srConvertionRatio = this->resampler->getSampleRateConversionRatio();
        long chunkSize = (this->decoder->getFrameReadCount() * srConvertionRatio) 
                        * this->decoder->getAudioFileDescriptor().channels;

        AudioChunk chunk(chunkSize);
        size_t readCount = this->decoder->read(this->readBuffer.data());

        // Transfer data from read buffer to chunk buffer.
        if (srConvertionRatio == 1.0) { // The ratio is the same, no need to convert.
            *(chunk.data()) = this->readBuffer;
        } else {
            this->resampler->resample(chunk.data()->data());
        }

        if (readCount > 0) {
            buffer.push(chunk);
        }
        return readCount;
    }

    void AudioReader::seek(size_t frames, int whence) {
        this->decoder->seek(frames, whence);
    }

    void* AudioReader::getReadBuffer() {
        return this->readBuffer.data();
    }

    AudioFileDescriptor& AudioReader::getAudioFileDescriptor() {
        return this->decoder->getAudioFileDescriptor();
    }

    int AudioReader::getFrameReadCount() {
        return this->decoder->getFrameReadCount();
    }

    double AudioReader::getSampleRateConversionRatio() {
        return this->resampler->getSampleRateConversionRatio();
    }

    void AudioReader::close() {
        this->resampler->close();
        this->decoder->close();
    }
}