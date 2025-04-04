#include "image.hpp"
#include <iostream>
#include <fstream>


Color::Color()
    : r(0), g(0), b(0)
{

}

Color::Color(float r, float g, float b)
    : r(r), g(g), b(b)
{

}

Color::~Color()
{

}

Image::Image(int width, int height)
    : m_width(width), m_height(height), m_colors(std::vector<Color>(width * height)) 
{

}

Image::~Image()
{
}

Color Image::GetColor(int x, int y) const 
{
    return m_colors[y * m_width + x];
}

void Image::SetColor(const Color& color, int x, int y)
{
    m_colors[y * m_width + x].r = color.r;
    m_colors[y * m_width + x].g = color.g;
    m_colors[y * m_width + x].b = color.b;

}
void Image::Export(const char* path) const
{
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);

    if(!f.is_open())
    {
    std::cout<<"File does not open/exist\n";
    return;
    }
    unsigned char bmpPad[3] = {0, 0, 0 };   
    const int paddingAmount = ((4 - (m_width * 3) % 4 ) % 4);
    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 3 +  paddingAmount * m_height; 

    unsigned char fileHeader [fileHeaderSize];

    //file type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    //file size 
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    //Reserved
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    fileHeader[10] = fileHeaderSize + informationHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char informationHeader [informationHeaderSize]; 
    // Header size
    informationHeader[0] = informationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    // Image width
    informationHeader[4] = m_width;
    informationHeader[5] = m_width >> 8;
    informationHeader[6] = m_width >> 16;
    informationHeader[7] = m_width >> 24;
    // Image height
    informationHeader[8] = m_height;
    informationHeader[9] = m_height >> 8;
    informationHeader[10] = m_height >> 16;
    informationHeader[11] = m_height >> 24;
    // Planes
    informationHeader[12] = 1;
    informationHeader[13] = 0;
    // Bits per pixel
    informationHeader[14] = 24;
    informationHeader[15] = 0;
    // Compression (no comperssion)
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    // Image size
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    // X pixels per meter (not specified)
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    // Y pixels per meter (not specified)
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    // Total colors  
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    // Important colors
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);
    
    for ( int y = 0; y < m_height ; y++)
    {
        for ( int x = 0; x < m_width ; x++)
        {
            unsigned char r = static_cast<unsigned char>(GetColor(x,y).r * 255.0f);
            unsigned char g = static_cast<unsigned char>(GetColor(x,y).g * 255.0f);
            unsigned char b = static_cast<unsigned char>(GetColor(x,y).b * 255.0f);

            unsigned char color[] = {b, g, r};

            f.write(reinterpret_cast<char*>(color),3);
        }
        f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
        
    }
    f.close();

    std::cout << "File created\n";
}


void Image::Read(const char * path)
{
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);

    if (!f.is_open())
    {
    std::cout << "File cannot be open" << std::endl;
    return;
    }
    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;

    unsigned char fileHeader[fileHeaderSize];
    f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

    if (fileHeader[0] != 'B' || fileHeader[1] != 'M')
    {
        std::cout << "It's not a bitmap image" << std::endl;
        f.close();
        return;
    }

    unsigned char informationHeader[informationHeaderSize];
    f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
    m_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
    m_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

    m_colors.resize(m_width * m_height);
    const int paddingAmount = ((4 - (m_width * 3) % 4 ) % 4);

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
        unsigned char color[3];
        f.read(reinterpret_cast<char*>(color),  3); 

        m_colors[y * m_width + x].r = static_cast<float>(color[2]) / 255.0f;
        m_colors[y * m_width + x].g = static_cast<float>(color[1]) / 255.0f;
        m_colors[y * m_width + x].b = static_cast<float>(color[0]) / 255.0f;
        }

        f.ignore(paddingAmount);
    }
    f.close();

    std::cout << "File read" << std::endl;
}
