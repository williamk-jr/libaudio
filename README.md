# Description

A small library for managing audio stream creation.

# Installation
Install PortAudio, libsndfile, and libsamplerate

### Ubuntu
```shell
sudo apt-get install portaudio19-dev libsndfile-dev libsamplerate-dev
```

Clone repository.
```shell
git clone https://github.com/williamk-jr/libaudio
```
## Library
Add as subdirectory in your project CMakeLists.

## Debug
Run debug executable using build script.
```shell
chmod +x ./scripts/build.sh
./scripts/build.sh
```

# Usage

### Backend
First, you need to pick a backend.
Currently, Portaudio is the only backend to choose from, more may be added later.

```cpp
iamaprogrammer::PortAudioBackend backend;
backend.initialize();
```

### Using the factory
Once you have a backend, you can create audio streams.
Libaudio provides multiple different ways to create audio streams.
However, the easiest way is to use the factory.

```cpp
iamaprogrammer::AudioStream audioStream = iamaprogrammer::AudioStreamFactory::fromFile(backend, "test_files/test.mp3");
```

You may also pass in a settings object where you can choose the decoder, resampler, and audio stream implementations.
This provides you more freedom in how the components are constructed.

### Using components directly
You may also manually construct the components of an audio stream.

```cpp
// TODO
```
