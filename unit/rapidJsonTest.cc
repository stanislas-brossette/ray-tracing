#include <gtest/gtest.h>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "Vector3.hh"
#include <iostream>
#include <cstdio>

using namespace rapidjson;

void scanVector3(Value& vIn, Vector3& vRes)
{
    std::vector<double> vec3;
    for (auto& vec : vIn.GetArray())
        vec3.push_back(vec.GetDouble());
    vRes = Vector3(vec3[0], vec3[1], vec3[2]);
}

void scanVector3RGB(Value& vIn, Vector3RGB& vRes)
{
    std::vector<int> vec3;
    for (auto& vec : vIn.GetArray())
        vec3.push_back(vec.GetDouble());
    vRes = Vector3RGB(vec3[0], vec3[1], vec3[2]);
}

struct camData
{
    int resX = 0;
    double fovX = 0;
    double fovY = 0;
    Vector3 pos = Vector3(0,0,0);
    Vector3 rotAxis = Vector3(0,0,0);
    double rotAngle = 0;
};

camData scanCamera(Value& vIn)
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

struct renderData
{
    int nPixPerRender = 0;
    int nLightRay = 0;
};

renderData scanRender(Value& vIn)
{
    renderData rData;
    rData.nPixPerRender = vIn.FindMember("nPixPerRender")->value.GetInt();
    rData.nLightRay = vIn.FindMember("nLightRay")->value.GetInt();
    std::cout << "render.nPixPerRender: " << rData.nPixPerRender << std::endl;
    std::cout << "render.nLightRay: " << rData.nLightRay << std::endl;
    return rData;
}

struct ambiantData
{
    Vector3RGB color = Vector3RGB(0, 0, 0);
    double intensity = 0;
};

ambiantData scanAmbiant(Value& vIn)
{
    ambiantData aData;
    aData.intensity = vIn.FindMember("intensity")->value.GetDouble();
    scanVector3RGB(vIn.FindMember("color")->value, aData.color);
    std::cout << "ambiantLight.intensity: " << aData.intensity << std::endl;
    std::cout << "ambiantLight.color: " << aData.color << std::endl;
    return aData;
}
 
TEST(rapidJsonTest, RapidJsonTest)
{
    std::string path("/home/stanislas/profiles/devel/src/raytracing/unit/data/cylinderTest.json");
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
     
    ASSERT_TRUE(true);
}
