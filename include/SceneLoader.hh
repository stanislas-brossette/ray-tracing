#pragma once
#include <iostream>
#include <cstdio>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

#include "dataStructures.hh"
#include "Vector3.hh"
#include "Frame3.hh"
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "Cylinder.hh"
#include "ClosedCylinder.hh"
#include "Plane.hh"
#include "Polygon.hh"
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
    SceneLoader (const std::string& path);
    virtual ~SceneLoader ();
    SceneData load(const std::string& path);
    void scanVector2(Value& vIn, Vector2& vRes);
    void scanVector3(Value& vIn, Vector3& vRes);
    void scanVector3RGB(Value& vIn, Vector3RGB& vRes);
    CamData scanCamera(Value& vIn);
    RenderData scanRender(Value& vIn);
    AmbiantData scanAmbiant(Value& vIn);
    MaterialData* scanMaterial(Value& vIn);
    void scanBaseGeometry(Value& vIn, GeometryData* gData);
    GeometryData* scanGeometry(Value& vIn);
    void scanItems(Value& vIn, std::vector<ItemData>& vOut);

    SceneData sceneData_;

private:
};
