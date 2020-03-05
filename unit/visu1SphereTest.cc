#include <gtest/gtest.h>
#include <iostream>
#include "Vector3.hh"
#include "Frame3.hh"
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "Camera.hh"
#include "Window.hh"

void testRender(const Scene& myScene, Window& myWindow, const std::string& s)
{
  myWindow.clear();
  int iter = 0;
  while(iter < 8000000)
  {
    Pixel p = myScene.castRandomRay(0);
    myWindow.addPixel(p);
    if(iter % 100000 ==0)
        myWindow.render();
    iter++;
  }

  std::cout << s;
  char answer;
  std::cin >> answer;
  ASSERT_TRUE(answer == 'y');
}

TEST(ManoTests, Visu1SpheresTest)
{
  int resX = 1000;
  int fovX = 100;
  int fovY = 100;
  Scene myScene;
  Frame3 f;

  Camera cam(f, fovX, fovY, resX);

  Window myWindow(cam.resX_, cam.resY_);

  AmbiantLight al(0.02, Vector3RGB(255,255,255));

  Material mls(Vector3RGB(255, 255, 255), 0, 0, true, 1);
  Material ms0(Vector3RGB(255, 0, 0), 0, 0, false, 0);

  Frame3 fls, fs0;
  fs0.translate( 0.0, 2.0, 0.0);

  Sphere gsls(fls, 0.1);
  Sphere gs0(fs0, 0.5);

  Item ls(&gsls, &mls);
  Item s0(&gs0, &ms0);

  myScene.addCamera(&cam);
  myScene.setAmbiantLight(al);
  myScene.addItem(&ls);
  myScene.addItem(&s0);

  ls.geometry_->f_.setOriginPos( 0.0, 1.3, 1.0);
  s0.material_->color_ = Vector3RGB(255, 0, 0);
  testRender(myScene, myWindow, "You should see 1 red sphere lit from the top. Correct [y/n]?");

  ls.geometry_->f_.setOriginPos( 0.0, 1.3, -1.0);
  s0.material_->color_ = Vector3RGB(0, 255, 0);
  testRender(myScene, myWindow, "You should see 1 green sphere lit from the bottom. Correct? [y/n]");

  ls.geometry_->f_.setOriginPos( 1.0, 1.3, 0.0);
  s0.material_->color_ = Vector3RGB(0, 0, 255);
  testRender(myScene, myWindow, "You should see 1 blue sphere lit from the right. Correct? [y/n]");

  ls.geometry_->f_.setOriginPos( -1.0, 1.3, 0.0);
  s0.material_->color_ = Vector3RGB(255, 20, 150);
  testRender(myScene, myWindow, "You should see 1 pink sphere lit from the left. Correct? [y/n]");
}

