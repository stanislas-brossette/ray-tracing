#include <iostream>
#include "Vector3.hh"
#include "Frame3.hh"
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "Camera.hh"
#include "Window.hh"

int main(void)
{
  int resX = 1000;
  int fovX = 160;
  int fovY = 160;
  Scene myScene;
  Frame3 f;

  Camera cam(f, fovX, fovY, resX);

  Window myWindow(cam.resX_, cam.resY_);

  AmbiantLight al(0.2, Vector3RGB(255,255,255));

  Material mls(Vector3RGB(255, 255, 255), 0, 0, true, 1);
  Material ms0(Vector3RGB(255, 0, 0), 0, 0, false, 0);
  Material ms1(Vector3RGB(0, 255, 0), 0, 0, false, 0);
  Material ms2(Vector3RGB(0, 0, 255), 0, 0, false, 0);

  Frame3 fls, fs0, fs1, fs2;
  fls.translate(4, -2, 4);
  fs0.translate(0.01, 1.99, 0.1);
  fs1.translate(-1, 2.02, 0.5);
  fs2.translate(-0.02, 2, -1.25);

  Sphere gsls(fls, 1.5);
  Sphere gs0(fs0, 1.5);
  Sphere gs1(fs1, 0.5);
  Sphere gs2(fs2, 0.7);

  Item ls(&gsls, &mls);
  Item s0(&gs0, &ms0);
  Item s1(&gs1, &ms1);
  Item s2(&gs2, &ms2);

  myScene.addCamera(&cam);
  myScene.setAmbiantLight(&al);
  myScene.addItem(&ls);
  myScene.addItem(&s0);
  myScene.addItem(&s1);
  myScene.addItem(&s2);

  std::cout << myScene << std::endl;

  int iter = 0;
  while(iter < 10000000)
  {
    Pixel p = myScene.castRandomRay(0);
    myWindow.addPixel(p);
    if(iter % 100000 ==0)
        myWindow.render();
    iter++;
  }
  return 0;
}
