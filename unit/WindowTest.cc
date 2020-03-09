#include <gtest/gtest.h>
#include <iostream>
#include "Window.hh"
#include "Pixel.hh"


TEST(WindowTest, windowTest)
{
    size_t resX = 500;
    size_t resY = 500;
    Window w(resX, resY);
    for (size_t i = 0; i < resX; i++)
    {
        for (size_t j = 0; j < resY; j++)
        {
            w.addPixel(Pixel(i, j, 255, (i/50)*50, (j/50)*50, 0));
        }
    }
    //w.render();


    w.renderConvolut();

    char answer;
    std::cout << "Satisfied?" << std::endl;
    std::cin >> answer;
    ASSERT_TRUE(answer == 'y');
}
