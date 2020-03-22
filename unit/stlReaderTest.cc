#include <gtest/gtest.h>
#include <iostream>

#include "stl_reader/stl_reader.h"


TEST(UnitTests, stlTest)
{
    std::vector<float> coords, normals;
    std::vector<unsigned int> tris, solids;

    try {
      stl_reader::ReadStlFile ("geometry.stl", coords, normals, tris, solids);
      const size_t numTris = tris.size() / 3;
      for(size_t itri = 0; itri < numTris; ++itri) {
          std::cout << "coordinates of triangle " << itri << ": ";
          for(size_t icorner = 0; icorner < 3; ++icorner) {
              float* c = &coords[3 * tris [3 * itri + icorner]];
              std::cout << "(" << c[0] << ", " << c[1] << ", " << c[2] << ") ";
          }
          std::cout << std::endl;
      
          float* n = &normals [3 * itri];
          std::cout   << "normal of triangle " << itri << ": "
                      << "(" << n[0] << ", " << n[1] << ", " << n[2] << ")\n";
      }
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
}
