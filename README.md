
### Building locally

```bash
mkdir build
cd build

cmake ..
make
```

### Testing locally

After building locally, run
```bash
ctest
```

### Test coverage
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS_DEBUG="-g -fprofile-arcs -ftest-coverage"     -DCMAKE_C_FLAGS_DEBUG="-g -fprofile-arcs -ftest-coverage"

make -j

ctest -T Test -T Coverage
```

#### Coverage report
```txt
Performing coverage
   Processing coverage (each . represents one file):
    ..................................
   Accumulating results (each . represents one file):
    ..............................................
        Covered LOC:         1471
        Not covered LOC:     441
        Total LOC:           1912
        Percentage Coverage: 76.94%
```