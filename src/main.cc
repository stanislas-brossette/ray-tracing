#include <iostream>
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "LightSource.hh"
#include "Camera.hh"
#include "Window.hh"

int main(void)
{
  int resX = 500;
  int resY = 500;
  Window myWindow(resX, resY);
  Scene myScene;

  Camera cam(Vector3(0,0,0), Vector3(0,1,0), 120, 120, resX, resY);

  AmbiantLight al(0.2, Vector3RGB(255,255,255));

  LightSource ls0(Vector3(4,-2,4), 0.6, 1.2, Vector3RGB(255, 0, 0));

  double rugosity = 0.0002;

  Sphere s(Vector3(0.0, 3.0, 0.0), 1.5, Vector3RGB(0, 255, 0), rugosity);
  Sphere s0(Vector3(0.01, 1.99, 0.1), 0.5, Vector3RGB(0, 255, 0), rugosity);
  Sphere s1(Vector3(-1, 2.02, 0.5), 0.7, Vector3RGB(0, 0, 255), rugosity);
  Sphere s2(Vector3(-0.02, 2, -1.25), 0.5, Vector3RGB(255, 0, 0), rugosity);
  Sphere s3(Vector3(1.0, 4, 2.75), 0.7, Vector3RGB(120, 120, 0), rugosity);
  Sphere s4(Vector3(1.0, 3, -2.0), 0.5, Vector3RGB(0, 120, 120), rugosity);

  myScene.addCamera(&cam);
  myScene.setAmbiantLight(&al);
  myScene.addLightSource(&ls0);
  myScene.addItem(&s);
  myScene.addItem(&s0);
  myScene.addItem(&s1);
  myScene.addItem(&s2);
  myScene.addItem(&s3);
  myScene.addItem(&s4);

  std::cout << myScene << std::endl;

  int iter = 0;
  while(iter < 400000)
  {
    Pixel p = myScene.castRay(0);
    myWindow.render(p);
    if(iter % 1000 ==0)
      std::cout << iter << std::endl;
    iter++;
  }
  return 0;
}
