#include <gtest/gtest.h>
#include <iostream>
#include "Vector3.hh"
#include "Frame3.hh"
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "Plane.hh"
#include "Camera.hh"
#include "Window.hh"

void testRender(const Scene& myScene, double resX, double resY, const std::string& s, bool requireConfirm)
{
    Window myWindow(resX, resY);
    int iter = 0;
    int nPixPerRender = 100000;
    while(iter < 6000000)
    {
        std::vector<Pixel> pixs(nPixPerRender);
        myScene.renderParallel(pixs, 0, nPixPerRender);
        myWindow.addPixels(pixs);
        myWindow.render();
        iter+=nPixPerRender;
    }

    std::cout << s;
    if (requireConfirm)
    {
        char answer;
        std::cin >> answer;
        ASSERT_TRUE(answer == 'y');
    }
}

TEST(ManoTests, Visu1SpheresTest)
{
    int resX = 1000;
    int fovX = 100;
    int fovY = 100;
    Scene myScene;
    Frame3 f;

    Camera cam(f, fovX, fovY, resX);

    AmbiantLight al(0.02, Vector3RGB(255,255,255));

    Frame3 fls;
    Material mls(Vector3RGB(255, 255, 255), 0, 0, 0, true, 1);
    Sphere gsls(fls, 0.1);
    Item ls(&gsls, &mls);

    Frame3 fs0;
    Material ms0(Vector3RGB(255, 0, 0), 0.1, 0, 1, false, 0);
    fs0.translate( 0.0, 2.0, 0.0);
    Sphere gs0(fs0, 0.5);
    Item s0(&gs0, &ms0);

    Frame3 fpBottom;
    Material mpBottom(Vector3RGB(150, 150, 150), 0, 0, 0, false, 0);
    fpBottom.translate( 0.0, 2.0, -0.5);
    fpBottom.rotate(Vector3(1,0,0), 90.0);
    Plane gpBottom(fpBottom);
    Item pBottom(&gpBottom, &mpBottom);

    myScene.addCamera(&cam);
    myScene.setAmbiantLight(al);
    myScene.addItem(&ls);
    myScene.addItem(&s0);
    myScene.addItem(&pBottom);

    ls.geometry_->f_.setOriginPos( 0.0, 1.3, 1.0);
    s0.material_->color_ = Vector3RGB(255, 0, 0);
    testRender(myScene, cam.resX_, cam.resY_, "You should see 1 red sphere lit from the top. Correct [y/n]?", false);

    ls.geometry_->f_.setOriginPos( 0.0, 1.3, -1.0);
    s0.material_->color_ = Vector3RGB(0, 255, 0);
    testRender(myScene, cam.resX_, cam.resY_, "You should see 1 green sphere lit from the bottom. Correct? [y/n]", false);

    ls.geometry_->f_.setOriginPos( 1.0, 1.3, 0.0);
    s0.material_->color_ = Vector3RGB(0, 0, 255);
    testRender(myScene, cam.resX_, cam.resY_, "You should see 1 blue sphere lit from the right. Correct? [y/n]", false);

    ls.geometry_->f_.setOriginPos( -1.0, 1.3, 0.0);
    s0.material_->color_ = Vector3RGB(255, 20, 150);
    testRender(myScene, cam.resX_, cam.resY_, "You should see 1 pink sphere lit from the left. Correct? [y/n]", true);
}

TEST(ManoTests, VisuSphereFromInsideTest)
{
    int resX = 1000;
    int fovX = 100;
    int fovY = 100;
    Scene myScene;
    Frame3 f;

    Camera cam(f, fovX, fovY, resX);

    AmbiantLight al(0.02, Vector3RGB(255,255,255));

    Frame3 fls;
    Material mls(Vector3RGB(255, 255, 255), 0, 0, 0, true, 5);
    fls.translate( 0.2, -0.2, -0.1);
    Sphere gsls(fls, 0.1);
    Item ls(&gsls, &mls);

    Frame3 fs0;
    Material ms0(Vector3RGB(255, 0, 0), 0.1, 0, 1, false, 0);
    fs0.translate( 0.2, 0.4, 0.0);
    Sphere gs0(fs0, 0.1);
    Item s0(&gs0, &ms0);

    Frame3 fs1;
    Material ms1(Vector3RGB(0, 254, 0), 0.1, 0, 1, false, 0);
    fs1.translate( 0.0, 0.0, 0.0);
    Sphere gs1(fs1, 1.0);
    Item s1(&gs1, &ms1);

    Frame3 fpBottom;
    Material mpBottom(Vector3RGB(150, 150, 150), 0, 0, 0, false, 0);
    fpBottom.translate( 0.0, 0.0, -0.5);
    Plane gpBottom(fpBottom);
    Item pBottom(&gpBottom, &mpBottom);

    myScene.addCamera(&cam);
    myScene.setAmbiantLight(al);
    myScene.addItem(&ls);
    myScene.addItem(&s0);
    myScene.addItem(&s1);
    myScene.addItem(&pBottom);

    testRender(myScene, cam.resX_, cam.resY_, "You should see 1 pink sphere lit from the left. Correct? [y/n]", true);
}
