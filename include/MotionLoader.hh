#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

#include "Vector3.hh"

using namespace rapidjson;

struct pose
{
    std::string bodyName;
    Vector3 pos;
    Vector3 vx;
    Vector3 vy;
    Vector3 vz;
};

class MotionLoader
{
public:
    MotionLoader ();
    MotionLoader (const std::string& path);
    virtual ~MotionLoader ();
    void load(const std::string& path);
    void scanVector3(Value& vIn, Vector3& vRes);
    Vector3 get(int frameId, const std::string& bodyName, const std::string& valueName);

private:
    Document document;



};
