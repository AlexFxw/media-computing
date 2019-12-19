/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2019-12-19 11:02:27
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2019-12-19 11:37:33
 * @Description: This header is used to deal with image files.
 */
#ifndef COLORIZE_IMAGE_HANDLER
#define COLORIZE_IMAGE_HANDLER

#include <sys/stat.h>
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>


enum CHANNEL {
    L, A, B
};

class LabImage
{
private:
    /* data */
    cv::Mat image;
    int _width;
    int _height;
public:
    LabImage(/* args */);
    ~LabImage();
    int load(const std::string& file_name);
    int save(const std::string& file_name);
};

LabImage::LabImage(/* args */)
{
    _width = 0;
    _height = 0;
}

LabImage::~LabImage()
{
}

int LabImage::load(const std::string& file_name)
{
    struct stat buffer;
    if(stat(file_name.c_str(), &buffer) != 0) {
        return 1;
    }
    image = cv::imread(file_name, cv::IMREAD_COLOR);
    cv::cvtColor(image, image, cv::COLOR_BGR2Lab);
    _height = image.rows;
    _width = image.cols; 
    return 0;
}

int LabImage::save(const std::string& file_name)
{
    cv::cvtColor(image, image, cv::COLOR_Lab2BGR);
    return cv::imwrite(file_name, image) == true;
}

#endif // !1COLORIZE_IMAGE_HANDLER