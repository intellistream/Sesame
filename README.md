# MOStream
### Prerequisites
- gcc 12 or latr
- cmake 3.14 or later
- [boost](https://github.com/boostorg/boost)
- [gflags](https://github.com/gflags/gflags)
### Build
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```
### Run Tests
Run the tests:
```bash
cd build/test
./google_test
```