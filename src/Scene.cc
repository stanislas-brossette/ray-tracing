#include "Scene.hh"

Scene::Scene()
  : items_(),
    cameras_(),
    ambiantLight_()
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

void Scene::setAmbiantLight(const AmbiantLight& ambiantLight)
{
  ambiantLight_ = ambiantLight;
}

Pixel Scene::castRandomRay(size_t camIndex) const
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
          if (items_[itemIndex]->material_->lightEmitter_)
          {
              pix.a_ = int(255*items_[itemIndex]->material_->lightIntensity_);
              pix.r_ = items_[itemIndex]->material_->color_.r_;
              pix.g_ = items_[itemIndex]->material_->color_.g_;
              pix.b_ = items_[itemIndex]->material_->color_.b_;
              return pix;
          }
          else
          {
              impact = true;
              impactPoint = tmpPoint;
              impactNormal = tmpNormal;
              impactDist = tmpDist;
              impactItemIndex = itemIndex;
          }
      }
  }
  if (impact)
  {
      /*******************
      *  Ambiant light  *
      *******************/
      pix.a_ = int(255*ambiantLight_.alpha_);
      pix.r_ = items_[impactItemIndex]->material_->color_.r_;
      pix.g_ = items_[impactItemIndex]->material_->color_.g_;
      pix.b_ = items_[impactItemIndex]->material_->color_.b_;

      /************************
      *  diffuse reflection  *
      ************************/
      //Is there a light source in the half plane (impactPoint impactNormal)
      for (size_t itemIndex = 0; itemIndex < items_.size(); itemIndex++)
      {
          Item* lsItem = items_[itemIndex];
          if (lsItem->material_->lightEmitter_)
          {
              double cosAngle = 0;
              bool inHalfSpace = lsItem->isInHalfSpace(impactPoint, impactNormal, cosAngle);
              double distImpactToLightSource = (impactPoint - lsItem->geometry_->f_.o_).norm();
              //Need to check if the diffusion light ray is intercepted by another object, that would cast a shadow
              if(inHalfSpace)
              {
                  double distReductionFactor = 1/std::sqrt(distImpactToLightSource+1);
                  pix.a_ += int(255*cosAngle*distReductionFactor);
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
  if(pix.a_ > 255)
      pix.a_ = 255;
  if(pix.r_ > 255)
      pix.r_ = 255;
  if(pix.g_ > 255)
      pix.g_ = 255;
  if(pix.b_ > 255)
      pix.b_ = 255;
  return pix;
}

std::ostream& operator<<(std::ostream& os, const Scene& s)
{
  os << "++++++++++++++ Scene ++++++++++++++\n";

  os << s.ambiantLight_.describe() << std::endl;

  for(size_t camIndex = 0; camIndex < s.cameras_.size(); ++camIndex)
    os << s.cameras_[camIndex]->describe() << std::endl;

  for(size_t itemIndex = 0; itemIndex < s.items_.size(); ++itemIndex)
    os << s.items_[itemIndex]->describe() << std::endl;

  os << "+++++++++++++++++++++++++++++++++++";
}
