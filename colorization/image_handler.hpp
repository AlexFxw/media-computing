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
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <ctime>

enum CHANNEL {
    L = 0,
    A = 1,
    B = 2
};

class LabImage
{
private:
    /* data */
    cv::Mat image;
    /* No need to delete _pixel_ptr in destruction funciton because the memory will be released by OpenCV */
    uint8_t *_pixel_ptr;  
    int _width, _height, _channels;
    double _mean[3], _sig[3];
public:
    LabImage(/* args */);
    ~LabImage();
    int load(const std::string& file_name);
    double mean(const CHANNEL& channel = L);
    double sig(const CHANNEL& channel = L);
    int remap_histogram(LabImage& target, const CHANNEL& channel = L);
    int save(const std::string& file_name);
    int get_pixel(const int& w, const int& h, const CHANNEL& channel, uint8_t& value) const;
    int set_pixel(const int& w, const int& h, const CHANNEL& channel, const uint8_t new_value);
    int jitter_sampling(const int& sample_num, std::vector<std::pair<int, int>> &res_arr) const;
};

LabImage::LabImage(/* args */)
{
    _width = 0;
    _height = 0;
    _pixel_ptr = nullptr;
    _channels = 3;  // 3 in lab image
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
    for(int i = 0; i < 3; i ++) {
        _mean[i] = -1;
        _sig[i] = -1;
    }
    _pixel_ptr = (uint8_t*) image.data;
    return 0;
}

int LabImage::save(const std::string& file_name)
{
    cv::cvtColor(image, image, cv::COLOR_Lab2BGR);
    return cv::imwrite(file_name, image) == true;
}

double LabImage::mean(const CHANNEL& channel)
{
    if(_mean[channel] >= 0) {
        return _mean[channel];
    }
    double avg = 0; 
    uint32_t row_sum = 0;
    uint8_t pixel_val = 0;
    for(int h = 0; h < _height; h ++)
    {
        for(int w = 0; w < _width; w ++)
        {
            if(get_pixel(w, h, channel, pixel_val)) {
                continue;
            }
            row_sum += pixel_val;
        }
        avg += (double)row_sum / (double)_width;
    }
    avg /= (double)_height;
    _mean[channel] = avg;
    return avg;
}

double LabImage::sig(const CHANNEL& channel)
{
    if(_sig[channel] >= 0) {
        return _sig[channel];
    }
    uint8_t pixel_val = 0;
    double mean_value = this->mean(channel);
    double sum = 0, row_sum = 0;
    for(int h = 0; h < _height; h ++)
    {
        row_sum = 0;
        for(int w = 0; w < _width; w ++)
        {
            if(get_pixel(w, h, channel, pixel_val)) {
                continue;
            }
            row_sum += pow(pixel_val - mean_value, 2);
        }
        sum += row_sum / (double)_width;
    }
    sum /= (double)_height;
    double sig = sqrt(sum);
    _sig[channel] = sig;
    return sig;
}

int LabImage::get_pixel(const int& w, const int& h, const CHANNEL& channel, uint8_t& value) const
{
    if(w < 0 || w >= _width || h < 0 || h >= _height) {
        printf("Invalid w, h in getting pixel\n");
        return 1;
    }
    value = _pixel_ptr[h*_height*_channels + w*_channels + (int)channel];
    return 0;
}

int LabImage::set_pixel(const int& w, const int& h, const CHANNEL& channel, uint8_t new_value)
{
    if(w < 0 || w >= _width || h < 0 || h >= _height) {
        printf("Invalid w, h in getting pixel\n");
        return 1;
    }
    _pixel_ptr[h*_height*_channels + w*_channels + (int)channel] = new_value;
    return 0;
}

int LabImage::remap_histogram(LabImage& target, const CHANNEL& channel)
{
    uint8_t pixel_val = 0;
    double src_mean = this->mean(channel), target_mean = target.mean(channel);
    double src_sig = this->sig(channel), target_sig = target.sig(channel);
    double ratio = target_sig / src_sig;
    for(int h = 0; h < _height; h ++)
    {
        for(int w = 0; w < _width; w++)
        {
            if(this->get_pixel(w, h, channel, pixel_val)) {
                printf("Fail to remap histogram.\n");
                return 1;
            }
            pixel_val = (uint8_t)(ratio * (pixel_val - src_mean) + target_mean);
            set_pixel(w, h, channel, pixel_val);
        }
    }
    return 0;
}

int LabImage::jitter_sampling(const int& sample_num, std::vector<std::pair<int, int>> &res_arr) const
{
    res_arr.clear();
    int n = sqrt(sample_num);
    int h_step = (int)(_height / n), w_step = (int)(_width / n);
    std::default_random_engine random(time(NULL));
    std::uniform_int_distribution<int> u_rand(1, h_step * w_step);
    int value = 0, next_w = 0, next_h = 0;
    int tmp_w = 0, tmp_h = 0;
    uint8_t pixel_val = 0;
    for(int h = 0; h < _height; h += h_step)
    {
        for(int w = 0; w < _width; w += w_step)
        {
            value = u_rand(random);
            tmp_h = ceil(value / w_step);
            tmp_w = value - (next_h - 1) * w_step;
            next_h = h + tmp_h - 1;
            next_w = w + tmp_w - 1;
            if(this->get_pixel(w, h, L, pixel_val)) {
                printf("Overflow in jitter sampling.\n");
                return 1;
            }
            res_arr.push_back(std::pair<int, int>(w, h));
        }
    }
    return 0;
}

#endif // !1COLORIZE_IMAGE_HANDLER