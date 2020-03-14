#pragma once
#include <iostream>
#include <cstdio>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

#include "Vector3.hh"
#include "Frame3.hh"
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "Cylinder.hh"
#include "Plane.hh"
#include "Camera.hh"
#include "Window.hh"
#include "Renderer.hh"
#include "AmbiantLight.hh"
#include "Material.hh"
#include "Geometry.hh"

using namespace rapidjson;

class SceneLoader
{
public:
    SceneLoader ();
    virtual ~SceneLoader ();
    void load(const std::string& path);
    void scanVector3(Value& vIn, Vector3& vRes);
    void scanVector3RGB(Value& vIn, Vector3RGB& vRes);
    camData scanCamera(Value& vIn);
    renderData scanRender(Value& vIn);
    ambiantData scanAmbiant(Value& vIn);
    materialData scanMaterial(Value& vIn);
    geometryData scanGeometry(Value& vIn);
    void scanItems(Value& vIn, std::vector<itemData>& vOut);

private:
};
