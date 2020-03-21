#include <gtest/gtest.h>
#include <iostream>
#include "Vector3.hh"
#include "Frame3.hh"
#include "Scene.hh"
#include "Item.hh"
#include "Sphere.hh"
#include "Plane.hh"
#include "Polygon.hh"
#include "Camera.hh"
#include "Window.hh"

void testRender2(const Scene& myScene, Window& myWindow, const std::string& s)
{
    myWindow.clear();
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

TEST(ManoTests, VisuPlaneSphereTest)
{
    int resX = 1000;
    int fovX = 110;
    int fovY = 100;
    Scene myScene;
    Frame3 f;

    Camera cam(f, fovX, fovY, resX);

    Window myWindow(cam.resX_, cam.resY_);

    AmbiantLight al(0.02, Vector3RGB(255,255,255));

    Frame3 fls;
    Material mls(Vector3RGB(255, 255, 255), 0, 0, 0, true, 0.6);
    Sphere gsls(fls, 0.1);
    Item ls(&gsls, &mls);
    ls.geometry_->f_.setOriginPos( 0.7, 1.0, -0.4);

    Frame3 fls2;
    Material mls2(Vector3RGB(255, 255, 255), 0, 0, 0, true, 0.6);
    Sphere gsls2(fls2, 0.1);
    Item ls2(&gsls2, &mls2);
    ls2.geometry_->f_.setOriginPos( 0.5, 2.6, -0.4);

    Frame3 fs0;
    Material ms0(Vector3RGB(255, 0, 0), 0.1, 0, 1, false, 0);
    fs0.translate( 0.0, 2.0, 0.0);
    Sphere gs0(fs0, 0.5);
    Item s0(&gs0, &ms0);

    Frame3 fs1;
    Material ms1(Vector3RGB(255, 20, 150), 0.1, 0, 1, false, 0);
    fs1.translate( 1.05, 1.2, -0.2);
    Sphere gs1(fs1, 0.3);
    Item s1(&gs1, &ms1);

    Frame3 fs2;
    Material ms2(Vector3RGB(0, 0, 255), 0.1, 0, 1, false, 0);
    fs2.translate( -0.6, 0.8, -0.3);
    Sphere gs2(fs2, 0.2);
    Item s2(&gs2, &ms2);

    Frame3 fpBottom;
    Material mpBottom(Vector3RGB(80, 80, 80), 0, 0, 0, false, 0);
    fpBottom.translate( 0.0, 2.0, -0.5);
    Plane gpBottom(fpBottom);
    Item pBottom(&gpBottom, &mpBottom);

    Frame3 fpTop;
    Material mpTop(Vector3RGB(80, 80, 80), 0, 0, 0, false, 0);
    fpTop.translate( 0.0, 2.0, 0.5);
    Plane gpTop(fpTop);
    Item pTop(&gpTop, &mpTop);

    Frame3 fpBack;
    Material mpBack(Vector3RGB(80, 80, 80), 0, 0, 0, false, 0);
    fpBack.translate( 0.0, 3.0, 0.0);
    fpBack.rotate(Vector3(1.0, 0.0, 0.0), 90);
    Plane gpBack(fpBack);
    Item pBack(&gpBack, &mpBack);

    Frame3 fpRight;
    Material mpRight(Vector3RGB(80, 80, 80), 0, 0, 0, false, 0);
    fpRight.translate( 3.0, 0.0, 0.0);
    fpRight.rotate(Vector3(0.0, 1.0, 0.0), 90);
    Plane gpRight(fpRight);
    Item pRight(&gpRight, &mpRight);

    Frame3 fpLeft;
    Material mpLeft(Vector3RGB(80, 80, 80), 0, 0, 0, false, 0);
    fpLeft.translate( -3.0, 0.0, 0.0);
    fpLeft.rotate(Vector3(0.0, 1.0, 0.0), 90);
    Plane gpLeft(fpLeft);
    Item pLeft(&gpLeft, &mpLeft);

    myScene.setCamera(cam);
    myScene.setAmbiantLight(al);
    myScene.addItem(&ls);
    myScene.addItem(&ls2);
    myScene.addItem(&s0);
    myScene.addItem(&s1);
    myScene.addItem(&s2);
    myScene.addItem(&pTop);
    myScene.addItem(&pBack);
    myScene.addItem(&pBottom);
    myScene.addItem(&pLeft);
    myScene.addItem(&pRight);

    testRender2(myScene, myWindow, "You should see a cool scene with 3 spheres in a grey room. Correct [y/n]?");

    //ls.geometry_->f_.setOriginPos( 0.0, 1.3, -1.0);
    //s0.material_->color_ = Vector3RGB(0, 255, 0);
    //testRender(myScene, myWindow, "You should see 1 green sphere lit from the bottom. Correct? [y/n]");

    //ls.geometry_->f_.setOriginPos( 1.0, 1.3, 0.0);
    //s0.material_->color_ = Vector3RGB(0, 0, 255);
    //testRender(myScene, myWindow, "You should see 1 blue sphere lit from the right. Correct? [y/n]");

    //ls.geometry_->f_.setOriginPos( -1.0, 1.3, 0.0);
    //s0.material_->color_ = Vector3RGB(255, 20, 150);
    //testRender(myScene, myWindow, "You should see 1 pink sphere lit from the left. Correct? [y/n]");
}

TEST(UnitTests, PlaneTest)
{
    double prec = 1e-9;
    Frame3 f;
    f.translate(Vector3(0,0,0));

    Plane pl(f);
    Polygon po(f);

    Vector3 impactPoint(0,0,0);
    Vector3 normal(0,0,0);
    double dist(0);
    bool impact(false);

    LightRay lr(Vector3(0,0,1), Vector3(0,0,-1));
    impact = pl.intersect(lr, impactPoint, normal, dist);
    ASSERT_TRUE(impact);
    ASSERT_TRUE(impactPoint.isApprox(Vector3(0, 0, 0), prec));
    ASSERT_TRUE(normal.isApprox(Vector3(0, 0, 1), prec));
    ASSERT_TRUE(std::abs(dist - 1.0) < prec);

    lr = LightRay(Vector3(0,0,1), Vector3(0,0,1));
    impact = pl.intersect(lr, impactPoint, normal, dist);
    ASSERT_FALSE(impact);

    std::cout << "Polygon intersection should fail" << std::endl;
    lr = LightRay(Vector3(0,0,1), Vector3(0,0,1));
    impact = po.intersect(lr, impactPoint, normal, dist);
    ASSERT_FALSE(impact);

    std::cout << "Polygon intersection should succeed" << std::endl;
    lr = LightRay(Vector3(0,0,1), Vector3(0,0,-1));
    impact = po.intersect(lr, impactPoint, normal, dist);
    ASSERT_TRUE(impact);
    ASSERT_TRUE(impactPoint.isApprox(Vector3(0, 0, 0), prec));
    ASSERT_TRUE(normal.isApprox(Vector3(0, 0, 1), prec));
    ASSERT_TRUE(std::abs(dist - 1.0) < prec);
}

