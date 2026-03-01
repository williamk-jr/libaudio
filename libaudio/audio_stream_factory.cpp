#include "audio_stream_factory.h"

namespace iamaprogrammer {
    AudioStream AudioStreamFactory::fromFile(const IAudioBackend& backend, const std::string& filePath, AudioStreamSettings settings) {
        std::unique_ptr<IAudioDecoder> decoder = AudioStreamFactory::createDecoder(settings);
        std::unique_ptr<IAudioResampler> resampler = AudioStreamFactory::createResampler(settings, backend); 
        std::unique_ptr<AudioReader> reader = std::make_unique<AudioReader>(std::move(decoder), std::move(resampler));
        reader->open(filePath);

        std::unique_ptr<IBasicAudioStream> stream = AudioStreamFactory::createAudioStream(settings, *reader);
        return iamaprogrammer::AudioStream(std::move(reader), std::move(stream));
    }

    AudioStream AudioStreamFactory::fromFile(const IAudioBackend& backend, const std::string& filePath) {
        AudioStreamSettings settings;
        settings.readerSettings.readerType = ReaderType::SNDLIB;
        settings.resamplerSettings.resamplerType = ResamplerType::SR;
        settings.streamSettings.streamType = StreamType::PORTAUDIO;

        return fromFile(backend, filePath, settings);
    }

    std::unique_ptr<IBasicAudioStream> AudioStreamFactory::createAudioStream(AudioStreamSettings settings, AudioReader& reader) {
      switch (settings.streamSettings.streamType) {
          case StreamType::PORTAUDIO:
              return std::make_unique<PortAudioStream>(reader);
          default:
              throw std::runtime_error("Unsupported stream type");
      }
    }

    std::unique_ptr<IAudioDecoder> AudioStreamFactory::createDecoder(AudioStreamSettings settings) {
      switch (settings.readerSettings.readerType) {
          case ReaderType::SNDLIB:
              return std::make_unique<SndlibAudioDecoder>(settings.readerSettings.bufferSize);
          default:
              throw std::runtime_error("Unsupported reader type");
      }
    }

    std::unique_ptr<IAudioResampler> AudioStreamFactory::createResampler(AudioStreamSettings settings, const IAudioBackend& backend) {
      switch (settings.resamplerSettings.resamplerType) {
          case ResamplerType::SR:
              return std::make_unique<SRAudioResampler>(backend.getDefaultAudioDevice().samplerate);
          default:
              throw std::runtime_error("Unsupported resampler type");
      }
    }
}
