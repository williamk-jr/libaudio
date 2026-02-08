#include <iostream>
#include <chrono>
#include <thread>
#include <audio_stream.h>
#include <audio_stream_factory.h>
#include <backends/port_audio_backend.h>

int main() {
  std::cout << "hello world" << "\n";
  iamaprogrammer::PortAudioBackend backend;
  backend.initialize();

  // iamaprogrammer::SndlibAudioReader reader(1024);
  // reader.open("test_files/test.mp3");

  // iamaprogrammer::SRAudioResampler resampler(reader, backend.getDefaultAudioDevice().samplerate);
  // iamaprogrammer::PortAudioStream stream(reader, resampler);

  // iamaprogrammer::AudioStream audioStream(&reader, &resampler, &stream);
  iamaprogrammer::AudioStream audioStream = 
      iamaprogrammer::AudioStreamFactory::fromFile(backend, "test_files/test.mp3");

  audioStream.setup();
  audioStream.start();

  while (!audioStream.isFinished()) {
    //Pa_Sleep(1000);
    //std::cout << audioStream.position() << " s/ " << audioStream.duration() << " s" << std::endl;
  }

  audioStream.stop();
  audioStream.end();
  backend.terminate();
  return 0;
}