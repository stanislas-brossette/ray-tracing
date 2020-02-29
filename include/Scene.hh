#pragma once
#include <iostream>
#include <vector>

#include "Vector3.hh"
#include "Item.hh"
#include "LightSource.hh"
#include "AmbiantLight.hh"
#include "Camera.hh"

class Scene
{
public:
  Scene ();
  void addItem(Item* i);
  void addCamera(Camera* c);
  void addLightSource(LightSource* ls);
  void setAmbiantLight(AmbiantLight* al);
  virtual ~Scene ();
  friend std::ostream& operator<<(std::ostream& os, const Scene& s);

  AmbiantLight* ambiantLight_;
  std::vector<Item*> items_;
  std::vector<Camera*> cameras_;
  std::vector<LightSource*> lightSources_;
};
