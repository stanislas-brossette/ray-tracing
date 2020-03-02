#include <gtest/gtest.h>

#include "Vector3.hh"

TEST(UnitTests, Vector3Test)
{
    Vector3 v0(0, 1, 2);
    Vector3 v1(1, -1, 2);

    ASSERT_TRUE(v0 == v0);
    ASSERT_FALSE(v0 == v1);

    double prec = 1e-9;
    ASSERT_TRUE(v0.isApprox(v0, prec));
    ASSERT_FALSE(v0.isApprox(v1, prec));

    ASSERT_EQ(v0, Vector3(0, 1, 2));
    ASSERT_EQ(v0 + v1, Vector3(1, 0, 4));
    ASSERT_EQ(v0 - v1, Vector3(-1, 2, 0));
    ASSERT_EQ(v0.dot(v1), 3);
    ASSERT_EQ(v0.norm(), std::sqrt(5));
    ASSERT_EQ(v0*3, Vector3(0,3,6));

    v0.normalize();
    double r5 = std::sqrt(5);
    ASSERT_EQ(v0, Vector3(0, 1/r5, 2/r5));
}

TEST(UnitTests, Vector3RotationTest)
{
    double prec = 1e-9;

    Vector3 v0(1, 0, 0);
    Vector3 vRX = v0.rotateX(90);
    ASSERT_TRUE(vRX.isApprox(Vector3(1, 0, 0), prec));
    Vector3 vRY = v0.rotateY(90);
    ASSERT_TRUE(vRY.isApprox(Vector3(0, 0, -1), prec));
    Vector3 vRZ = v0.rotateZ(90);
    ASSERT_TRUE(vRZ.isApprox(Vector3(0, 1, 0), prec));

    v0 = Vector3(0, 1, 0);
    vRX = v0.rotateX(180);
    ASSERT_TRUE(vRX.isApprox(Vector3(0, -1, 0), prec));
    vRY = v0.rotateY(180);
    ASSERT_TRUE(vRY.isApprox(Vector3(0, 1, 0), prec));
    vRZ = v0.rotateZ(180);
    ASSERT_TRUE(vRZ.isApprox(Vector3(0, -1, 0), prec));

    v0 = Vector3(0, 0, 1);
    vRX = v0.rotateX(-90);
    ASSERT_TRUE(vRX.isApprox(Vector3(0, 1, 0), prec));
    vRY = v0.rotateY(-90);
    ASSERT_TRUE(vRY.isApprox(Vector3(-1, 0, 0), prec));
    vRZ = v0.rotateZ(-90);
    ASSERT_TRUE(vRZ.isApprox(Vector3(0, 0, 1), prec));
}
