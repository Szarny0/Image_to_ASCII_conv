#include <iostream>
#include "image.hpp"
#include <fstream>
using namespace std;

int main (void){
/*
    const int width = 640;
    const int height = 480;
    Image image(width, height);

    for ( int y = 0; y < height ; y++)
    {
        for ( int x = 0; x < width ; x++)
        {
            image.SetColor( Color( (float)x / (float)width, 1.0f - ((float)x / (float)width), (float)y / (float)height ), x, y );
        }    
    }
   image.Export("image.bmp"); 
*/   
    Image copy(0, 0);
    const string ASCIICHARS = "                      `^”,:;Il!i~+_-?][}{1)(|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    int strLen = ASCIICHARS.size();
    copy.Read("A.bmp");
    std::ofstream f;
    f.open("ascii.txt", std::ios::out);
    for (int y = 11; y >= 0; y--)
    {
        for (int x = 10; x >= 0; x--)
        {
        const Color pixel = copy.GetColor(x, y);
        float bright = (0.2126 * pixel.r) + (0.7152 * pixel.g) + (0.0722 * pixel.b);


        float real = bright * (float)(strLen-1);
        //cout << bright << endl;
        int number = (int)real;
        

        char c = ASCIICHARS[number];
        f << c; 
        }
        f << endl;
    }
    f.close();
 //  copy.Export("copy.bmp");
}

