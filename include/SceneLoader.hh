#pragma once
#include <iostream>

#include "Vector3.hh"
#include "Frame3.hh"
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "Cylinder.hh"
#include "Plane.hh"
#include "Camera.hh"
#include "Window.hh"

class SceneLoader
{
public:
    SceneLoader ();
    virtual ~SceneLoader ();
    Scene& load(const std::string& path);

private:
};
