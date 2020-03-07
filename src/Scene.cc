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

    //Does the primary light ray contact with an item?
    bool impact = findFirstImpact(lr, impactItemIndex, impactPoint, impactNormal, impactDist);


    //Compute illumination
    if (impact)
    {
        /*************************************
        *  Direct impact with light source  *
        *************************************/
        if (items_[impactItemIndex]->material_->lightEmitter_)
        {
            pix.a_ = int(255*items_[impactItemIndex]->material_->lightIntensity_);
            pix.r_ = items_[impactItemIndex]->material_->color_.r_;
            pix.g_ = items_[impactItemIndex]->material_->color_.g_;
            pix.b_ = items_[impactItemIndex]->material_->color_.b_;
            return pix;
        }
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
        Pixel pixIfNotInShadow;
        bool needToCheckShadow = false;
        LightRay lrImpactToLightSource;
        double distImpactToLightSource;
        for (size_t itemIndex = 0; itemIndex < items_.size(); itemIndex++)
        {
            Item* lsItem = items_[itemIndex];
            if (lsItem->material_->lightEmitter_)
            {
                double cosAngle = 0;
                bool inHalfSpace = lsItem->isInHalfSpace(impactPoint, impactNormal, cosAngle);
                distImpactToLightSource = (impactPoint - lsItem->geometry_->f_.o_).norm();
                lrImpactToLightSource.origin_ = impactPoint;
                lrImpactToLightSource.dir_ = lsItem->geometry_->f_.o_ - impactPoint;
                lrImpactToLightSource.dir_.normalize();

                //Need to check if the diffusion light ray is intercepted by another object, that would cast a shadow
                if(inHalfSpace)
                {
                    needToCheckShadow = true;
                    //double distReductionFactor = 1/std::sqrt(distImpactToLightSource+1);
                    double distReductionFactor = 1/(distImpactToLightSource+1);
                    //double distReductionFactor = 1/std::pow(distImpactToLightSource+1,2);
                    pixIfNotInShadow.a_ = int(255*cosAngle*distReductionFactor);
                    pixIfNotInShadow.r_ = items_[impactItemIndex]->material_->color_.r_;
                    pixIfNotInShadow.g_ = items_[impactItemIndex]->material_->color_.g_;
                    pixIfNotInShadow.b_ = items_[impactItemIndex]->material_->color_.b_;

                    bool inShadow = false;
                    size_t shadowingItemIndex = 0;
                    if(needToCheckShadow)
                    {
                        inShadow = isIntercepted(lrImpactToLightSource, distImpactToLightSource, impactItemIndex);
                        if(not inShadow)
                        {
                            pix.a_ += pixIfNotInShadow.a_;
                            pix.r_ = pixIfNotInShadow.r_;
                            pix.g_ = pixIfNotInShadow.g_;
                            pix.b_ = pixIfNotInShadow.b_;
                        }
                    }
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

    pix.clamp();
    return pix;
}

bool Scene::isIntercepted(const LightRay& lrImpactToLightSource, double distImpactToLightSource, size_t impactItemIndex) const
{
    bool inShadow = false;
    size_t shadowingItemIndex = 0;
    for (size_t shadItemIndex = 0; shadItemIndex < items_.size(); shadItemIndex++)
    {
        Vector3 tmpPoint;
        Vector3 tmpNormal;
        double tmpDist;
        Item* shadowingItem = items_[shadItemIndex];
        if((not shadowingItem->material_->lightEmitter_) and (not (shadItemIndex == impactItemIndex)))
        {
            bool tmpImpact = shadowingItem->intersect(lrImpactToLightSource, tmpPoint, tmpNormal, tmpDist);
            if (tmpImpact && tmpDist < distImpactToLightSource)
            {
                shadowingItemIndex = shadItemIndex;
                inShadow = true;
                break;
            }
        }
    }
    return inShadow;
}

bool Scene::findFirstImpact(const LightRay& lr, size_t& impactItemIndex, Vector3& impactPoint, Vector3& impactNormal, double& impactDist) const
{
    bool impact = false;
    for (size_t itemIndex = 0; itemIndex < items_.size(); itemIndex++)
    {
        Vector3 tmpPoint;
        Vector3 tmpNormal;
        double tmpDist;
        bool tmpImpact = items_[itemIndex]->intersect(lr, tmpPoint, tmpNormal, tmpDist);
        if (tmpImpact && tmpDist < impactDist)
        {
            impactItemIndex = itemIndex;
            impact = true;
            impactPoint = tmpPoint;
            impactNormal = tmpNormal;
            impactDist = tmpDist;
            impactItemIndex = itemIndex;
        }
    }
    return impact;
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
