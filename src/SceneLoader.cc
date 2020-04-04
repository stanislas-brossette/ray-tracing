#include <iostream>
#include <cstdio>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

#include "SceneLoader.hh"
#include "Vector3.hh"

using namespace rapidjson;

SceneLoader::SceneLoader()
{
}

SceneLoader::SceneLoader(const std::string& path)
{
    load(path);
}

SceneLoader::~SceneLoader()
{
}

SceneData SceneLoader::load(const std::string& path)
{
    FILE* fp = fopen(path.c_str(), "r");
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document document;
    document.Parse(readBuffer);
    fclose(fp);
    if(document.HasParseError())
    {
        std::cout << "ERROR: Json has errors" << std::endl;
    }

    sceneData_.sExData = scanScene(document.FindMember("scene")->value);
    sceneData_.cData = scanCamera(document.FindMember("camera")->value);
    sceneData_.rData = scanRender(document.FindMember("render")->value);
    sceneData_.aData = scanAmbiant(document.FindMember("ambiantLight")->value);
    scanItems(document.FindMember("items")->value, sceneData_.itemsData);
    return sceneData_;
}

void SceneLoader::scanVector2(Value& vIn, Vector2& vRes)
{
    std::vector<double> vec2;
    for (auto& vec : vIn.GetArray())
        vec2.push_back(vec.GetDouble());
    vRes = Vector2(vec2[0], vec2[1]);
}

void SceneLoader::scanVector3(Value& vIn, Vector3& vRes)
{
    std::vector<double> vec3;
    for (auto& vec : vIn.GetArray())
        vec3.push_back(vec.GetDouble());
    vRes = Vector3(vec3[0], vec3[1], vec3[2]);
}

void SceneLoader::scanVector3RGB(Value& vIn, Vector3RGB& vRes)
{
    std::vector<double> vec3;
    for (auto& vec : vIn.GetArray())
        vec3.push_back(vec.GetDouble());
    vRes = Vector3RGB(vec3[0]/255.0, vec3[1]/255.0, vec3[2]/255.0);
}

SceneExtraData SceneLoader::scanScene(Value& vIn)
{
    SceneExtraData sData;
    sData.maxNumThreads = vIn.FindMember("maxNumThreads")->value.GetInt();
    sData.powerDistReduction = vIn.FindMember("powerDistReduction")->value.GetInt();
    sData.exposition = vIn.FindMember("exposition")->value.GetDouble();
    sData.gammaPower = vIn.FindMember("gammaPower")->value.GetDouble();
    sData.simplifiedRender = vIn.FindMember("simplifiedRender")->value.GetBool();
    sData.maxDepthRecursion = vIn.FindMember("maxDepthRecursion")->value.GetInt();
    return sData;
}

CamData SceneLoader::scanCamera(Value& vIn)
{
    CamData cData;
    cData.fov = vIn.FindMember("fov")->value.GetDouble();
    scanVector3(vIn.FindMember("target")->value, cData.target);
    scanVector3(vIn.FindMember("upGuide")->value, cData.upGuide);
    cData.aspectRatio = vIn.FindMember("aspectRatio")->value.GetDouble();
    scanVector3(vIn.FindMember("pos")->value, cData.pos);
    cData.rotAngle = vIn.FindMember("rotAngle")->value.GetDouble();
    scanVector3(vIn.FindMember("rotAxis")->value, cData.rotAxis);
    cData.resX = vIn.FindMember("resX")->value.GetInt();
    return cData;
}

RenderData SceneLoader::scanRender(Value& vIn)
{
    RenderData rData;
    rData.nPixPerRender = vIn.FindMember("nPixPerRender")->value.GetInt();
    rData.percentLightRay = vIn.FindMember("percentLightRay")->value.GetInt();
    return rData;
}

AmbiantData SceneLoader::scanAmbiant(Value& vIn)
{
    AmbiantData aData;
    aData.intensity = vIn.FindMember("intensity")->value.GetDouble();
    scanVector3RGB(vIn.FindMember("color")->value, aData.color);
    return aData;
}

MaterialData* SceneLoader::scanMaterial(Value& vIn)
{
    MaterialData* mData = new MaterialData();
    Value::MemberIterator itr = vIn.FindMember("specularExponent");
    if(itr != vIn.MemberEnd()) mData->specularExponent = itr->value.GetInt();
    itr = vIn.FindMember("specularGain");
    if(itr != vIn.MemberEnd()) mData->specularGain = itr->value.GetDouble();
    mData->rugosity = vIn.FindMember("rugosity")->value.GetDouble();
    mData->refraction = vIn.FindMember("refraction")->value.GetDouble();
    itr = vIn.FindMember("absorption");
    if(itr != vIn.MemberEnd()) mData->absorption = itr->value.GetDouble();
    mData->reflectiveness = vIn.FindMember("reflectiveness")->value.GetDouble();
    mData->lightEmitter = vIn.FindMember("lightEmitter")->value.GetBool();
    mData->lightIntensity = vIn.FindMember("lightIntensity")->value.GetDouble();
    mData->textureType = vIn.FindMember("textureType")->value.GetString();
    mData->textureData = scanTexture(vIn);
    return mData;
}

