# Ray tracing simulator

# installation:
sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev libsdl2-mixer-dev

mkdir build  
cd build  
cmake ..  
make  

# tests:
cd build  
./unit/unit  

# run main.cc:
./RayTracingMain

# play around with scenes:
./RayTracingMainFromJson testScene.json  
./RayTracingMainFromJson tableScene.json  
....

The scene can be conveniently modified in the json files in ./data/

# filter tests:
cd build  
./unit/unit --gtest_filter=\*Plane\* (or replace Plane with 9 or 1 or Sphere or whatever test you want)  
