#include "Scene.hh"

Scene::Scene()
  : items_(),
    camera_(),
    ambiantLight_(),
    simplifiedRender_(false),
    maxDepthIndex_(5)
{
}

Scene::Scene(const SceneData& sData)
  : items_(),
    camera_(sData.cData),
    ambiantLight_(sData.aData),
    simplifiedRender_(sData.sExData.simplifiedRender),
    maxDepthIndex_(sData.sExData.maxDepthRecursion),
    maxNumThreads_(sData.sExData.maxNumThreads),
    powerDistReduction_(sData.sExData.powerDistReduction),
    exposition_(sData.sExData.exposition),
    gammaPower_(sData.sExData.gammaPower)
{
    for (size_t i = 0; i < sData.itemsData.size(); i++)
    {
        PerformanceTracker::instance().incrementItems();
        items_.push_back(new Item(sData.itemsData.at(i)));
        items_[i]->geometry_->simplifiedRender_ = simplifiedRender_;
    }
}

Scene::~Scene()
{
    for (size_t i = 0; i < items_.size(); i++)
    {
        delete items_[i];
    }
}

void Scene::renderParallel(std::vector<Pixel>& res, const size_t& nPixToCompute, size_t nPointsRendered) const
{
    int nPix = res.size();
    int numThreads = std::thread::hardware_concurrency() - 1;
    if(numThreads > maxNumThreads_)
        numThreads = maxNumThreads_;

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
        pool[i] = new std::thread(&Scene::castMultipleRandomRays, this, std::ref(res), beginIndex, endIndex, nPointsRendered);
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

void Scene::castMultipleRandomRays( std::vector<Pixel>& vecPix, size_t beginIndex, size_t endIndex, size_t nRenderedPixels) const
{
    for (size_t i = beginIndex; i < endIndex; i++)
    {
        castPrimaryRay( vecPix[i], i + nRenderedPixels);
    }
}

void Scene::castRandomRay( Pixel& pix, size_t iOrderedRay) const
{
}

double Scene::getDistReductionFactor(double dist) const
{
    double distReductionFactor;
    switch(powerDistReduction_)
    {
        case 1:
            distReductionFactor = 1/(dist+1);
            break;
        case 2:
            distReductionFactor = 3.14/std::pow(dist+1,2);
            break;
        case 3:
            distReductionFactor = 1/std::pow(dist/4+1,3);
            break;
        case 0:
            distReductionFactor = 1;
            break;
    }
    return distReductionFactor;
}

struct InterceptionData
{
    InterceptionData()
        : itemIndex(0),
        intercept(false),
        opaque(false),
        lightEmitter(false),
        color(0,0,0),
        lengthIntercept(0),
        distIntercept(0)
    {
    }

    size_t itemIndex;
    bool intercept;
    bool opaque;
    bool lightEmitter;
    Vector3RGB color;
    double lengthIntercept; // TODO: not used yet
    double distIntercept;
    Vector3 pointInWorld;
    Vector3 pointInFrame;
};

bool compareByDist(const InterceptionData& a, const InterceptionData& b)
{
    return a.distIntercept < b.distIntercept;
}

void Scene::isIntercepted(const LightRay& lrImpactToLightSource, double distImpactToLightSource, size_t impactItemIndex, size_t lightSourceIndex, Vector3RGB& transparencyColor) const
{
    std::vector<InterceptionData> interceptionData;
    for (size_t shadItemIndex = 0; shadItemIndex < items_.size(); shadItemIndex++)
    {
        if(not (shadItemIndex == lightSourceIndex))
        {
            Vector3 tmpPoint;
            Vector3 tmpNormal;
            double tmpDist;
            Item* shadowingItem = items_[shadItemIndex];
            bool tmpImpact = shadowingItem->intersect(lrImpactToLightSource, tmpPoint, tmpNormal, tmpDist);
            if(tmpImpact and tmpDist < distImpactToLightSource)
            {
                InterceptionData intData;
                intData.itemIndex = shadItemIndex;
                intData.intercept = true;
                intData.opaque = (shadowingItem->material_->refractiveIndex_ == 0.0);
                intData.lightEmitter = shadowingItem->material_->lightEmitter_;
                intData.pointInWorld = tmpPoint;
                intData.pointInFrame = shadowingItem->geometry_->f_.pointFromWorld(tmpPoint);
                intData.color = shadowingItem->material_->texture_->color(intData.pointInFrame.x_, intData.pointInFrame.y_);
                intData.distIntercept = tmpDist;
                interceptionData.push_back(intData);
            }
        }
    }
    //Sort interceptionData vector by order of distance of interception
    std::sort(interceptionData.begin(), interceptionData.end(), compareByDist);

    transparencyColor = Vector3RGB(1,1,1);
    for (auto& intData: interceptionData)
    {
        if(intData.opaque and not intData.lightEmitter)
        {
            transparencyColor *= Vector3RGB(0,0,0);
        }
        else if(intData.lightEmitter)
        {
            // Nothing to do, product by 1...
            //transparencyColor *= Vector3RGB(1,1,1);
        }
        else
        {
            //Compute the distance traveled in the transparent medium
            Item* intItem = items_[intData.itemIndex];
            double distTransparent;
            {
                LightRay lrTransparent(intData.pointInWorld + lrImpactToLightSource.dir_ * 1e-9, lrImpactToLightSource.dir_);
                Vector3 impactPointTransparent, impactNormalTransparent;
                intItem->intersect(lrTransparent, impactPointTransparent, impactNormalTransparent, distTransparent);
            }
            transparencyColor *= intItem->material_->texture_->color(intData.pointInFrame.x_, intData.pointInFrame.y_);
            transparencyColor *= std::exp(-intItem->material_->absorption_*distTransparent);
        }
    }
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

    os << s.ambiantLight_ << std::endl;

    os << s.camera_ << std::endl;

    for(size_t itemIndex = 0; itemIndex < s.items_.size(); ++itemIndex)
        os << s.items_[itemIndex]->describe() << std::endl;

    os << "+++++++++++++++++++++++++++++++++++";
    return os;
}

std::string SceneData::describe() const
{
    std::stringstream ss;
    ss << "========== SceneData ==========\n";
    ss << rData.describe();
    ss << aData.describe();
    ss << cData.describe();
    for (size_t i = 0; i < itemsData.size(); i++)
    {
        ss << itemsData[i].describe();
    }
    return ss.str();
}

void Scene::translateCamera(double x, double y, double z)
{
    camera_.frame_.translate(x, y, z);
}

void Scene::translateCameraLocal(double x, double y, double z)
{
    camera_.frame_.translateLocal(x, y, z);
    std::cout << "camera_.frame_: " << camera_.frame_ << std::endl;
}

void Scene::multiplyResolution(double ratio)
{
    int newResX = int(ratio * double(camera_.resX_));
    int newResY = int(ratio * double(camera_.resY_));
    camera_.changeResolution(newResX, newResY);
}

void Scene::toggleSimplifiedRender()
{
    simplifiedRender_ = not simplifiedRender_;
    for (size_t i = 0; i < items_.size(); i++)
    {
        items_[i]->geometry_->simplifiedRender_ = simplifiedRender_;
    }
}

void Scene::castPrimaryRay(Pixel& pix, size_t iOrderedRay) const
{
    PerformanceTracker::instance().incrementPrimaryRays();
    LightRay lr;
    camera_.castOrderedRay(lr, pix, iOrderedRay);
    castRay(pix, lr, 0);
}

void Scene::castRay(Pixel& pix, const LightRay& lr, size_t depthIndex) const
{
    PerformanceTracker::instance().incrementRays();
    if(depthIndex >= maxDepthIndex_)
    {
        pix.setColor(ambiantLight_.intensity_, ambiantLight_.color_);
        return;
    }

    Pixel ambiantPix(pix.x_, pix.y_);
    Pixel diffuseRefPix(pix.x_, pix.y_);
    Pixel specReflPix(pix.x_, pix.y_);
    Pixel refracPix(pix.x_, pix.y_);

    Vector3 impactPoint(0,0,0);
    Vector3 impactNormal(0,0,0);
    double impactDist = INFINITY_d();
    size_t impactItemIndex = 0;
    bool impact = findFirstImpact(lr, impactItemIndex, impactPoint, impactNormal, impactDist);

    if(not impact)
    {
        pix.setColor(ambiantLight_.intensity_, ambiantLight_.color_);
        return;
    }
    else if(simplifiedRender_)
    {
        double cosAngleSimplified = std::abs(lr.dir_.dot(impactNormal));
        Vector3 impactPointInFrame = items_[impactItemIndex]->geometry_->f_.pointFromWorld(impactPoint);
        pix.setColor(cosAngleSimplified, items_[impactItemIndex]->material_->texture_->color(impactPointInFrame.x_, impactPointInFrame.y_));
        return;
    }
    else
    {
        Item* impactItem = items_[impactItemIndex];
        Vector3 impactPointInFrame = impactItem->geometry_->f_.pointFromWorld(impactPoint);

        /*************************************
        *  Direct impact with light source  *
        *************************************/
        if (impactItem->material_->lightEmitter_)
        {
            double distReductionFactor = getDistReductionFactor(impactDist);
            if(depthIndex == 0) // exception when ray goes straight from lightsource to camera
                pix.setColor(1.0, impactItem->material_->texture_->color(0,0));
            else
                pix.setColor(distReductionFactor*impactItem->material_->lightIntensity_, impactItem->material_->texture_->color(0,0));
            pix.clamp();
            return;
        }

        /*************
        *  Fresnel  *
        *************/
        double FR = 1; // Fresnel coef reflexion
        double FT = 0; // Fresnel coef transmission
        bool refractionExists = false;

        double n1 = lr.refractiveIndex_;
        double n2 = 0;
        if(lr.refractiveIndex_ == impactItem->material_->refractiveIndex_)
            n2 = 1.0;
        else
            n2 = impactItem->material_->refractiveIndex_;
        double n1sn2 = n1/n2;
        double c1 = -impactNormal.dot(lr.dir_); // cos(theta1)
        double c22 = 1 - std::pow(n1sn2,2)*(1-std::pow(c1,2)); // squared cos(theta2)
        double c2 = 0; // cos(theta2)

        if(c22 >= 0 and n2 > 0)
        {
            c2 = std::sqrt(c22);
            refractionExists = true;
            double FRparallel = std::pow((n2*c1 - n1*c2)/(n2*c1 + n1*c2),2);
            double FRorthogonal = std::pow((n2*c2 - n1*c1)/(n2*c2 + n1*c1),2);
            FR = (FRparallel + FRorthogonal)/2;
            FT = 1 - FR;
        }

        /****************
        *  Refraction  *
        ****************/
        if(refractionExists)
        {
            Vector3 lrRefractDir;
            //TODO remove redundent calculation in refract
            refractionExists = lr.dir_.refract(lrRefractDir, impactNormal, n1, n2);
            if(refractionExists)
            {
                Vector3 impactPointRefr = impactPoint - impactNormal*1e-6;
                LightRay lrRefraction(impactPointRefr, lrRefractDir, n2);
                castRay(refracPix, lrRefraction, depthIndex+1);
                refracPix *= impactItem->material_->texture_->color(0,0);
            }
        }

        /*************************
        *  Specular reflection  *
        *************************/
        // Rugosity can be taken into account here
        Vector3 lrSpecRefDir = lr.dir_.symmetrize(impactNormal);
        //lrSpecRefDir.addNoise(impactItem->material_->rugosity_);
        lrSpecRefDir.normalize();
        Vector3 impactPointRefl = impactPoint + impactNormal*1e-6;
        LightRay lrSpecReflection(impactPointRefl, lrSpecRefDir, lr.refractiveIndex_);
        castRay(specReflPix, lrSpecReflection, depthIndex+1);

        /************************
        *  Diffuse reflection  *
        ************************/
        //Is there a light source in the half plane (impactPoint impactNormal)
        bool needToCheckShadow = false;
        LightRay lrImpactToLightSource;
        double distImpactToLightSource;
        for (size_t itemIndex = 0; itemIndex < items_.size(); itemIndex++)
        {
            Pixel pixIfNotInShadow(pix.x_, pix.y_);
            Pixel pixPhongIfNotInShadow(pix.x_, pix.y_);
            Item* lsItem = items_[itemIndex];
            double cosAngleDiffuse = 0;
            double cosAnglePhong = 0;
            if (lsItem->material_->lightEmitter_)
            {
                bool inHalfSpace = lsItem->isInHalfSpace(impactPoint, impactNormal, lr.dir_, cosAngleDiffuse, cosAnglePhong);
                distImpactToLightSource = (impactPoint - lsItem->geometry_->f_.o_).norm();
                lrImpactToLightSource.dir_ = lsItem->geometry_->f_.o_ - impactPoint;
                lrImpactToLightSource.dir_.normalize();
                lrImpactToLightSource.origin_ = impactPoint + lrImpactToLightSource.dir_ * 1e-9;

                //Check if the diffusion light ray is intercepted by another object, that would cast a shadow
                if(inHalfSpace)
                {
                    double distReductionFactor = getDistReductionFactor(distImpactToLightSource);
                    //TODO: there is a problem here, the setColor should contain some form of light intensity
                    //Contribution of diffuse reflexion
                    pixIfNotInShadow.setColor(cosAngleDiffuse*distReductionFactor*lsItem->material_->lightIntensity_, impactItem->material_->texture_->color(impactPointInFrame.x_, impactPointInFrame.y_)*lsItem->material_->texture_->color(0,0));
                    //Contribution of specular phong reflexion
                    double phongCoeff = impactItem->material_->specularGain_ * std::pow(cosAnglePhong, impactItem->material_->specularExponent_);
                    pixPhongIfNotInShadow.setColor(phongCoeff*distReductionFactor*lsItem->material_->lightIntensity_, lsItem->material_->texture_->color(0,0));


                    Vector3RGB transparencyColor(1,1,1);
                    isIntercepted(lrImpactToLightSource, distImpactToLightSource, impactItemIndex, itemIndex, transparencyColor);
                    pixIfNotInShadow *= transparencyColor;
                    pixPhongIfNotInShadow *= transparencyColor;
                    diffuseRefPix += pixIfNotInShadow;
                    diffuseRefPix += pixPhongIfNotInShadow;
                }
            }
            else
            {
                cosAngleDiffuse = std::abs(- lr.dir_.dot(impactNormal));
                ambiantPix.setColor(cosAngleDiffuse*ambiantLight_.intensity_, impactItem->material_->texture_->color(impactPointInFrame.x_, impactPointInFrame.y_));
                diffuseRefPix += ambiantPix;
            }
        }


        double FRspec = FR * impactItem->material_->reflectiveness_;
        double FRdiff = FR - FRspec;

        refracPix*=FT;
        specReflPix*=FRspec;
        diffuseRefPix*=FRdiff;
        pix += refracPix;
        pix += specReflPix;
        pix += diffuseRefPix;

        pix.applyGammaCorrection(exposition_, gammaPower_);
        pix.clamp();
    }
}