TextureData* SceneLoader::scanTexture(Value& vIn)
{
    std::string type = vIn.FindMember("textureType")->value.GetString();
    if(type == "SolidColor")
    {
        SolidColorData* tData = new SolidColorData();
        scanVector3RGB(vIn.FindMember("color")->value, tData->color);
        return tData;
    }
    if(type == "CheckerBoard")
    {
        CheckerBoardData* tData = new CheckerBoardData();
        scanVector3RGB(vIn.FindMember("color0")->value, tData->color0);
        scanVector3RGB(vIn.FindMember("color1")->value, tData->color1);
        tData->squareSizeX = vIn.FindMember("squareSizeX")->value.GetDouble();
        tData->squareSizeY = vIn.FindMember("squareSizeY")->value.GetDouble();
        return tData;
    }
    if(type == "Image")
    {
        ImageData* tData = new ImageData();
        tData->path = vIn.FindMember("path")->value.GetString();
        tData->sizeX = vIn.FindMember("sizeX")->value.GetDouble();
        tData->sizeY = vIn.FindMember("sizeY")->value.GetDouble();
        return tData;
    }
    else
    {
        std::cout << "Unknown textureType in SceneLoader::scanTexture" << std::endl;
    }
}

void SceneLoader::scanBaseGeometry(Value& vIn, GeometryData* gData)
{
    gData->type = vIn.FindMember("type")->value.GetString();
    scanVector3(vIn.FindMember("pos")->value, gData->pos);

    Value::MemberIterator itr = vIn.FindMember("rotAxis");
    if(itr != vIn.MemberEnd()) scanVector3(itr->value, gData->rotAxis);

    itr = vIn.FindMember("rotAngle");
    if(itr != vIn.MemberEnd()) gData->rotAngle = itr->value.GetDouble();
}

GeometryData* SceneLoader::scanGeometry(Value& vIn)
{
    std::string type = vIn.FindMember("type")->value.GetString();
    if(type == "Sphere")
    {
        SphereData* sData = new SphereData();
        scanBaseGeometry(vIn, sData);
        sData->radius = vIn.FindMember("radius")->value.GetDouble();
        return sData;
    }
    else if(type == "Plane")
    {
        PlaneData* pData = new PlaneData();
        scanBaseGeometry(vIn, pData);
        return pData;
    }
    else if(type == "Polygon")
    {
        PolygonData* pData = new PolygonData();
        scanBaseGeometry(vIn, pData);
        {
            const Value& pointsValue = vIn["points"];
            pData->points.resize(pointsValue.Size());
            for (SizeType i = 0; i < pointsValue.Size(); i++)
            {
                pData->points[i] = Vector2(pointsValue[i]["x"].GetDouble(), pointsValue[i]["y"].GetDouble());
            }
        }
        return pData;
    }
    else if(type == "Cylinder")
    {
        CylinderData* cData = new CylinderData();
        scanBaseGeometry(vIn, cData);
        cData->radius = vIn.FindMember("radius")->value.GetDouble();
        cData->length = vIn.FindMember("length")->value.GetDouble();
        return cData;
    }
    else if(type == "ClosedCylinder")
    {
        ClosedCylinderData* cData = new ClosedCylinderData();
        scanBaseGeometry(vIn, cData);
        cData->radius = vIn.FindMember("radius")->value.GetDouble();
        cData->length = vIn.FindMember("length")->value.GetDouble();
        return cData;
    }
    else if(type == "Cube")
    {
        CubeData* cData = new CubeData();
        scanBaseGeometry(vIn, cData);
        cData->size = vIn.FindMember("size")->value.GetDouble();
        return cData;
    }
    else if(type == "Mesh")
    {
        MeshData* mData = new MeshData();
        scanBaseGeometry(vIn, mData);
        mData->path = vIn.FindMember("path")->value.GetString();
        return mData;
    }
    else
    {
        std::cout << "ERROR: wrong item type" << std::endl;
    }

    GeometryData* gData = new GeometryData();
    return gData;
}

void SceneLoader::scanItems(Value& vIn, std::vector<ItemData>& vOut)
{
    for (auto& o : vIn.GetObject())
    {
        ItemData iData;
        iData.name = o.name.GetString();
        iData.mData = scanMaterial(o.value.FindMember("material")->value);
        iData.gData = scanGeometry(o.value.FindMember("geometry")->value);
        vOut.push_back(iData);
    }
}

