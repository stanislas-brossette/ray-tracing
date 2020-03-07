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
    Pixel pix, ambiantPix, diffuseRefPix, specReflPix;
    cameras_[camIndex]->castRandomRay(lr, pix);

    Vector3 impactPoint;
    Vector3 impactNormal;
    double impactDist = 1e9;
    size_t impactItemIndex = 0;

    //Does the primary light ray contact with an item?
    bool impact = findFirstImpact(lr, impactItemIndex, impactPoint, impactNormal, impactDist);

    //Compute illumination
    if (impact)
    {
        Item* impactItem = items_[impactItemIndex];

        /*************************************
        *  Direct impact with light source  *
        *************************************/
        if (impactItem->material_->lightEmitter_)
        {
            pix.a_ = int(255*impactItem->material_->lightIntensity_);
            pix.r_ = impactItem->material_->color_.r_;
            pix.g_ = impactItem->material_->color_.g_;
            pix.b_ = impactItem->material_->color_.b_;
            return pix;
        }

        /*******************
        *  Ambiant light  *
        *******************/
        ambiantPix.a_ = int(255*ambiantLight_.alpha_);
        ambiantPix.r_ = impactItem->material_->color_.r_;
        ambiantPix.g_ = impactItem->material_->color_.g_;
        ambiantPix.b_ = impactItem->material_->color_.b_;
        pix = pix + ambiantPix;

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
                    pixIfNotInShadow.r_ = impactItem->material_->color_.r_;
                    pixIfNotInShadow.g_ = impactItem->material_->color_.g_;
                    pixIfNotInShadow.b_ = impactItem->material_->color_.b_;

                    bool inShadow = false;
                    size_t shadowingItemIndex = 0;
                    if(needToCheckShadow)
                    {
                        inShadow = isIntercepted(lrImpactToLightSource, distImpactToLightSource, impactItemIndex);
                        if(not inShadow)
                        {
                            diffuseRefPix = pixIfNotInShadow;
                            pix = pix + diffuseRefPix;
                        }
                    }
                }
            }
        }


        /*************************
        *  specular reflection  *
        *************************/
        if (impactItem->material_->reflectiveness_ > 0)
        {
            // Rugosity can be taken into account here
            Vector3 lrSpecRefDir = lr.dir_.symmetrize(impactNormal);
            lrSpecRefDir.addNoise(impactItem->material_->rugosity_);
            lrSpecRefDir.normalize();
            LightRay lrSpecReflection(impactPoint, lrSpecRefDir);
            Vector3 secImpactPoint;
            Vector3 secImpactNormal;
            double secImpactDist = 1e9;
            size_t secImpactItemIndex = 0;

            //Does the primary light ray contact with an item?
            bool secImpact = findFirstImpact(lrSpecReflection, secImpactItemIndex, secImpactPoint, secImpactNormal, secImpactDist);
            Item* specReflItem = items_[secImpactItemIndex];
            if (secImpact and specReflItem->material_->lightEmitter_)
            {
                double distReductionFactor = 1/std::sqrt(secImpactDist+1);
                specReflPix.x_ = pix.x_;
                specReflPix.y_ = pix.y_;
                specReflPix.a_ = int(255*distReductionFactor*impactItem->material_->reflectiveness_*specReflItem->material_->lightIntensity_);
                specReflPix.r_ = specReflItem->material_->color_.r_;
                specReflPix.g_ = specReflItem->material_->color_.g_;
                specReflPix.b_ = specReflItem->material_->color_.b_;
                pix = pix + specReflPix;
            }
        }
    }

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
