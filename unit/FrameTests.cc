#include <gtest/gtest.h>
#include <iostream>

#include "Vector3.hh"
#include "Frame3.hh"

TEST(UnitTests, Frame3Test)
{
  Frame3 fWorld;
  Frame3 fTransOnly(Vector3(1,2,3), Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 1));
  Frame3 fRotOnly(Vector3(0,0,0), Vector3(0, 0, -1), Vector3(0, 1, 0), Vector3(1, 0, 0));
  Frame3 fTransRot(Vector3(1,2,3), Vector3(0, 0, -1), Vector3(0, 1, 0), Vector3(1, 0, 0));
  ASSERT_TRUE(fWorld==fWorld);
  ASSERT_FALSE(fWorld==fTransOnly);
  ASSERT_FALSE(fWorld==fRotOnly);

  double prec = 1e-9;
  fWorld.translate(Vector3(1,2,0));
  fWorld.translate(Vector3(0,0,3));
  ASSERT_TRUE(fWorld.isApprox(fTransOnly, prec));

  fWorld.rotate(Vector3(0,1,0), 90);
  ASSERT_TRUE(fWorld.isApprox(fTransRot, prec));

  fWorld = Frame3();
  fWorld.rotate(Vector3(0,1,0), 90);
  ASSERT_TRUE(fWorld.isApprox(fRotOnly, prec));
}


