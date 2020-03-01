#include "Scene.hh"

Scene::Scene()
  : items_(),
    cameras_(),
    lightSources_()
{
}

Scene::~Scene()
{
}

void Scene::addItem(Item* item)
{
  items_.push_back(item);
}

void Scene::addCamera(Camera* cam)
{
  cameras_.push_back(cam);
}

void Scene::addLightSource(LightSource* lightSource)
{
  lightSources_.push_back(lightSource);
}

void Scene::setAmbiantLight(AmbiantLight* ambiantLight)
{
  ambiantLight_ = ambiantLight;
}

Pixel Scene::castRay(size_t camIndex)
{
  LightRay lr = cameras_[camIndex]->castRandomRay();

  bool contact = false;
  double distance = 0.0;

  std::vector<Pixel> pixels(items_.size());

  double minContactDist = 999999999999;
  size_t minContactDistIndex = 0;
  LightRay minContactDistLr2;
  Pixel minContactPixel;
  bool minContact = false;

  //Does the primary light ray contact with an item?
  for (size_t itemIndex = 0; itemIndex < items_.size(); itemIndex++)
  {
    contact = false;
    distance = 0.0;

    LightRay lr2;

    pixels[itemIndex] = items_[itemIndex]->intersect(lr, lr2, contact, distance);

    if(contact && distance < minContactDist)
    {
      minContact = contact;
      minContactDist = distance;
      minContactDistIndex = itemIndex;
      minContactDistLr2 = lr2;
      minContactPixel = pixels[itemIndex];
    }
  }

  //Does the secondary light ray contact with a light source?
  if(minContact)
  {
    Pixel pixelLightSource;
    bool touchedLightSource;
    for (size_t lsIndex = 0; lsIndex < lightSources_.size(); lsIndex++)
    {
      Pixel tmpPixel = lightSources_[lsIndex]->intersect(minContactDistLr2, contact, distance);
      if(contact)
      {
        minContactPixel.r_ = 255;
        minContactPixel.g_ = 255;
        minContactPixel.b_ = 255;
      }
    }
  }

  return minContactPixel;
}

std::ostream& operator<<(std::ostream& os, const Scene& s)
{
  os << "++++++++++++++ Scene ++++++++++++++\n";

  os << s.ambiantLight_->describe() << std::endl;

  for(size_t camIndex = 0; camIndex < s.cameras_.size(); ++camIndex)
    os << s.cameras_[camIndex]->describe() << std::endl;

  for(size_t lsIndex = 0; lsIndex < s.lightSources_.size(); ++lsIndex)
    os << s.lightSources_[lsIndex]->describe() << std::endl;

  for(size_t itemIndex = 0; itemIndex < s.items_.size(); ++itemIndex)
    os << s.items_[itemIndex]->describe() << std::endl;

  os << "+++++++++++++++++++++++++++++++++++";
}
