#include "SceneLoader.hh"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "Vector3.hh"
#include <iostream>
#include <cstdio>

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

    sceneData_.cData = scanCamera(document.FindMember("camera")->value);
    sceneData_.rData = scanRender(document.FindMember("render")->value);
    sceneData_.aData = scanAmbiant(document.FindMember("ambiantLight")->value);
    scanItems(document.FindMember("items")->value, sceneData_.itemsData);
    return sceneData_;
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
    std::vector<int> vec3;
    for (auto& vec : vIn.GetArray())
        vec3.push_back(vec.GetDouble());
    vRes = Vector3RGB(vec3[0], vec3[1], vec3[2]);
}

CamData SceneLoader::scanCamera(Value& vIn)
{
    CamData cData;
    cData.resX = vIn.FindMember("resX")->value.GetInt();
    cData.fovX = vIn.FindMember("fovX")->value.GetDouble();
    cData.fovY = vIn.FindMember("fovY")->value.GetDouble();

    Value::MemberIterator itr = vIn.FindMember("fovType");
    if(itr != vIn.MemberEnd())
        cData.fovType = itr->value.GetInt();

    cData.rotAngle = vIn.FindMember("rotAngle")->value.GetDouble();
    scanVector3(vIn.FindMember("pos")->value, cData.pos);
    scanVector3(vIn.FindMember("rotAxis")->value, cData.rotAxis);
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
    scanVector3RGB(vIn.FindMember("color")->value, mData->color);
    mData->rugosity = vIn.FindMember("rugosity")->value.GetDouble();
    mData->refraction = vIn.FindMember("refraction")->value.GetDouble();
    mData->reflectiveness = vIn.FindMember("reflectiveness")->value.GetDouble();
    mData->lightEmitter = vIn.FindMember("lightEmitter")->value.GetBool();
    mData->lightIntensity = vIn.FindMember("lightIntensity")->value.GetDouble();
    return mData;
}

void SceneLoader::scanBaseGeometry(Value& vIn, GeometryData* gData)
{
    gData->type = vIn.FindMember("type")->value.GetString();
    scanVector3(vIn.FindMember("pos")->value, gData->pos);

    Value::MemberIterator itr = vIn.FindMember("rotAxis");
    if(itr != vIn.MemberEnd())
        scanVector3(itr->value, gData->rotAxis);

    itr = vIn.FindMember("rotAngle");
    if(itr != vIn.MemberEnd())
        gData->rotAngle = itr->value.GetDouble();
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

