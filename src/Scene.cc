#include "Scene.hh"

Scene::Scene()
  : items_(),
    camera_(),
    ambiantLight_()
{
}

Scene::Scene(const SceneData& sData)
  : items_(),
    camera_(sData.cData),
    ambiantLight_(sData.aData)
{
    for (size_t i = 0; i < sData.itemsData.size(); i++)
    {
      items_.push_back(new Item(sData.itemsData.at(i)));
    }
}

Scene::~Scene()
{
}

void Scene::renderSerial(std::vector<Pixel>& res, const size_t& nPoints, size_t nPointsRendered) const
{
    for (size_t i = 0; i < nPoints; i++)
    {
        castRandomRayInPlace( res[i], nPointsRendered + i);
    }
}

void Scene::renderParallel(std::vector<Pixel>& res, const size_t& nPixToCompute, size_t nPointsRendered) const
{
    int nPix = res.size();
    int numThreads = std::thread::hardware_concurrency();

    std::vector<int> pixPerThread(numThreads);
    for (size_t i = 0; i < numThreads; i++)
    {
        pixPerThread[i] = nPix/numThreads;
    }
    pixPerThread[0] += nPix%numThreads;

    std::vector<std::thread*> pool(numThreads, nullptr);

    size_t index = 0;
    for (size_t i = 0; i < numThreads; i++)
    {
        std::vector<Pixel> threadPix(pixPerThread[i]);
        size_t beginIndex = index;
        size_t endIndex = index + pixPerThread[i];
        pool[i] = new std::thread(&Scene::castMultipleRandomRaysInPlace, this, std::ref(res), beginIndex, endIndex, nPointsRendered);
        index += pixPerThread[i];
    }
    for (size_t i = 0; i < numThreads; i++)
    {
        pool[i]->join();
        delete pool[i];
    }
}

void Scene::addItem(Item* item)
{
    items_.push_back(item);
}

void Scene::setCamera(const Camera& cam)
{
    camera_ = cam;
}

void Scene::setAmbiantLight(const AmbiantLight& ambiantLight)
{
    ambiantLight_ = ambiantLight;
}

void Scene::castMultipleRandomRaysInPlace( std::vector<Pixel>& vecPix, size_t beginIndex, size_t endIndex, size_t nRenderedPixels) const
{
    for (size_t i = beginIndex; i < endIndex; i++)
    {
        castRandomRayInPlace( vecPix[i], i + nRenderedPixels);
    }
}

void Scene::castRandomRayInPlace( Pixel& pix, size_t iOrderedRay) const
{
    LightRay lr;
    Pixel ambiantPix, diffuseRefPix, specReflPix;
    camera_.castOrderedRay(lr, pix, iOrderedRay);

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
            pix.a_ = 255*impactItem->material_->lightIntensity_;
            pix.r_ = impactItem->material_->color_.r_;
            pix.g_ = impactItem->material_->color_.g_;
            pix.b_ = impactItem->material_->color_.b_;
            return;
        }

        /*******************
        *  Ambiant light  *
        *******************/
        ambiantPix.a_ = 255*ambiantLight_.intensity_;
        ambiantPix.r_ = impactItem->material_->color_.r_;
        ambiantPix.g_ = impactItem->material_->color_.g_;
        ambiantPix.b_ = impactItem->material_->color_.b_;

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
            double cosAngle = 0;
            bool inHalfSpace = lsItem->isInHalfSpace(impactPoint, impactNormal, cosAngle);
            if (lsItem->material_->lightEmitter_)
            {
                distImpactToLightSource = (impactPoint - lsItem->geometry_->f_.o_).norm();
                lrImpactToLightSource.origin_ = impactPoint;
                lrImpactToLightSource.dir_ = lsItem->geometry_->f_.o_ - impactPoint;
                lrImpactToLightSource.dir_.normalize();

                //Check if the diffusion light ray is intercepted by another object, that would cast a shadow
                if(inHalfSpace)
                {
                    needToCheckShadow = true;
                    double distReductionFactor = getDistReductionFactor(distImpactToLightSource+1);
                    pixIfNotInShadow.a_ = 255*cosAngle*distReductionFactor;
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
            else
            {
                cosAngle = std::abs(- lr.dir_.dot(impactNormal));
                ambiantPix.a_ = 255*cosAngle*ambiantLight_.intensity_;
                ambiantPix.r_ = impactItem->material_->color_.r_;
                ambiantPix.g_ = impactItem->material_->color_.g_;
                ambiantPix.b_ = impactItem->material_->color_.b_;
            }
        }
        pix = pix + ambiantPix;


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
            if (secImpact)
            {
                double distReductionFactor = getDistReductionFactor(secImpactDist+1);
                if(specReflItem->material_->lightEmitter_)
                {
                    specReflPix.a_ = 255*distReductionFactor*impactItem->material_->reflectiveness_*specReflItem->material_->lightIntensity_;
                }
                else
                {
                    specReflPix.a_ = 255*distReductionFactor*impactItem->material_->reflectiveness_*ambiantLight_.intensity_;
                }
                specReflPix.x_ = pix.x_;
                specReflPix.y_ = pix.y_;
                specReflPix.r_ = specReflItem->material_->color_.r_;
                specReflPix.g_ = specReflItem->material_->color_.g_;
                specReflPix.b_ = specReflItem->material_->color_.b_;
                pix = pix + specReflPix;
            }
        }
    }
    return;
}

double Scene::getDistReductionFactor(double dist) const
{
    //double distReductionFactor = 1/std::sqrt(dist+1);
    //double distReductionFactor = 1/(dist+1);
    //double distReductionFactor = 1/std::pow(dist+1,2);
    double distReductionFactor = 1/std::pow(dist/4+1,3);
    return distReductionFactor;
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
        if (tmpImpact && tmpDist < impactDist && tmpDist > 1e-9)
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

    os << s.camera_.describe() << std::endl;

    for(size_t itemIndex = 0; itemIndex < s.items_.size(); ++itemIndex)
        os << s.items_[itemIndex]->describe() << std::endl;

    os << "+++++++++++++++++++++++++++++++++++";
}

std::string SceneData::describe() const
{
    std::stringstream ss;
    ss << "========== Scene ==========\n";
    ss << rData.describe();
    ss << aData.describe();
    ss << cData.describe();
    for (size_t i = 0; i < itemsData.size(); i++)
    {
        ss << itemsData[i].describe();
    }
    return ss.str();
}
