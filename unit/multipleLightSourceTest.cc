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

void testRenderMultiLS(const Scene& myScene, double resX, double resY, const std::string& s)
{
    Window myWindow(resX, resY);
    int iter = 0;
    int nPixPerRender = 100000;
    while(iter < myWindow.nPixels())
    {
        if(iter+nPixPerRender > myWindow.nPixels())
            nPixPerRender = myWindow.nPixels() - iter;
        std::vector<Pixel> pixs(nPixPerRender);
        myScene.renderParallel(pixs, nPixPerRender, iter);
        myWindow.addPixels(pixs);
        myWindow.render();
        iter+=nPixPerRender;
    }

    std::cout << s;
    char answer;
    std::cin >> answer;
    ASSERT_TRUE(answer == 'y');
}

TEST(ManoTests, multiLightSourcesTest)
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

    Frame3 fls2;
    Material mls2(Vector3RGB(255, 255, 255), 0, 0, 0, true, 1);
    Sphere gsls2(fls2, 0.1);
    Item ls2(&gsls2, &mls2);

    Frame3 fs0;
    Material ms0(Vector3RGB(255, 0, 0), 0.1, 0, 0.1, false, 0);
    fs0.translate( 0.0, 2.0, 0.0);
    Sphere gs0(fs0, 0.5);
    Item s0(&gs0, &ms0);

    Frame3 fpBottom;
    Material mpBottom(Vector3RGB(150, 150, 150), 0, 0, 0, false, 0);
    fpBottom.translate( 0.0, 2.0, -0.5);
    fpBottom.rotate(Vector3(1,0,0), 90.0);
    Plane gpBottom(fpBottom);
    Item pBottom(&gpBottom, &mpBottom);

    myScene.setCamera(cam);
    myScene.setAmbiantLight(al);
    myScene.addItem(&ls);
    myScene.addItem(&ls2);
    myScene.addItem(&s0);
    myScene.addItem(&pBottom);

    ls.geometry_->f_.setOriginPos( 0.0, 1.3, 1.0);
    ls2.geometry_->f_.setOriginPos( 1.0, 1.3, 0.0);
    s0.material_->color_ = Vector3RGB(255, 0, 0);
    testRenderMultiLS(myScene, cam.resX_, cam.resY_, "You should see 1 red sphere lit from the top and right. Correct [y/n]?");
}


