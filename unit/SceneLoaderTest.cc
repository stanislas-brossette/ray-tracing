#include <gtest/gtest.h>
#include "SceneLoader.hh"

TEST(sceneLoaderTest, SceneLoaderTest)
{
    std::string path("/home/stanislas/profiles/devel/src/raytracing/unit/data/cylinderTest.json");
    SceneLoader sceneLoader;
    sceneData sData = sceneLoader.load(path);
    std::cout << sData.describe();
    ASSERT_TRUE(true);
}
