# Ray tracing simulator

# installation:
mkdir build  
cd build  
cmake ..  
make  

# test(best to watch):
cd build  
./unit/unit  

# run main.cc:
./RayTracingMain

# play around with scenes:
./RayTracingMainFromJson
The scene can be conveniently modified in ./data/testScene.json

# filter tests:
cd build  
./unit/unit --gtest_filter=\*Plane\* (or replace Plane with 9 or 1 or Sphere or whatever test you want)  
