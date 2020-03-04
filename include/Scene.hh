#pragma once
#include <iostream>
#include <vector>

#include "Vector3.hh"
//#include "Item.hh"
//#include "LightSource.hh"
#include "AmbiantLight.hh"
#include "Camera.hh"
#include "Pixel.hh"

class Scene
{
public:
  Scene ();
  virtual ~Scene ();
  //void addItem(Item* i);
  void addCamera(Camera* c);
  //void addLightSource(LightSource* ls);
  void setAmbiantLight(AmbiantLight* al);
  Pixel castRay(size_t camIndex);

  AmbiantLight* ambiantLight_;
  //std::vector<Item*> items_;
  std::vector<Camera*> cameras_;
  //std::vector<LightSource*> lightSources_;

  friend std::ostream& operator<<(std::ostream& os, const Scene& s);
};
