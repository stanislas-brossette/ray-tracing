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

void Scene::addItem(Item* i)
{
  items_.push_back(i);
}

void Scene::addCamera(Camera* i)
{
  cameras_.push_back(i);
}

void Scene::addLightSource(LightSource* i)
{
  lightSources_.push_back(i);
}

void Scene::setAmbiantLight(AmbiantLight* i)
{
  ambiantLight_ = i;
}

std::ostream& operator<<(std::ostream& os, const Scene& s)
{
  os << "++++++++++++++ Scene ++++++++++++++\n";

  os << s.ambiantLight_->describe() << std::endl;

  for(size_t i = 0; i < s.cameras_.size(); ++i)
    os << s.cameras_[i]->describe() << std::endl;

  for(size_t i = 0; i < s.lightSources_.size(); ++i)
    os << s.lightSources_[i]->describe() << std::endl;

  for(size_t i = 0; i < s.items_.size(); ++i)
    os << s.items_[i]->describe() << std::endl;

  os << "+++++++++++++++++++++++++++++++++++";
}
