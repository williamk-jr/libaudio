cmake -B build -S . -DLIBAUDIO_DEBUG_PROGRAM=true
make -j $(nproc) -C build