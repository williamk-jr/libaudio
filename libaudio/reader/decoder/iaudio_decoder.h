#pragma once
#include "../../audio_file_descriptor.h"
#include "../../audio_chunk.h"
#include "../../audio_buffer.h"

#include <queue>
#include <filesystem>

namespace iamaprogrammer {
  class AudioReader;

  class IAudioDecoder {
  friend class AudioReader;
  friend class IAudioResampler;

  protected:

    /*
    @brief Opens the audio file.
    */
    virtual void open(std::filesystem::path filePath) = 0;

    /*
    @brief Reads audio into the provided buffer.
    @return The number of frames read. This can be less than the requested frame count if the end of the file is reached.
    */
    virtual size_t read(float* buffer) = 0;

    /*
    @brief Seeks a specified number of frames from a specified position. The position is determined by the whence parameter, which can be SEEK_SET, SEEK_CUR, or SEEK_END.
    */
    virtual void seek(size_t frames, int whence) = 0;


    /*
    @brief Gets a descriptor containing information on how to handle the audio file.
    */
    virtual AudioFileDescriptor& getAudioFileDescriptor() = 0;

    /*
    @brief Gets the maximum number of frames that can be read in a single read operation.
    */
    virtual int getFrameReadCount() = 0;

    /*
    @brief Closes the audio file reader.
    */
    virtual void close() = 0;
  };
}