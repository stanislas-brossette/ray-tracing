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

SceneLoader::~SceneLoader()
{
}

void SceneLoader::load(const std::string& path)
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

    camData cData = scanCamera(document.FindMember("camera")->value);
    std::cout << cData.describe();
    renderData rData = scanRender(document.FindMember("render")->value);
    std::cout << rData.describe();
    ambiantData aData = scanAmbiant(document.FindMember("ambiantLight")->value);
    std::cout << aData.describe();
    std::vector<itemData> itemVec;
    scanItems(document.FindMember("items")->value, itemVec);
    for (size_t i = 0; i < itemVec.size(); i++)
    {
        std::cout << itemVec[i].describe();
    }
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

camData SceneLoader::scanCamera(Value& vIn)
{
    camData cData;
    cData.resX = vIn.FindMember("resX")->value.GetInt();
    cData.fovX = vIn.FindMember("fovX")->value.GetDouble();
    cData.fovY = vIn.FindMember("fovY")->value.GetDouble();
    cData.rotAngle = vIn.FindMember("rotAngle")->value.GetDouble();
    scanVector3(vIn.FindMember("pos")->value, cData.pos);
    scanVector3(vIn.FindMember("rotAxis")->value, cData.rotAxis);
    return cData;
}

renderData SceneLoader::scanRender(Value& vIn)
{
    renderData rData;
    rData.nPixPerRender = vIn.FindMember("nPixPerRender")->value.GetInt();
    rData.nLightRay = vIn.FindMember("nLightRay")->value.GetInt();
    return rData;
}

ambiantData SceneLoader::scanAmbiant(Value& vIn)
{
    ambiantData aData;
    aData.intensity = vIn.FindMember("intensity")->value.GetDouble();
    scanVector3RGB(vIn.FindMember("color")->value, aData.color);
    return aData;
}

materialData* SceneLoader::scanMaterial(Value& vIn)
{
    materialData* mData = new materialData();
    scanVector3RGB(vIn.FindMember("color")->value, mData->color);
    mData->rugosity = vIn.FindMember("rugosity")->value.GetDouble();
    mData->refraction = vIn.FindMember("refraction")->value.GetDouble();
    mData->reflectiveness = vIn.FindMember("reflectiveness")->value.GetDouble();
    mData->lightEmitter = vIn.FindMember("lightEmitter")->value.GetBool();
    mData->lightIntensity = vIn.FindMember("lightIntensity")->value.GetDouble();
    return mData;
}

void SceneLoader::scanBaseGeometry(Value& vIn, geometryData* gData)
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

geometryData* SceneLoader::scanGeometry(Value& vIn)
{
    std::string type = vIn.FindMember("type")->value.GetString();
    if(type == "Sphere")
    {
        sphereData* sData = new sphereData();
        scanBaseGeometry(vIn, sData);
        sData->radius = vIn.FindMember("radius")->value.GetDouble();
        return sData;
    }
    else if(type == "Plane")
    {
        planeData* pData = new planeData();
        scanBaseGeometry(vIn, pData);
        return pData;
    }
    else if(type == "Cylinder")
    {
        cylinderData* cData = new cylinderData();
        scanBaseGeometry(vIn, cData);
        cData->radius = vIn.FindMember("radius")->value.GetDouble();
        cData->length = vIn.FindMember("length")->value.GetDouble();
        return cData;
    }
    else if(type == "ClosedCylinder")
    {
        closedCylinderData* cData = new closedCylinderData();
        scanBaseGeometry(vIn, cData);
        cData->radius = vIn.FindMember("radius")->value.GetDouble();
        cData->length = vIn.FindMember("length")->value.GetDouble();
        return cData;
    }
    else
    {
        std::cout << "ERROR: wrong item type" << std::endl;
    }

    geometryData* gData = new geometryData();
    return gData;
}

void SceneLoader::scanItems(Value& vIn, std::vector<itemData>& vOut)
{
    for (auto& o : vIn.GetObject())
    {
        itemData iData;
        iData.name = o.name.GetString();
        iData.mData = scanMaterial(o.value.FindMember("material")->value);
        iData.gData = scanGeometry(o.value.FindMember("geometry")->value);
        vOut.push_back(iData);
    }
}

