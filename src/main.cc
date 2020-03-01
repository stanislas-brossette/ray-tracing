#include <iostream>
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "LightSource.hh"
#include "Camera.hh"
#include "Window.hh"

int main(void)
{
  Window myWindow(500, 500);
  Scene myScene;

  Camera cam(Vector3(0,0,0), Vector3(0,1,0), 10, 10);

  AmbiantLight al(0.2, Vector3RGB(255,255,255));

  LightSource ls0(Vector3(0,0,1), 0.6, Vector3RGB(255, 255, 255));

  Sphere s0(Vector3(0, 3, 0), 0.5, Vector3RGB(0, 255, 0));
  Sphere s1(Vector3(0.5, 2, 0.5), 0.5, Vector3RGB(0, 0, 255));

  myScene.addCamera(&cam);
  myScene.setAmbiantLight(&al);
  myScene.addLightSource(&ls0);
  myScene.addItem(&s0);
  myScene.addItem(&s1);

  std::cout << myScene << std::endl;

  int iter = 0;
  while(iter < 100)
  {
    myScene.castRay(0);
    myWindow.render();
    std::cout << iter << std::endl;
    iter++;
  }
  return 0;
}
