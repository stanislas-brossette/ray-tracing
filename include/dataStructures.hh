#pragma once

#include <string>
#include <vector>

#include "Vector3.hh"
#include "utils.hh"

struct SceneExtraData
{
    int maxNumThreads = 8;
    int maxDepthRecursion = 5;
    int powerDistReduction = 2;
    double exposition = 1.0;
    double gammaPower = 2.2;
    bool simplifiedRender = false;
};

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
    double fov = 0;
    Vector3 target = Vector3(0,0,0);
    Vector3 upGuide = Vector3(0,0,0);
    double aspectRatio = 1.0;
    Vector3 pos = Vector3(0,0,0);
    Vector3 rotAxis = Vector3(0,0,0);
    double rotAngle = 0;
    int resX = 0;
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

struct ClosedCylinderData : public GeometryData
{
    double radius = 0;
    double length = INFINITY_d();
    std::string describe() const;
};

struct CylinderData : public GeometryData
{
    double radius = 0;
    double length = 1e9;
    std::string describe() const;
};

struct CubeData : public GeometryData
{
    double size = 1.0;
    std::string describe() const;
};

struct PlaneData : public GeometryData
{
    std::string describe() const;
};

struct PolygonData : public GeometryData
{
    std::vector<Vector2> points;
    std::string describe() const;
};

struct SphereData : public GeometryData
{
    double radius = 0;
    std::string describe() const;
};

struct MeshData : public GeometryData
{
    std::string path = "";
    std::string describe() const;
};

struct TextureData
{
    virtual ~TextureData() = default;
};

struct SolidColorData : public TextureData
{
    Vector3RGB color = Vector3RGB(0,0,0);
    std::string describe() const;
};

struct CheckerBoardData : public TextureData
{
    Vector3RGB color0 = Vector3RGB(0,0,0);
    Vector3RGB color1 = Vector3RGB(0,0,0);
    double squareSizeX = 0;
    double squareSizeY = 0;
    std::string describe() const;
};

struct ImageData : public TextureData
{
    std::string path = "";
    double sizeX = 0;
    double sizeY = 0;
    std::string describe() const;
};

struct MaterialData
{
    std::string textureType = "";
    TextureData *textureData;
    int specularExponent = 50;
    double specularGain = 0.1;
    double rugosity = 0.0;
    double refraction = 0.0;
    double reflectiveness = 0.0;
    bool lightEmitter = false;
    double lightIntensity = 0.0;
    std::string describe() const;
    virtual ~MaterialData() = default;
};

struct ItemData
{
    std::string name;
    MaterialData* mData;
    GeometryData* gData;
    std::string describe() const;
};

struct SceneData
{
    AmbiantData aData;
    CamData cData;
    RenderData rData;
    SceneExtraData sExData;
    std::vector<ItemData> itemsData;
    std::string describe() const;
};

