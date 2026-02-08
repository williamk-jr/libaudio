#include "audio_stream_factory.h"

namespace iamaprogrammer {
    AudioStream AudioStreamFactory::fromFile(const IAudioBackend& backend, const std::string& filePath, AudioStreamSettings settings) {
        std::unique_ptr<IAudioReader> reader;
        std::unique_ptr<IAudioResampler> resampler; 
        std::unique_ptr<IBasicAudioStream> stream;

        switch (settings.readerSettings.readerType) {
            case ReaderType::SNDLIB:
                reader = std::make_unique<SndlibAudioReader>(settings.readerSettings.bufferSize);
                reader->open(filePath);
                break;
            default:
                throw std::runtime_error("Unsupported reader type");
        }

        switch (settings.resamplerSettings.resamplerType) {
            case ResamplerType::SR:
                resampler = std::make_unique<SRAudioResampler>(*reader, backend.getDefaultAudioDevice().samplerate);
                break;
            default:
                throw std::runtime_error("Unsupported resampler type");
        }

        switch (settings.streamSettings.streamType) {
            case StreamType::PORTAUDIO:
                stream = std::make_unique<PortAudioStream>(*reader, *resampler);
                break;
            default:
                throw std::runtime_error("Unsupported stream type");
        }

        return iamaprogrammer::AudioStream(std::move(reader), std::move(resampler), std::move(stream));
    }

    AudioStream AudioStreamFactory::fromFile(const IAudioBackend& backend, const std::string& filePath) {
        AudioStreamSettings settings;
        settings.readerSettings.readerType = ReaderType::SNDLIB;
        settings.resamplerSettings.resamplerType = ResamplerType::SR;
        settings.streamSettings.streamType = StreamType::PORTAUDIO;

        return fromFile(backend, filePath, settings);
    }
}