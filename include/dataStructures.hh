#pragma once

#include <string>
#include <vector>

#include "Vector3.hh"

struct RenderData
{
    int nPixPerRender = 0;
    int percentLightRay = 100;
    std::string describe() const;
};

struct AmbiantData
{
    Vector3RGB color = Vector3RGB(0, 0, 0);
    double intensity = 0;
    std::string describe() const;
};

struct CamData
{
    int resX = 0;
    double fovX = 0;
    double fovY = 0;
    Vector3 pos = Vector3(0,0,0);
    Vector3 rotAxis = Vector3(0,0,0);
    double rotAngle = 0;
    int fovType = 1;
    std::string describe() const;
};

struct MaterialData
{
    Vector3RGB color = Vector3RGB(0,0,0);
    double rugosity = 0.0;
    double refraction = 0.0;
    double reflectiveness = 0.0;
    bool lightEmitter = false;
    double lightIntensity = 0.0;
    std::string describe() const;
};

struct GeometryData
{
    std::string type = "";
    Vector3 pos = Vector3(0,0,0);
    Vector3 rotAxis = Vector3(1,0,0);
    double rotAngle = 0;
    virtual std::string describe() const;
};

struct ItemData
{
    std::string name;
    MaterialData* mData;
    GeometryData* gData;
    std::string describe() const;
};

struct ClosedCylinderData : public GeometryData
{
    double radius = 0;
    double length = 1e9;
    std::string describe() const;
};

struct CylinderData : public GeometryData
{
    double radius = 0;
    double length = 1e9;
    std::string describe() const;
};

struct PlaneData : public GeometryData
{
    std::string describe() const;
};

struct PolygonData : public GeometryData
{
    std::string describe() const;
};

struct SphereData : public GeometryData
{
    double radius = 0;
    std::string describe() const;
};

struct SceneData
{
    AmbiantData aData;
    CamData cData;
    RenderData rData;
    std::vector<ItemData> itemsData;
    std::string describe() const;
};

