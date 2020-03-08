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

TEST(ManoTests, Visu9SpheresTest)
{
    int resX = 1000;
    int fovX = 100;
    int fovY = 100;
    Scene myScene;
    Frame3 f;

    Camera cam(f, fovX, fovY, resX);

    Window myWindow(cam.resX_, cam.resY_);

    AmbiantLight al(0.02, Vector3RGB(255,255,255));

    Material mls(Vector3RGB(255, 255, 255), 0.05, 0, 1, true, 1);
    Material ms0(Vector3RGB(255, 0  , 0  ), 0.05, 0, 5, false, 0);
    Material ms1(Vector3RGB(0  , 255, 0  ), 0.05, 0, 5, false, 0);
    Material ms2(Vector3RGB(0  , 0  , 255), 0.05, 0, 5, false, 0);
    Material ms3(Vector3RGB(255, 125, 255), 0.05, 0, 5, false, 0);
    Material ms4(Vector3RGB(255, 255, 0  ), 0.05, 0, 5, false, 0);
    Material ms5(Vector3RGB(125, 255, 125), 0.05, 0, 5, false, 0);
    Material ms6(Vector3RGB(0  , 125, 255), 0.05, 0, 5, false, 0);
    Material ms7(Vector3RGB(125, 255, 255), 0.05, 0, 5, false, 0);
    Material ms8(Vector3RGB(255, 125, 0  ), 0.05, 0, 5, false, 0);

    Frame3 fls, fs0, fs1, fs2, fs3, fs4, fs5, fs6, fs7, fs8;
    fls.translate(0.0, 2.0, 0.0);
    fs0.translate( 0.0, 2.5, 0.0);
    fs1.translate( 1.2, 2.5, 0.0);
    fs2.translate( 0.0, 2.5, 1.2);
    fs3.translate(-1.2, 2.5, 0.0);
    fs4.translate( 0.0, 2.5, -1.2);
    fs5.translate( 1.2, 2.5, 1.2);
    fs6.translate( 1.2, 2.5, -1.2);
    fs7.translate(-1.2, 2.5, 1.2);
    fs8.translate(-1.2, 2.5, -1.2);

    Sphere gsls(fls, 0.2);
    Sphere gs0(fs0, 0.5);
    Sphere gs1(fs1, 0.5);
    Sphere gs2(fs2, 0.5);
    Sphere gs3(fs3, 0.5);
    Sphere gs4(fs4, 0.5);
    Sphere gs5(fs5, 0.5);
    Sphere gs6(fs6, 0.5);
    Sphere gs7(fs7, 0.5);
    Sphere gs8(fs8, 0.5);

    Item ls(&gsls, &mls);
    Item s0(&gs0, &ms0);
    Item s1(&gs1, &ms1);
    Item s2(&gs2, &ms2);
    Item s3(&gs3, &ms3);
    Item s4(&gs4, &ms4);
    Item s5(&gs5, &ms5);
    Item s6(&gs6, &ms6);
    Item s7(&gs7, &ms7);
    Item s8(&gs8, &ms8);

    Frame3 fpBottom;
    Material mpBottom(Vector3RGB(150, 150, 150), 0, 0, 5, false, 0);
    fpBottom.translate( 0.0, 3.0, -0.5);
    fpBottom.rotate(Vector3(1,0,0), 90.0);
    Plane gpBottom(fpBottom);
    Item pBottom(&gpBottom, &mpBottom);

    myScene.addCamera(&cam);
    myScene.setAmbiantLight(al);
    myScene.addItem(&ls);
    //myScene.addItem(&s0);
    myScene.addItem(&s1);
    myScene.addItem(&s2);
    myScene.addItem(&s3);
    myScene.addItem(&s4);
    myScene.addItem(&s5);
    myScene.addItem(&s6);
    myScene.addItem(&s7);
    myScene.addItem(&s8);
    myScene.addItem(&pBottom);

    std::cout << myScene << std::endl;

    int iter = 0;
    int nPixPerRender = 100000;
    while(iter < 10000000)
    {
        std::vector<Pixel> pixs(nPixPerRender);
        myScene.renderParallel(pixs, 0, nPixPerRender);
        myWindow.addPixels(pixs);
        myWindow.render();
        iter+=nPixPerRender;
    }

    std::cout << "You should see 9 sheres of same size\nCorrect?" << std::endl;
    char answer;
    std::cin >> answer;
    ASSERT_TRUE(answer == 'y');
}

