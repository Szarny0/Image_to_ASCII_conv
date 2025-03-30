#include <iostream>
#include "image.hpp"
using namespace std;

int main (void){
    const int width = 4;
    const int height = 4;
    Image image(width, height);

    for ( int y = 0; y < height ; y++)
    {
        for ( int x = 0; x < width ; x++)
        {
            image.SetColor( Color( (float)x / (float)width, 1.0f - ((float)x / (float)width), (float)y / (float)height ), x, y );
        }    
    }
   image.Export("image.bmp"); 
}



