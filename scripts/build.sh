cmake -B build -S . -G "MinGW Makefiles" -DLIBAUDIO_DEBUG_PROGRAM=true -DCMAKE_BUILD_TYPE=Debug
make -j $(nproc) -C build