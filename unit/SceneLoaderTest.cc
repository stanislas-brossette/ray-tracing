#include <gtest/gtest.h>
#include "SceneLoader.hh"

TEST(sceneLoaderTest, SceneLoaderTest)
{
    std::string path = std::string(UNIT_DATA) + "cylinderTest.json";
    SceneLoader sceneLoader;
    SceneData sData = sceneLoader.load(path);
    std::cout << sData.describe();
    ASSERT_TRUE(true);
}
