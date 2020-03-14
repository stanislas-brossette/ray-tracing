#include <gtest/gtest.h>
#include "SceneLoader.hh"

TEST(sceneLoaderTest, SceneLoaderTest)
{
    std::string path("/home/stanislas/profiles/devel/src/raytracing/unit/data/cylinderTest.json");
    SceneLoader sceneLoader;
    sceneLoader.load(path);
    ASSERT_TRUE(true);
}
