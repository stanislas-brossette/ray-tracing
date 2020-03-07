# Ray tracing simulator

# installation:
mkdir build
cd build
cmake ..
make

# test:
cd build
./unit/unit

# filter tests:
cd build
./unit/unit --gtest_filter=\*Plane\* (or replace Plane with 9 or 1 or Sphere or whatever test you want)
