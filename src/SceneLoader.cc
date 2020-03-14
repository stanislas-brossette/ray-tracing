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
    std::cout << "readBuffer: " << readBuffer << std::endl;
    Document document;
    document.Parse(readBuffer);
    std::cout << "document.IsObject(): " << document.IsObject() << std::endl;
    fclose(fp);
    if(document.HasParseError())
    {
        std::cout << "ERROR: Json has errors" << std::endl;
    }

    camData cData = scanCamera(document.FindMember("camera")->value);
    renderData rData = scanRender(document.FindMember("render")->value);
    ambiantData aData = scanAmbiant(document.FindMember("ambiantLight")->value);
    std::vector<itemData> itemVec;
    scanItems(document.FindMember("items")->value, itemVec);
    std::cout << "Scanned " << itemVec.size() << " items" << std::endl;

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
    std::cout << "camera.resX: " << cData.resX << std::endl;
    std::cout << "camera.fovX: " << cData.fovX << std::endl;
    std::cout << "camera.fovY: " << cData.fovY << std::endl;
    std::cout << "camera.rotAxis: " << cData.rotAxis << std::endl;
    std::cout << "camera.rotAngle: " << cData.rotAngle << std::endl;
    std::cout << "camera.pos: " << cData.pos << std::endl;
    return cData;
}

renderData SceneLoader::scanRender(Value& vIn)
{
    renderData rData;
    rData.nPixPerRender = vIn.FindMember("nPixPerRender")->value.GetInt();
    rData.nLightRay = vIn.FindMember("nLightRay")->value.GetInt();
    std::cout << "render.nPixPerRender: " << rData.nPixPerRender << std::endl;
    std::cout << "render.nLightRay: " << rData.nLightRay << std::endl;
    return rData;
}

ambiantData SceneLoader::scanAmbiant(Value& vIn)
{
    ambiantData aData;
    aData.intensity = vIn.FindMember("intensity")->value.GetDouble();
    scanVector3RGB(vIn.FindMember("color")->value, aData.color);
    std::cout << "ambiantLight.intensity: " << aData.intensity << std::endl;
    std::cout << "ambiantLight.color: " << aData.color << std::endl;
    return aData;
}

materialData SceneLoader::scanMaterial(Value& vIn)
{
    materialData mData;
    scanVector3RGB(vIn.FindMember("color")->value, mData.color);
    mData.rugosity = vIn.FindMember("rugosity")->value.GetDouble();
    mData.refraction = vIn.FindMember("refraction")->value.GetDouble();
    mData.reflectiveness = vIn.FindMember("reflectiveness")->value.GetDouble();
    mData.lightEmitter = vIn.FindMember("lightEmitter")->value.GetBool();
    mData.lightIntensity = vIn.FindMember("lightIntensity")->value.GetDouble();
    std::cout << "mData.color : " << mData.color << std::endl;
    std::cout << "mData.rugosity : " << mData.rugosity << std::endl;
    std::cout << "mData.refraction : " << mData.refraction << std::endl;
    std::cout << "mData.reflectiveness : " << mData.reflectiveness << std::endl;
    std::cout << "mData.lightEmitter : " << mData.lightEmitter << std::endl;
    std::cout << "mData.lightIntensity : " << mData.lightIntensity << std::endl;
    return mData;
}

geometryData SceneLoader::scanGeometry(Value& vIn)
{
    geometryData gData;
    gData.type = vIn.FindMember("type")->value.GetString();
    scanVector3(vIn.FindMember("pos")->value, gData.pos);

    Value::MemberIterator itr = vIn.FindMember("rotAxis");
    if(itr != vIn.MemberEnd())
        scanVector3(itr->value, gData.rotAxis);

    itr = vIn.FindMember("rotAngle");
    if(itr != vIn.MemberEnd())
        gData.rotAngle = itr->value.GetDouble();

    std::cout << "gData.type : " << gData.type << std::endl;
    std::cout << "gData.pos : " << gData.pos << std::endl;
    std::cout << "gData.rotAxis : " << gData.rotAxis << std::endl;
    std::cout << "gData.rotAngle : " << gData.rotAngle << std::endl;
    return gData;
}

void SceneLoader::scanItems(Value& vIn, std::vector<itemData>& vOut)
{
    for (auto& o : vIn.GetObject())
    {
        std::cout << "scan item: " << o.name.GetString() << std::endl;
        itemData iData;
        iData.mData = scanMaterial(o.value.FindMember("material")->value);
        iData.gData = scanGeometry(o.value.FindMember("geometry")->value);
        vOut.push_back(iData);
    }
}

