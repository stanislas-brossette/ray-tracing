#include <gtest/gtest.h>
#include <iostream>

#include "Vector3.hh"
#include "Frame3.hh"
#include "Sphere.hh"
#include "LightRay.hh"

TEST(UnitTests, SphereTest)
{
    double prec = 1e-9;
    Frame3 f;
    f.translate(Vector3(0,3,0));

    Sphere s(f,1);

    Vector3 impactPoint(0,0,0);
    Vector3 normal(0,0,0);
    double dist(0);
    bool impact(false);

    LightRay lr(Vector3(0,0,0), Vector3(0,1,0));
    impact = s.intersect(lr, impactPoint, normal, dist);
    ASSERT_TRUE(impact);
    ASSERT_TRUE(impactPoint.isApprox(Vector3(0, 2, 0), prec));
    ASSERT_TRUE(normal.isApprox(Vector3(0, -1, 0), prec));
    ASSERT_TRUE(std::abs(dist - 2) < prec);

    lr = LightRay(Vector3(0,0,0), Vector3(0,-1,0));
    impact = s.intersect(lr, impactPoint, normal, dist);
    ASSERT_FALSE(impact);

    lr = LightRay(Vector3(0,6,0), Vector3(0,-1,0));
    impact = s.intersect(lr, impactPoint, normal, dist);
    ASSERT_TRUE(impact);
    ASSERT_TRUE(impactPoint.isApprox(Vector3(0, 4, 0), prec));
    ASSERT_TRUE(normal.isApprox(Vector3(0, 1, 0), prec));
    ASSERT_TRUE(std::abs(dist - 2) < prec);

    Vector3 dir(0, 1, 1);
    dir.normalize();
    lr = LightRay(Vector3(0,0,0), dir);
    impact = s.intersect(lr, impactPoint, normal, dist);
    ASSERT_FALSE(impact);

    dir = Vector3(0, 3, 1);
    dir.normalize();
    lr = LightRay(Vector3(0,0,0), dir);
    impact = s.intersect(lr, impactPoint, normal, dist);
    Vector3 expectedImpactPoint(0, 2.4, 0.8);
    Vector3 expectedNormal(0, -0.6, 0.8);
    double expectedDist = expectedImpactPoint.norm();
    ASSERT_TRUE(impactPoint.isApprox(expectedImpactPoint, prec));
    ASSERT_TRUE(normal.isApprox(expectedNormal, prec));
    ASSERT_TRUE(std::abs(dist - expectedDist) < prec);
}
