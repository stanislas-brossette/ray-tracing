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
  for (size_t itemIndex = 0; itemIndex < items_.size(); itemIndex++)
  {
    contact = false;
    distance = 0.0;

    pixels[itemIndex] = items_[itemIndex]->intersect(lr, contact, distance);
    if(contact)
    {
      std::cout << "Contact with item " << itemIndex << std::endl;
      std::cout << lr.describe() << std::endl;
      std::cout << "dist: " << distance << std::endl;
    }

    if(contact && distance < minContactDist)
    {
      minContactDist = distance;
      minContactDistIndex = itemIndex;
      std::cout << "minContactDist: " << minContactDist << std::endl;
      std::cout << "minContactDistIndex: " << minContactDistIndex << std::endl;
    }
  }
  return pixels[minContactDistIndex];
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
