#include "Globals.hpp"
// #include "Point.hpp"

namespace g{

unsigned long debugCount = 0;

unsigned int windowWidth, windowHeight;

unsigned long getDebugCount(){
    return ++debugCount;
}

void log(std::string message, unsigned int verbosity){
    if(verbosity <= verbose)
        std::cout << ++debugCount << ": " << message << std::endl;
}

void printVector(const std::vector<float>& v, int itemsPerLine, int spacing){
    std::cout << "\n";
    int count = 1;
    for (auto i: v){
        if ((count % itemsPerLine) or itemsPerLine == -1){
            std::cout << std::setw(spacing) << i << ", ";
            ++count;
        }
        else{
            std::cout << std::setw(spacing) << i << "\n";
            ++count;
        }

        if(count > TOO_MUCH)
            break;
    }
    std::cout << "\n";
}

void printVector(const std::vector<int>& v, int itemsPerLine, int spacing){
    std::cout << "\n";
    int count = 1;
    for (auto i: v){
        if ((count % itemsPerLine) or itemsPerLine == -1){
            std::cout << std::setw(spacing) << i << ", ";
            ++count;
        }
        else{
            std::cout << std::setw(spacing) << i << "\n";
            ++count;
        }

        if(count > TOO_MUCH)
            break;
    }
    std::cout << "\n";
}

float round(float var, int digits) { 
    // 37.66666 * 100 =3766.66 
    // 3766.66 + .5 =3767.16    for rounding off value 
    // then type cast to int so value is 3767 
    // then divided by 100 so the value converted into 37.67 
    float value = int((var * (digits * 10) + .5));
    return float(value / (digits * 10));
}

// Stolen from pygame
/*
static int set_at(SDL_Surface *surf, int x, int y, Uint32 color, int *drawn_area){
    SDL_PixelFormat *format = surf->format;
    Uint8 *pixels = (Uint8 *)surf->pixels;
    Uint8 *byte_buf, rgb[4];

    if (x < surf->clip_rect.x || x >= surf->clip_rect.x + surf->clip_rect.w ||
        y < surf->clip_rect.y || y >= surf->clip_rect.y + surf->clip_rect.h)
        return 0;

    switch (format->BytesPerPixel) {
        case 1:
            *((Uint8 *)pixels + y * surf->pitch + x) = (Uint8)color;
            break;
        case 2:
            *((Uint16 *)(pixels + y * surf->pitch) + x) = (Uint16)color;
            break;
        case 4:
            *((Uint32 *)(pixels + y * surf->pitch) + x) = color;
            break;
        default: /*case 3:
            SDL_GetRGB(color, format, rgb, rgb + 1, rgb + 2);
            byte_buf = (Uint8 *)(pixels + y * surf->pitch) + x * 3;
#if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            *(byte_buf + (format->Rshift >> 3)) = rgb[0];
            *(byte_buf + (format->Gshift >> 3)) = rgb[1];
            *(byte_buf + (format->Bshift >> 3)) = rgb[2];
#else
            *(byte_buf + 2 - (format->Rshift >> 3)) = rgb[0];
            *(byte_buf + 2 - (format->Gshift >> 3)) = rgb[1];
            *(byte_buf + 2 - (format->Bshift >> 3)) = rgb[2];
#endif
            break;
    }
    add_pixel_to_drawn_list(x, y, drawn_area);
    return 1;
}
*/

/* 
    void printVector(const std::vector<Point>& v, int itemsPerLine, int spacing){
        std::cout << "\n";
        int count = 1;
        for (auto i: v){
            if ((count % itemsPerLine) or itemsPerLine == -1){
                std::cout << '(' << i.x << ", " << i.y << "), ";
                ++count;
            }
            else{
                std::cout << '(' << i.x << ", " << i.y << ")\n";
                ++count;
            }

            if(count > TOO_MUCH)
                break;
        }
        std::cout << "\n";
    }
 */
} // namespace g
