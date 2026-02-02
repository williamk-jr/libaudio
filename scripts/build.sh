cmake -B build -S . -DLIBAUDIO_DEBUG_PROGRAM
make -j $(nproc) -C build