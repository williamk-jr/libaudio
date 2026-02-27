#pragma once

#include <iostream>
#include <filesystem>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>

#include "audio_chunk.h"
#include "audio_stream_settings.h"
#include "audio_file_descriptor.h"
#include "stream/ibasic_audio_stream.h"
#include "reader/audio_reader.h"

namespace iamaprogrammer {
  class AudioStream {
    public:

      AudioStream();
      
      AudioStream(std::unique_ptr<AudioReader> reader, std::unique_ptr<IBasicAudioStream> stream);

      /**
       * @brief Prepare stream for usage.
       */
      void setup();

      /**
       * @brief Start audio stream.
       */
      void start();

      /**
       * @brief Fast forward or backward through audio stream.
       * @param seconds The number of seconds to move through the stream. Positive moves forward, negative moves backwards.
       */
      void seek(float seconds);

      /**
       * @brief Stop audio stream.
       */
      void stop();
      
      /**
       * @brief Get current time position in audio stream.
       * @return Current time position in seconds.
       */
      long position();
      
      /**
       * @brief Get total duration of audio stream.
       * @return Total duration in seconds.
       */
      long duration();
      
      /**
       * @brief Whether the stream has finished processing data.
       */
      bool isFinished();
      
      /**
       * @brief Whether the stream is currently playing audio.
       */
      bool isActive();
      
      /**
       * @brief Whether the stream has completely stopped processing audio.
       */
      bool isStopped();
      
      /**
       * @brief End audio stream.
       */
      void end();

    private:

      enum StreamState {
        OPEN,
        CLOSED
      };
      
      enum PlayingState {
        PLAYING,
        SEEKING,
        STOPPED,
        BUFFERING
      };

      static int MAX_LOADED_CHUNKS;
      
      // Essential
      std::unique_ptr<AudioReader> reader;
      std::unique_ptr<IBasicAudioStream> basicAudioStream;
      std::thread audioReaderThread;

      // State
      PlayingState playingState = PlayingState::STOPPED;
      StreamState streamState = StreamState::CLOSED;

      void handleError();
      void audioReaderThreadCallback();
  };
}