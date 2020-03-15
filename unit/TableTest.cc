#include <gtest/gtest.h>
#include <iostream>
#include "Vector3.hh"
#include "Frame3.hh"
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "Cylinder.hh"
#include "ClosedCylinder.hh"
#include "Plane.hh"
#include "Camera.hh"
#include "Window.hh"

void renderTable(const Scene& myScene, Window& myWindow, const std::string& s)
{
    std::cout << myScene << std::endl;
    myWindow.clear();
    int iter = 0;
    int nPixPerRender = 100000;
    while(iter < 10000000)
    {
        std::vector<Pixel> pixs(nPixPerRender);
        myScene.renderParallel(pixs, nPixPerRender);
        myWindow.addPixels(pixs);
        myWindow.render();
        iter+=nPixPerRender;
    }

    std::cout << s;
    char answer;
    std::cin >> answer;
    ASSERT_TRUE(answer == 'y');
}


TEST(ManoTests, TableTest)
{
    int resX = 1200;
    int fovX = 110;
    int fovY = 100;
    Scene myScene;

    Frame3 fCam;
    fCam.translate(0.1, -0.6, 0.35);
    fCam.rotate(Vector3(1.0, 0.0, 0.0), -20);
    fCam.rotate(Vector3(0.0, 0.0, 1.0), 10);
    Camera cam(fCam, fovX, fovY, resX);

    AmbiantLight al(0.05, Vector3RGB(246,243,255));

    Frame3 fpFloor;
    Material mpFloor(Vector3RGB(80, 81, 82), 0.1, 0, 0.2, false, 0);
    fpFloor.translate( 0.0, 0.0, 0.0);
    Plane gpFloor(fpFloor);
    Item floor(&gpFloor, &mpFloor);

    Vector3RGB colorTable(210,105,30);
    double radiusTable(0.5);
    double heightTable(0.75);

    Material mTable(colorTable, 0.01, 0, 0.05, false, 0);

    Frame3 fTableTop;
    fTableTop.translate( 0.0, 0.0, heightTable);
    ClosedCylinder gTableTop(fTableTop, radiusTable, 0.02);
    Item tableTop(&gTableTop, &mTable);

    Frame3 fTableLeg0;
    fTableLeg0.translate( (radiusTable-0.05), 0.0, heightTable/2);
    Cylinder gTableLeg0(fTableLeg0, 0.025, heightTable);
    Item tableLeg0(&gTableLeg0, &mTable);

    Frame3 fTableLeg1;
    fTableLeg1.translate( 0.0, (radiusTable-0.05), heightTable/2);
    Cylinder gTableLeg1(fTableLeg1, 0.025, heightTable);
    Item tableLeg1(&gTableLeg1, &mTable);

    Frame3 fTableLeg2;
    fTableLeg2.translate( -(radiusTable-0.05), 0.0, heightTable/2);
    Cylinder gTableLeg2(fTableLeg2, 0.025, heightTable);
    Item tableLeg2(&gTableLeg2, &mTable);

    Frame3 fTableLeg3;
    fTableLeg3.translate( 0.0, -(radiusTable-0.05), heightTable/2);
    Cylinder gTableLeg3(fTableLeg3, 0.025, heightTable);
    Item tableLeg3(&gTableLeg3, &mTable);

    Frame3 fAppleRed;
    fAppleRed.translate( 0.2, -0.2, heightTable + 0.05);
    Material mAppleRed(Vector3RGB(250, 20, 24), 0, 0, 0, false, 0.0);
    Sphere gsAppleRed(fAppleRed, 0.05);
    Item appleRed(&gsAppleRed, &mAppleRed);

    Frame3 fAppleBlue;
    fAppleBlue.translate( -0.2, 0.2, heightTable + 0.05);
    Material mAppleBlue(Vector3RGB(25, 250, 20), 0, 0, 0, false, 0.0);
    Sphere gsAppleBlue(fAppleBlue, 0.05);
    Item appleBlue(&gsAppleBlue, &mAppleBlue);

    Frame3 fAppleYellow;
    fAppleYellow.translate( -0.2, -0.2, heightTable + 0.05);
    Material mAppleYellow(Vector3RGB(255, 250, 20), 0, 0, 0, false, 0.0);
    Sphere gsAppleYellow(fAppleYellow, 0.05);
    Item appleYellow(&gsAppleYellow, &mAppleYellow);

    Frame3 fLightBulb;
    fLightBulb.translate( 0.0, 0.0, heightTable + 0.3);
    Material mLightBulb(Vector3RGB(255, 255, 255), 0, 0, 0, true, 5.0);
    Sphere gsLightBulb(fLightBulb, 0.07);
    Item lightBulb(&gsLightBulb, &mLightBulb);

    Material mLamp(Vector3RGB(1,20,24), 0.01, 0, 3.0, false, 0);
    Frame3 fLamp;
    fLamp.translate( 0.0, 0.0, heightTable + 0.1);
    Cylinder gLamp(fLamp, 0.05, 0.25);
    Item lamp(&gLamp, &mLamp);

    Material mLampTop(Vector3RGB(1,20,240), 0.01, 0, 0.0, false, 0);
    Frame3 fLampTop;
    fLampTop.translate( 0.0, 0.0, heightTable + 0.3);
    Cylinder gLampTop(fLampTop, 0.15, 0.15);
    Item lampTop(&gLampTop, &mLampTop);

    Frame3 fSpot;
    fSpot.translate( 1.0, 1.0, 1.5);
    Material mSpot(Vector3RGB(255, 255, 255), 0, 0, 0, true, 10.0);
    Sphere gsSpot(fSpot, 0.05);
    Item spot(&gsSpot, &mSpot);

    myScene.setCamera(cam);
    myScene.setAmbiantLight(al);
    myScene.addItem(&floor);
    myScene.addItem(&tableTop);
    myScene.addItem(&tableLeg0);
    myScene.addItem(&tableLeg1);
    myScene.addItem(&tableLeg2);
    myScene.addItem(&tableLeg3);
    myScene.addItem(&lightBulb);
    myScene.addItem(&appleRed);
    myScene.addItem(&appleBlue);
    myScene.addItem(&appleYellow);
    myScene.addItem(&lamp);
    myScene.addItem(&lampTop);
    myScene.addItem(&spot);

    Window myWindowParallel(cam.resX_, cam.resY_);
    renderTable(myScene, myWindowParallel, "You should see a cool scene with 3 spheres in a grey room. Correct [y/n]?");
}

