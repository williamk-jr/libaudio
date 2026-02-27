#include "audio_stream_factory.h"

namespace iamaprogrammer {
    AudioStream AudioStreamFactory::fromFile(const IAudioBackend& backend, const std::string& filePath, AudioStreamSettings settings) {
        std::unique_ptr<IAudioDecoder> decoder;
        std::unique_ptr<IAudioResampler> resampler; 

        std::unique_ptr<AudioReader> reader;
        std::unique_ptr<IBasicAudioStream> stream;

        switch (settings.readerSettings.readerType) {
            case ReaderType::SNDLIB:
                decoder = std::make_unique<SndlibAudioDecoder>(settings.readerSettings.bufferSize);
                break;
            default:
                throw std::runtime_error("Unsupported reader type");
        }

        switch (settings.resamplerSettings.resamplerType) {
            case ResamplerType::SR:
                resampler = std::make_unique<SRAudioResampler>(backend.getDefaultAudioDevice().samplerate);
                break;
            default:
                throw std::runtime_error("Unsupported resampler type");
        }

        reader = std::make_unique<AudioReader>(std::move(decoder), std::move(resampler));
        reader->open(filePath);

        std::cout << "Create audio stream" << std::endl;
        switch (settings.streamSettings.streamType) {
            case StreamType::PORTAUDIO:
                stream = std::make_unique<PortAudioStream>(*reader);
                break;
            default:
                throw std::runtime_error("Unsupported stream type");
        }

        return iamaprogrammer::AudioStream(std::move(reader), std::move(stream));
    }

    AudioStream AudioStreamFactory::fromFile(const IAudioBackend& backend, const std::string& filePath) {
        AudioStreamSettings settings;
        settings.readerSettings.readerType = ReaderType::SNDLIB;
        settings.resamplerSettings.resamplerType = ResamplerType::SR;
        settings.streamSettings.streamType = StreamType::PORTAUDIO;

        return fromFile(backend, filePath, settings);
    }
}