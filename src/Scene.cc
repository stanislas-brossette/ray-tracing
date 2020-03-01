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
  std::cout << "Casting Ray: " << lr.describe() << std::endl;
  bool contact = false;
  for (size_t itemIndex = 0; itemIndex < items_.size(); itemIndex++)
  {
    std::cout << "Handling item " << itemIndex << std::endl;
    std::cout << items_[itemIndex]->describe() << std::endl;
    contact = contact or items_[itemIndex]->intersect(lr);
  }
  if(contact)
  {
    std::cout << "INTERSECT" << std::endl;
    Pixel p(lr.px_,lr.py_,1,255,0,0);
    return p;
  }
  else
  {
    std::cout << "NOPE" << std::endl;
    Pixel p(lr.px_,lr.py_,1,0,255,0);
    return p;
  }
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
