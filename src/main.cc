#include <iostream>
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "LightSource.hh"
#include "Camera.hh"

int main(void)
{
  Scene myScene;
  AmbiantLight al(0.2, Vector3RGB(255,257,2));
  LightSource ls0(Vector3(10,10,10), 0.6, Vector3RGB(23,25,36));
  Sphere s0(Vector3(0,0,0),2);
  Sphere s1(Vector3(1.01,1.2,1.1),0.9);
  myScene.setAmbiantLight(&al);
  myScene.addItem(&s0);
  myScene.addItem(&s1);
  myScene.addLightSource(&ls0);

  std::cout << myScene << std::endl;
  return 0;
}
