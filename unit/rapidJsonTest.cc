#include <gtest/gtest.h>
// rapidjson/example/simpledom/simpledom.cpp`
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <cstdio>
 
TEST(rapidJsonTest, RapidJsonTest)
{
    std::string path("/home/stanislas/wdc_workspace/src/ray-tracing/unit/data/cylinderTest.json");
    FILE* fp = fopen(path.c_str(), "r");
 
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    std::cout << "readBuffer: " << readBuffer << std::endl;
     
    rapidjson::Document d;
    d.Parse(readBuffer);
    std::cout << "d.IsObject(): " << d.IsObject() << std::endl;
    fclose(fp);

    if(d.HasParseError())
    {
        std::cout << "ERROR: Json has errors" << std::endl;
    }

    rapidjson::Value::MemberIterator sceneIt;
    for(sceneIt = d.MemberBegin(); sceneIt != d.MemberEnd(); sceneIt++)
    {
        std::string sceneName(sceneIt->name.GetString());
        std::cout << "sceneName: " << sceneName << std::endl;

        rapidjson::Value::MemberIterator elementIt;
        for(elementIt = d[sceneName].MemberBegin(); elementIt != d[sceneName].MemberEnd(); elementIt++)
        {
            std::string elementName(elementIt->name.GetString());
            std::cout << "elementName: " << elementName << std::endl;
        }
    }
     
    ASSERT_TRUE(true);
}
