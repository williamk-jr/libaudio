#pragma once

namespace iamaprogrammer {
  enum class ReaderType {
    SNDLIB
  };

  enum class ResamplerType {
    SR
  };

  enum class StreamType {
    PORTAUDIO
  };
  

  struct AudioStreamSettings {
    ReaderType readerType;
    ResamplerType resamplerType;
    StreamType streamType;
  };
}