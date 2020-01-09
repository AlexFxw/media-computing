/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 05:45:20
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-09 03:53:01
 * @Description: Handling the photos and video.
 */
#ifndef EDIT_MEDIA_HANDLER_HPP
#define EDIT_MEDIA_HANDLER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <cstdint>
#include <string>

enum CHANNEL
{
    R = 0,
    G = 1,
    B = 2
};

class Image
{
protected:
    uint8_t *pixel_ptr;

public:
    cv::Mat data;
    int width, height, channels;
    Image() : pixel_ptr(NULL) {}
    Image(int w, int h);
    Image(std::string img_name);
    Image(cv::Mat mat);
    ~Image();
    int save(std::string img_name);
    uint8_t get_pixel(int w, int h, CHANNEL channel);
    int set_pixel(int w, int h, CHANNEL channel, const uint8_t &value);
};

class Video
{
protected:
    cv::VideoCapture cap;

public:
    int frame_num;
    Video() : frame_num(0) {}
    int width, height, channels;
    int open(const std::string &video_name);
    bool get_next_frame(Image &frame);
    int save_frame(const std::string &video_name, const std::string &res_name, int frame);
};

int Video::open(const std::string &video_name)
{
    cap = cv::VideoCapture(video_name);
    return cap.isOpened();
}

bool Video::get_next_frame(Image &frame)
{
    cv::Mat mat;
    cap >> mat;
    if (mat.empty())
    {
        cap.release();
        return false;
    }
    frame = Image(mat);
    frame_num++;
    mat.release();
    return true;
}

int Video::save_frame(const std::string &video_name, const std::string &res_name, int frame)
{
    if(!this->open(video_name))
    {
        printf("Fail to open the video.\n");
        return 1;
    }
    Image img;
    int cur_frame = 1;
    while(this->get_next_frame(img))
    {
        if(cur_frame == frame)
            break;
        cur_frame++;
    }

    img.save(res_name);
    return 0;
}

Image::Image(int w, int h) : width(w), height(h), channels(3)
{
    this->data = cv::Mat(h, w, CV_8UC3);
    pixel_ptr = (uint8_t *)this->data.data;
}

Image::~Image()
{
    data.release();
}

Image::Image(std::string img_name)
{
    data = cv::imread(img_name, cv::IMREAD_COLOR);
    pixel_ptr = (uint8_t *)this->data.data;
    height = data.rows;
    width = data.cols;
    channels = 3;
}

Image::Image(cv::Mat mat)
{
    data = mat;
    pixel_ptr = (uint8_t *)this->data.data;
    height = data.rows;
    width = data.cols;
    channels = 3;
}

int Image::save(std::string img_name)
{
    cv::imwrite(img_name.c_str(), data);
    return 0;
}

uint8_t Image::get_pixel(int w, int h, CHANNEL channel)
{
    if (w < 0 || w >= this->width || h < 0 || h >= this->height)
    {
        return 0;
    }
    return this->pixel_ptr[h * width * channels + w * channels + (int)channel];
}

int Image::set_pixel(int w, int h, CHANNEL channel, const uint8_t &value)
{
    if (w < 0 || w >= this->width || h < 0 || h >= this->height)
    {
        printf("Fail to set pixel, out of bound.\n");
        return 1;
    }
    this->pixel_ptr[h * width * channels + w * channels + (int)channel] = value;
    return 0;
}

#endif // EDIT_MEDIA_HANDLER_HPP