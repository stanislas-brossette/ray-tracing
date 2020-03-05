#include "Scene.hh"

Scene::Scene()
  : items_(),
    cameras_()
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

void Scene::setAmbiantLight(AmbiantLight* ambiantLight)
{
  ambiantLight_ = ambiantLight;
}

Pixel Scene::castRandomRay(size_t camIndex)
{
  LightRay lr;
  Pixel pix;
  cameras_[camIndex]->castRandomRay(lr, pix);
  pix.a_ = 0;
  pix.r_ = 0;
  pix.g_ = 0;
  pix.b_ = 0;

  Vector3 impactPoint;
  Vector3 impactNormal;
  double impactDist = 1e9;
  size_t impactItemIndex = 0;
  bool impact = false;

  //Does the primary light ray contact with an item?
  for (size_t itemIndex = 0; itemIndex < items_.size(); itemIndex++)
  {
      Vector3 tmpPoint;
      Vector3 tmpNormal;
      double tmpDist;
      bool tmpImpact = items_[itemIndex]->intersect(lr, tmpPoint, tmpNormal, tmpDist);
      if (tmpImpact && tmpDist < impactDist)
      {
          impact = true;
          impactPoint = tmpPoint;
          impactNormal = tmpNormal;
          impactDist = tmpDist;
          impactItemIndex = itemIndex;
      }
  }
  if (impact)
  {
      /*******************
      *  Ambiant light  *
      *******************/
      pix.a_ = int(255*0.01);
      pix.r_ = items_[impactItemIndex]->material_->color_.r_;
      pix.g_ = items_[impactItemIndex]->material_->color_.g_;
      pix.b_ = items_[impactItemIndex]->material_->color_.b_;

      /************************
      *  diffuse reflection  *
      ************************/
      //Is there a light source in the half plane (impactPoint impactNormal)
      for (size_t itemIndex = 0; itemIndex < items_.size(); itemIndex++)
      {
          Item* item = items_[itemIndex];
          if (item->material_->lightEmitter_)
          {
              double cosAngle = 0;
              bool inHalfSpace = item->isInHalfSpace(impactPoint, impactNormal, cosAngle);
              if(inHalfSpace)
              {
                  pix.a_ = int(255*cosAngle);
                  pix.r_ = items_[impactItemIndex]->material_->color_.r_;
                  pix.g_ = items_[impactItemIndex]->material_->color_.g_;
                  pix.b_ = items_[impactItemIndex]->material_->color_.b_;
              }
          }
      }



      /*************************
      *  specular reflection  *
      *************************/
      //What is next in the path of the light ray (diffuse reflection from object or light source

      //pix.a_ = 1.0;
      //pix.r_ = items_[impactItemIndex]->material_->color_.r_;
      //pix.g_ = items_[impactItemIndex]->material_->color_.g_;
      //pix.b_ = items_[impactItemIndex]->material_->color_.b_;
  }
  return pix;
}

std::ostream& operator<<(std::ostream& os, const Scene& s)
{
  os << "++++++++++++++ Scene ++++++++++++++\n";

  os << s.ambiantLight_->describe() << std::endl;

  for(size_t camIndex = 0; camIndex < s.cameras_.size(); ++camIndex)
    os << s.cameras_[camIndex]->describe() << std::endl;

  for(size_t itemIndex = 0; itemIndex < s.items_.size(); ++itemIndex)
    os << s.items_[itemIndex]->describe() << std::endl;

  os << "+++++++++++++++++++++++++++++++++++";
}
