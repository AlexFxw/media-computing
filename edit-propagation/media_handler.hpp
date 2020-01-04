/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 05:45:20
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-04 12:28:07
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
    cv::Mat data;
    uint8_t *pixel_ptr;

public:
    int width, height, channels;
    Image(std::string img_name)
    {
        data = cv::imread(img_name.c_str(), cv::IMREAD_COLOR);
        pixel_ptr = (uint8_t *)data.data;
        height = data.rows;
        width = data.cols;
        channels = 3;
    }
    ~Image()
    {
        data.release();
    }
    int save(std::string img_name)
    {
        cv::imwrite(img_name.c_str(), data);
        return 0;
    }
    uint8_t get_pixel(int w, int h, CHANNEL channel)
    {
        if (w < 0 || w >= this->width || h < 0 || h >= this->height)
        {
            return 0;
        }
        return (uint8_t)this->pixel_ptr[h * width * channels + w * channels + (int)channel];
    }
};

#endif // EDIT_MEDIA_HANDLER_HPP