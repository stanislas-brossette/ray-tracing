#pragma once
#include <iostream>
#include <vector>
#include <thread>

#include "Vector3.hh"
#include "Item.hh"
#include "AmbiantLight.hh"
#include "Camera.hh"
#include "Pixel.hh"

class Scene
{
public:
    Scene ();
    virtual ~Scene ();
    void addItem(Item* i);
    void addCamera(Camera* c);
    void setAmbiantLight(const AmbiantLight& al);
    Pixel castRandomRay(size_t camIndex) const;
    void castRandomRayInPlace(size_t camIndex, Pixel& pix) const;
    void castMultipleRandomRaysInPlace(size_t camIndex, std::vector<Pixel>& vecPix, size_t beginIndex, size_t endIndex) const;
    void renderSerial(std::vector<Pixel>& res, const size_t& camIndex, const size_t& nPoint) const;
    void renderParallel(std::vector<Pixel>& res, const size_t& camIndex, const size_t& nPoint) const;
    bool isIntercepted(const LightRay& lrImpactToLightSource, double distImpactToLightSource, size_t impactItemIndex) const;
    bool findFirstImpact(const LightRay& lr, size_t& impactItemIndex, Vector3& impactPoint, Vector3& impactNormal, double& impactDist) const;

    AmbiantLight ambiantLight_;
    std::vector<Item*> items_;
    std::vector<Camera*> cameras_;

    friend std::ostream& operator<<(std::ostream& os, const Scene& s);
};
