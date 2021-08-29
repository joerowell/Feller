cd build
LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libasan.so.5.0.0 ASAN_OPTIONS=alloc_dealloc_mismatch=0 ctest -j4
lcov -c --directory . --output-file main_coverage.info
genhtml main_coverage.info --output-directory out
cd ../
