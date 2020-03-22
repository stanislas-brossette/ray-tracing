#include <gtest/gtest.h>
#include <iostream>
#include "Vector3.hh"
#include "Frame3.hh"
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "Cylinder.hh"
#include "ClosedCylinder.hh"
#include "Plane.hh"
#include "Mesh.hh"
#include "Camera.hh"
#include "Window.hh"

TEST(UnitTests, MeshTest)
{
    double prec = 1e-9;
    Frame3 f;
    std::string path = std::string(MESHES) + "cube.STL";
    std::cout << "path: " << path << std::endl;
    Mesh m(f, path);
    ASSERT_TRUE(true);
}
