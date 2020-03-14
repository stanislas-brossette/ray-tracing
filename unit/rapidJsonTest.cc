#include <gtest/gtest.h>
// rapidjson/example/simpledom/simpledom.cpp`
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Vector3.hh"
#include <iostream>
#include <cstdio>

using namespace rapidjson;

struct camData
{
    int resX = 0;
    double fovX = 0;
    double fovY = 0;
    Vector3 pos = Vector3(0,0,0);
    Vector3 rotAxis = Vector3(0,0,0);
    double rotAngle = 0;
};

void scanVector3(Value& vIn, Vector3& vRes)
{
    std::vector<double> vec3;
    for (auto& vec : vIn.GetArray())
        vec3.push_back(vec.GetDouble());
    vRes = Vector3(vec3[0], vec3[1], vec3[2]);
}

camData scanCamera(Value& v)
{
    camData c;
    c.resX = v.FindMember("resX")->value.GetInt();
    c.fovX = v.FindMember("fovX")->value.GetDouble();
    c.fovY = v.FindMember("fovY")->value.GetDouble();
    c.rotAngle = v.FindMember("rotAngle")->value.GetDouble();
    scanVector3(v.FindMember("pos")->value, c.pos);
    scanVector3(v.FindMember("rotAxis")->value, c.rotAxis);
    std::cout << "c.resX: " << c.resX << std::endl;
    std::cout << "c.fovX: " << c.fovX << std::endl;
    std::cout << "c.fovY: " << c.fovY << std::endl;
    std::cout << "c.rotAxis: " << c.rotAxis << std::endl;
    std::cout << "c.rotAngle: " << c.rotAngle << std::endl;
    std::cout << "c.pos: " << c.pos << std::endl;
    return c;
}
 
TEST(rapidJsonTest, RapidJsonTest)
{
    std::string path("/home/stanislas/wdc_workspace/src/ray-tracing/unit/data/cylinderTest.json");
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

    static const char* kTypeNames[] =
        { "Null", "False", "True", "Object", "Array", "String", "Number" };
    for (Value::ConstMemberIterator itr = document.MemberBegin();
        itr != document.MemberEnd(); ++itr)
    {
        std::string name(itr->name.GetString());
        camData cam;
        if(name == "camera")
        {
            cam = scanCamera(document[itr->name.GetString()]);
        }
        printf("Type of member %s is %s\n",
            itr->name.GetString(), kTypeNames[itr->value.GetType()]);
        if(kTypeNames[itr->value.GetType()] == "Object")
        {
            std::cout << "This is an object" << std::endl;
            Value& obj = document[name.c_str()];
            for (Value::ConstMemberIterator itr2 = obj.MemberBegin();
                itr2 != obj.MemberEnd(); ++itr2)
            {
                printf("Type of member %s is %s\n",
                    itr2->name.GetString(), kTypeNames[itr2->value.GetType()]);
            }
        }
    }


    //Value::MemberIterator sceneIt;
    //for(sceneIt = document.MemberBegin(); sceneIt != document.MemberEnd(); sceneIt++)
    //{
    //    std::string sceneName(sceneIt->name.GetString());
    //    std::cout << "sceneName: " << sceneName << std::endl;

    //    Value::MemberIterator elementIt;
    //    for(elementIt = document[sceneName].MemberBegin(); elementIt != document[sceneName].MemberEnd(); elementIt++)
    //    {
    //        std::string elementName(elementIt->name.GetString());
    //        std::cout << "elementName: " << elementName << std::endl;
    //    }
    //}
     
    ASSERT_TRUE(true);
}
