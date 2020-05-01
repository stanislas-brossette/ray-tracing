#include "MotionLoader.hh"
#include "StringException.hh"

using namespace rapidjson;

MotionLoader::MotionLoader()
{
}

MotionLoader::MotionLoader(const std::string& path)
{
    load(path);
}

MotionLoader::~MotionLoader()
{
}

void MotionLoader::scanVector3(Value& vIn, Vector3& vRes)
{
    std::vector<double> vec3;
    for (auto& vec : vIn.GetArray())
        vec3.push_back(vec.GetDouble());
    vRes = Vector3(vec3[0], vec3[1], vec3[2]);
}

void MotionLoader::load(const std::string& path)
{
    FILE* fp = fopen(path.c_str(), "r");
    char readBuffer[1055536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    document.Parse(readBuffer);
    fclose(fp);
    if(document.HasParseError())
    {
        StringException error("ERROR: Json file " + path + " has errors");
        throw error;
    }
    else
        std::cout << "Correctly loaded json motion" << std::endl;
    //std::cout << "MotionLoader::load" << std::endl;
    //std::vector<std::string> bodies{"PelvisLink", "LeftFrontalHipLink", "LeftTransverseHipLink", "LeftSagittalHipLink", "LeftSagittalKneeLink", "LeftSagittalAnkleLink", "LeftHenkeAnkleLink","RightFrontalHipLink", "RightTransverseHipLink", "RightSagittalHipLink", "RightSagittalKneeLink", "RightSagittalAnkleLink", "RightHenkeAnkleLink"};

    //for (int frameId = 0; frameId < 10; frameId++)
    //{
    //    //std::cout << "frameStr: " << frameStr << std::endl;
    //    for (size_t bodyId = 0; bodyId < bodies.size(); bodyId++)
    //    {
    //        std::string frameStr("frame" + std::to_string(frameId));
    //        Value& vIn = document.FindMember(frameStr.c_str())->value;
    //        std::cout << "frameStr.c_str(): " << frameStr.c_str() << std::endl;
    //        get(frameId, bodies[bodyId], "pos");
    //        get(frameId, bodies[bodyId], "vx");
    //        get(frameId, bodies[bodyId], "vy");
    //        get(frameId, bodies[bodyId], "vz");
    //        //std::cout << "bodies[bodyId]: " << bodies[bodyId] << std::endl;
    //        //Value& bIn = vIn.FindMember(bodies[bodyId].c_str())->value;
    //        //Vector3 pos, vx, vy, vz;
    //        //scanVector3(bIn.FindMember("pos")->value, pos);
    //        //scanVector3(bIn.FindMember("vx")->value, vx);
    //        //scanVector3(bIn.FindMember("vy")->value, vy);
    //        //scanVector3(bIn.FindMember("vz")->value, vz);

    //        //std::cout << "bodies[bodyId]: " << bodies[bodyId] << std::endl;
    //        //std::cout << "pos: " << pos << std::endl;
    //        //std::cout << "vx: " << vx << std::endl;
    //        //std::cout << "vy: " << vy << std::endl;
    //        //std::cout << "vz: " << vz << std::endl;
    //    }
    //}
}

Vector3 MotionLoader::get(int frameId, const std::string& bodyName, const std::string& valueName)
{
    //std::cout << "MotionLoader::get" << std::endl;
    std::string frameStr("frame" + std::to_string(frameId));
    //std::cout << "frameStr.c_str(): " << frameStr.c_str() << std::endl;
    Value& vIn = document.FindMember(frameStr.c_str())->value;
    //std::cout << "bodyName: " << bodyName << std::endl;
    Value& bIn = vIn.FindMember(bodyName.c_str())->value;
    Vector3 val;
    //std::cout << "valueName: " << valueName << std::endl;
    scanVector3(bIn.FindMember(valueName.c_str())->value, val);
    return val;
}
