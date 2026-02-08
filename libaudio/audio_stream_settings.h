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


  struct ReaderSettings {
    ReaderType readerType;
    int bufferSize = 1024;
  };

  struct ResamplerSettings {
    ResamplerType resamplerType;
  };

  struct StreamSettings {
    StreamType streamType;
  };
  

  struct AudioStreamSettings {
    ReaderSettings readerSettings;
    ResamplerSettings resamplerSettings;
    StreamSettings streamSettings;
  };
}