#pragma once
#include <iostream>
#include <vector>
#include <thread>

#include "dataStructures.hh"
#include "Vector3.hh"
#include "Item.hh"
#include "AmbiantLight.hh"
#include "Camera.hh"
#include "Pixel.hh"

class Scene
{
public:
    Scene ();
    Scene (const SceneData& sData);
    virtual ~Scene ();
    void addItem(Item* i);
    void setCamera(const Camera& c);
    void setAmbiantLight(const AmbiantLight& al);
    void castRandomRayInPlace(Pixel& pix, size_t index) const;
    void castMultipleRandomRaysInPlace(std::vector<Pixel>& vecPix, size_t beginIndex, size_t endIndex, size_t nRenderedPoints) const;
    void renderSerial(std::vector<Pixel>& res, const size_t& nPoint, size_t nRenderedPoints) const;
    void renderParallel(std::vector<Pixel>& res, const size_t& nPoint, size_t nRenderedPoints) const;
    bool isIntercepted(const LightRay& lrImpactToLightSource, double distImpactToLightSource, size_t impactItemIndex) const;
    bool findFirstImpact(const LightRay& lr, size_t& impactItemIndex, Vector3& impactPoint, Vector3& impactNormal, double& impactDist) const;
    double getDistReductionFactor(double dist) const;

    void translateCamera(double x, double y, double z);
    void translateCameraLocal(double x, double y, double z);
    void multiplyResolution(double ratio);
    void toggleSimplifiedRender();

    AmbiantLight ambiantLight_;
    std::vector<Item*> items_;
    Camera camera_;
    bool simplifiedRender_;

    friend std::ostream& operator<<(std::ostream& os, const Scene& s);
};
