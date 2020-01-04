/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 05:45:20
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-04 11:00:30
 * @Description: Handling the photos and video.
 */
#ifndef EDIT_MEDIA_HANDLER_HPP
#define EDIT_MEDIA_HANDLER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <string>

enum CHANNEL
{
    R = 0,
    G = 1,
    B = 2,
    ALPHA = 3
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
        channels = data.channels;
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
    uint8_t get_pixel(const int &w, const int &h, CHANNEL channel)
    {
        if (w < 0 || w >= width || h < 0 || h >= height)
        {
            printf("Invalid w, h in getting pixel\n");
            return 0;
        }
        return pixel_ptr[h * width * channels + w * channels + (int)channel];
    }
};

#endif // EDIT_MEDIA_HANDLER_HPP