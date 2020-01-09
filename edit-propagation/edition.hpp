/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 11:00:53
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-09 07:19:14
 * @Description: The class for edition. 
 */
#ifndef EDIT_EDITION_HPP
#define EDIT_EDITION_HPP

#include "media_handler.hpp"
#include "kdvalue.hpp"
#include "utils.hpp"

class Edition
{
protected:
    Image *image;
    ImageKD *center; // The center of the edition
    Float *coof;
    int width, height;

public:
    Edition(const std::string &img_path);
    ~Edition();
    Float distance(const ImageKD &pixel) const;
    Float distance(const VideoKD &pixel) const;
    Float get_coof(int w, int h) const;
    void transfer(const Float &cf, int r, int g, int b, int &new_r, int &new_g, int &new_b);
    std::vector<ImageKD> get_imagekd(Image &img) const;
    // FIXME: 0 or 1?
    std::vector<VideoKD> get_videokd(const std::string &video_name, int edit_frame = 1) const;
};

Edition::Edition(const std::string &img_path)
{
    // Single region edition now.
    image = new Image(img_path);
    width = image->width, height = image->height;
    coof = new Float[width * height];
    Float r = 0, g = 0, b = 0, w = 0, h = 0;
    int cnt = 0;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int _r = image->get_pixel(i, j, CHANNEL::R);
            int _g = image->get_pixel(i, j, CHANNEL::G);
            int _b = image->get_pixel(i, j, CHANNEL::B);
            if (_r == 255 && _g == 255 && _r == 255)
            {
                coof[j * width + i] = 0;
                continue;
            }
            // TODO: Find a better mapping method.
            // coof[j * width + i] = ((Float)_r / 255.0 + (Float)_g / 255.0 + (Float)_b / 255.0) / 3.0;
            coof[j * width + i] = Utils::init_e;
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

Edition::~Edition()
{
    delete image;
    delete center;
    delete coof;
}

Float Edition::distance(const ImageKD &pixel) const
{
    int _r = center->r - pixel.r;
    int _g = center->g - pixel.g;
    int _b = center->b - pixel.b;
    int _w = center->w - pixel.w;
    int _h = center->h - pixel.h;
    Float dist = _r * _r + _g * _g + _b * _b + _w * _w + _h * _h;
    dist = sqrt(dist);
    return dist;
}

Float Edition::distance(const VideoKD &pixel) const
{
    int _r = center->r - pixel.r;
    int _g = center->g - pixel.g;
    int _b = center->b - pixel.b;
    int _w = center->w - pixel.w;
    int _h = center->h - pixel.h;
    int _t = Utils::edit_frame - pixel.t;
    Float dist = _r * _r + _g * _g + _b * _b + _w * _w + _h * _h + _t * _t;
    dist = sqrt(dist);
    return dist;
}

Float Edition::get_coof(int w, int h) const
{
    if (w < 0 || w >= width || h < 0 || h >= height)
    {
        return 0.0;
    }
    return coof[h * width + w];
}

void Edition::transfer(const Float &cf, int r, int g, int b, int &new_r, int &new_g, int &new_b)
{
    // TODO: there may be a better mapping function?
    new_g = std::min(255, g + (int)(cf * 64.0));
    new_b = (int)((Float)b * cf);
}

std::vector<ImageKD> Edition::get_imagekd(Image &img) const
{
    std::vector<ImageKD> img_kdvalue;
    for (int w = 0; w < img.width; w++)
    {
        for (int h = 0; h < img.height; h++)
        {
            int r = img.get_pixel(w, h, CHANNEL::R);
            int g = img.get_pixel(w, h, CHANNEL::G);
            int b = img.get_pixel(w, h, CHANNEL::B);
            Float e = this->get_coof(w, h);
            Float weight = e > Utils::EPS ? Utils::w : 0;
            img_kdvalue.push_back(ImageKD(r, g, b, w, h, e, weight));
        }
    }
    return img_kdvalue;
}

std::vector<VideoKD> Edition::get_videokd(const std::string &video_name, int edit_frame) const
{
    std::vector<VideoKD> vec;
    Video video;
    if (!video.open(video_name))
    {
        printf("Fail to open video.\n");
        return vec;
    }
    Image frame;
    int cur_frame = 1;
    int width = 0, height = 0;
    while (video.get_next_frame(frame))
    {
        printf("cur_frame: %d\n", cur_frame);
        width = frame.width, height = frame.height;
        for (int w = 0; w < width; w++)
        {
            for (int h = 0; h < height; h++)
            {
                int r = frame.get_pixel(w, h, CHANNEL::R);
                int g = frame.get_pixel(w, h, CHANNEL::G);
                int b = frame.get_pixel(w, h, CHANNEL::B);
                Float e = (cur_frame != edit_frame) ? 0 : this->get_coof(w, h);
                Float weight = e > Utils::EPS ? Utils::w : 0;
                vec.push_back(VideoKD(r, g, b, w, h, cur_frame, e, weight));
            }
        }
        frame.data.release();
        cur_frame++;
    }
    Utils::video_length = cur_frame;
    Utils::width = width;
    Utils::height = height;
    return vec;
}

#endif // EDIT_EDITION_HPP