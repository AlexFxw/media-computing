/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 11:00:53
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-06 03:19:02
 * @Description: The class for edition. 
 */
#ifndef EDIT_EDITION_HPP
#define EDIT_EDITION_HPP

#include <cmath>
#include <string>
#include "media_handler.hpp"
#include "kdvalue.hpp"

class Edition
{
protected:
    Image *image;
    ImageKD *center; // The center of the edition

public:
    Edition(const std::string &img_path)
    {
        // Single region edition now.
        image = new Image(img_path);
        int width = image->width, height = image->height;
        double r = 0, g = 0, b = 0, w = 0, h = 0;
        int cnt = 0;
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                int _r = image->get_pixel(i, j, CHANNEL::R);
                int _g = image->get_pixel(i, j, CHANNEL::G);
                int _b = image->get_pixel(i, j, CHANNEL::B);
                if (_r == 255 && _g == 255 && _r == 255)
                    continue;
                cnt++;
                r += _r;
                g += _g;
                b += _b;
                w += i;
                h += j;
            }
        }
        center = new ImageKD((uint8_t)(r / cnt), (uint8_t)(g / cnt), (uint8_t)(b / cnt), (uint16_t)(w / cnt), (uint16_t)(h / cnt));
    }
    ~Edition()
    {
        delete image;
        delete center;
    }
    double distance(const ImageKD &pixel) const
    {
        double dist = 0;
        dist += pow(center->r - pixel.r, 2);
        dist += pow(center->g - pixel.g, 2);
        dist += pow(center->b - pixel.b, 2);
        dist += pow(center->w - pixel.w, 2);
        dist += pow(center->h - pixel.h, 2);
        dist = sqrt(dist);
        std::cout << "distance of: " << pixel << " | " << dist << std::endl;
        return dist;
    }
};

#endif // EDIT_EDITION_HPP