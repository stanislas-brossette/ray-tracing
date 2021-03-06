#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>

#include "dataStructures.hh"
#include "Vector3.hh"
#include "Item.hh"
#include "AmbiantLight.hh"
#include "Camera.hh"
#include "Pixel.hh"
#include "utils.hh"
#include "PerformanceTracker.hh"

class Scene
{
public:
    Scene ();
    Scene (const SceneData& sData);
    virtual ~Scene ();
    void addItem(Item* i);
    void setCamera(const Camera& c);
    void setAmbiantLight(const AmbiantLight& al);
    void castRandomRay(Pixel& pix, size_t index) const;
    void castMultipleRandomRays(std::vector<Pixel>& vecPix, size_t beginIndex, size_t endIndex, size_t nRenderedPoints) const;
    void renderParallel(std::vector<Pixel>& res, const size_t& nPoint, size_t nRenderedPoints) const;
    void isIntercepted(const LightRay& lrImpactToLightSource, double distImpactToLightSource, size_t impactItemIndex, size_t lightSourceIndex, Vector3RGB& transparencyColor) const;
    bool findFirstImpact(const LightRay& lr, size_t& impactItemIndex, Vector3& impactPoint, Vector3& impactNormal, double& impactDist) const;
    double getDistReductionFactor(double dist) const;

    void populateItem(Item* item, const ItemData& itemData);

    void castPrimaryRay(Pixel& pix, size_t iOrderedRay) const;
    void castRay(Pixel& pix, const LightRay& lr, size_t depthIndex) const;
    void castPrimaryRayAt(int pX, int pY, bool verbose = false) const;

    void translateCamera(double x, double y, double z);
    void translateCameraLocal(double x, double y, double z);
    void multiplyResolution(double ratio);
    void toggleSimplifiedRender();

    AmbiantLight ambiantLight_;
    std::vector<Item*> items_;
    Camera camera_;

    bool simplifiedRender_;
    size_t maxDepthIndex_;
    size_t maxNumThreads_;
    size_t numThreads_;
    size_t powerDistReduction_;
    double exposition_;
    double gammaPower_;

    mutable bool verbose_;

    friend std::ostream& operator<<(std::ostream& os, const Scene& s);
};
