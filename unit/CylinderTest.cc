#include <gtest/gtest.h>
#include <iostream>
#include "Vector3.hh"
#include "Frame3.hh"
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "Cylinder.hh"
#include "Plane.hh"
#include "Camera.hh"
#include "Window.hh"

void testRenderCylinder(const Scene& myScene, Window& myWindow, const std::string& s)
{
    std::cout << myScene << std::endl;
    myWindow.clear();
    int iter = 0;
    int nPixPerRender = 100000;
    while(iter < 20000000)
    {
        std::vector<Pixel> pixs(nPixPerRender);
        myScene.renderParallel(pixs, 0, nPixPerRender);
        myWindow.addPixels(pixs);
        myWindow.render();
        iter+=nPixPerRender;
    }

    std::cout << s;
    char answer;
    std::cin >> answer;
    ASSERT_TRUE(answer == 'y');
}

TEST(ManoTests, VisuCylinderTest)
{
    int resX = 1000;
    int fovX = 110;
    int fovY = 100;
    Scene myScene;
    Frame3 fCam;
    fCam.translate(0, -0.2, 0.5);
    //fCam.rotate(Vector3(1, 0, 0), -12);
    Camera cam(fCam, fovX, fovY, resX);

    Window myWindow(cam.resX_, cam.resY_);

    AmbiantLight al(0.02, Vector3RGB(255,255,255));

    Frame3 fpBottom;
    Material mpBottom(Vector3RGB(80, 80, 80), 0, 0, 0, false, 0);
    Plane gpBottom(fpBottom);
    Item pBottom(&gpBottom, &mpBottom);
    pBottom.geometry_->f_.setOriginPos( 0.0, 0.0, 0.0);

    Frame3 fCy0;
    fCy0.translate(-0.5, 1.0, 0.0);
    fCy0.rotate(Vector3(0.0, 1.0, 0.0), 30);
    Material mCy0(Vector3RGB(250, 30, 40), 0, 0, 0, false, 0);
    Cylinder gCy0(fCy0, 0.05, 1.5);
    Item cy0(&gCy0, &mCy0);

    Frame3 fCy1;
    fCy1.translate(0.5, 1.2, 0.0);
    fCy1.rotate(Vector3(0.0, 1.0, 0.0), -30);
    Material mCy1(Vector3RGB(30, 250, 40), 0, 0, 0, false, 0);
    Cylinder gCy1(fCy1, 0.05, 1.0);
    Item cy1(&gCy1, &mCy1);

    Frame3 fCy2;
    fCy2.translate(0.0, 1.0, 0.5);
    //fCy2.rotate(Vector3(0.0, 1.0, 0.0), 0);
    Material mCy2(Vector3RGB(30, 25, 200), 0, 0, 0, false, 0);
    Cylinder gCy2(fCy2, 0.5, 0.1);
    Item cy2(&gCy2, &mCy2);

    Frame3 fls;
    Material mls(Vector3RGB(255, 255, 255), 0, 0, 0, true, 3.0);
    Sphere gsls(fls, 0.04);
    Item ls(&gsls, &mls);
    ls.geometry_->f_.setOriginPos( 0.8, 1.0, 1.0);

    Frame3 fls2;
    Material mls2(Vector3RGB(255, 255, 255), 0, 0, 0, true, 1.0);
    Sphere gsls2(fls2, 0.04);
    Item ls2(&gsls2, &mls2);
    ls2.geometry_->f_.setOriginPos( 0.4, 0.9, 0.5);

    myScene.addCamera(&cam);
    myScene.setAmbiantLight(al);
    myScene.addItem(&ls);
    myScene.addItem(&ls2);
    myScene.addItem(&pBottom);
    myScene.addItem(&cy0);
    myScene.addItem(&cy1);
    myScene.addItem(&cy2);

    testRenderCylinder(myScene, myWindow, "You should see a cool scene with 3 infinite cylinders in a grey room. Correct [y/n]?");
}



