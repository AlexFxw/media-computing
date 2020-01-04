/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2019-12-19 11:02:27
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-04 06:46:17
 * @Description: This header is used to deal with image files.
 */
#ifndef COLORIZE_IMAGE_HANDLER
#define COLORIZE_IMAGE_HANDLER

#include <sys/stat.h>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
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
    const double MAX_E = 1 << 10;
public:
    LabImage(/* args */);
    ~LabImage();
    int load(const std::string& file_name);
    double mean(const CHANNEL& channel = L);
    double mean(const std::vector<std::pair<int, int>>& pixel_arr,const CHANNEL& channel);
    double mean(const int& min_w, const int& max_w, const int& min_h, const int& max_h, const CHANNEL& channel);
    double sig(const CHANNEL& channel = L);
    double sig(const std::vector<std::pair<int, int>>& pixel_arr, const CHANNEL& channel);
    double sig(const int& min_w, const int& max_w, const int& min_h, const int& max_h, const CHANNEL& channel);
    int remap_histogram(LabImage& target, const CHANNEL& channel = L);
    int save(const std::string& file_name);
    int get_pixel(const int& w, const int& h, const CHANNEL& channel, uint8_t& value) const;
    int set_pixel(const int& w, const int& h, const CHANNEL& channel, const uint8_t new_value);
    int jitter_sampling(const int& sample_num, std::vector<std::pair<int, int>> &res_arr) const;
    int calc_neighbor(const int& w, const int&h, const int& size, double &mean_value, double &sig_value);
    int transfer(LabImage& src_img, const std::vector<std::pair<int, int>>& res_arr, const int& size, const double& luminance_ratio);
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
    return cv::imwrite(file_name, image) == false;
}

double LabImage::mean(const CHANNEL& channel)
{
    if(_mean[channel] < 0) {
        _mean[channel] = this->mean(0, _width, 0, _height, channel);
    }
    return _mean[channel];
}

double LabImage::mean(const std::vector<std::pair<int, int>>& pixel_arr,const CHANNEL& channel = L)
{
    double avg = 0;
    uint8_t pixel_val = 0;
    int w = 0, h = 0;
    for(auto iter = pixel_arr.begin(); iter != pixel_arr.end(); iter ++) 
    {
        w = iter->first;
        h = iter->second;
        // FIXME: Not handle the error now.
        this->get_pixel(w, h, channel, pixel_val);
        avg += pixel_val;
    }
    avg /= (double)pixel_arr.size();
    return avg;
}

double LabImage::mean(const int& min_w, const int& max_w, const int& min_h, const int& max_h, const CHANNEL& channel = L)
{
    double avg = 0; 
    uint8_t pixel_val = 0;
    int cnt = 0;
    for(int h = min_h; h < max_h; h ++)
    {
        for(int w = min_w; w < max_w; w ++)
        {
            if(get_pixel(w, h, channel, pixel_val)) {
                continue;
            }
            avg += pixel_val;
            cnt ++;
        }
    }
    avg /= (double)cnt;
    return avg;   
}

double LabImage::sig(const CHANNEL& channel)
{
    if(_sig[channel] < 0) {
        _sig[channel] = this->sig(0, _width, 0, _height, channel);
    }
    return _sig[channel];
}

double LabImage::sig(const std::vector<std::pair<int, int>>& pixel_arr, const CHANNEL& channel = L)
{
    double mean_value = this->mean(pixel_arr, channel);
    double sig_value = 0;
    uint8_t pixel_val = 0;
    int w = 0, h = 0;
    for(auto iter = pixel_arr.begin(); iter != pixel_arr.end(); iter ++) {
        w = iter->first;
        h = iter->second;
        this->get_pixel(w, h, channel, pixel_val);
        sig_value += pow(mean_value - (double)pixel_val, 2);
    }
    sig_value /= (double)pixel_arr.size();
    return sqrt(sig_value);
}

double LabImage::sig(const int& min_w, const int& max_w, const int& min_h, const int& max_h, const CHANNEL& channel = L)
{
    uint8_t pixel_val = 0;
    double mean_value = this->mean(channel);
    double sum = 0, row_sum = 0;
    for(int h = min_h; h < max_h; h ++)
    {
        row_sum = 0;
        for(int w = min_w; w < max_w; w ++)
        {
            if(get_pixel(w, h, channel, pixel_val)) {
                continue;
            }
            row_sum += pow(pixel_val - mean_value, 2);
        }
        sum += row_sum / (double)(max_w - min_w);
    }
    sum /= (double)(max_h - min_h);
    return sqrt(sum);
}

int LabImage::get_pixel(const int& w, const int& h, const CHANNEL& channel, uint8_t& value) const
{
    if(w < 0 || w >= _width || h < 0 || h >= _height) {
        printf("Invalid w, h in getting pixel\n");
        return 1;
    }
    value = _pixel_ptr[h*_width*_channels + w*_channels + (int)channel];
    return 0;
}

int LabImage::set_pixel(const int& w, const int& h, const CHANNEL& channel, uint8_t new_value)
{
    if(w < 0 || w >= _width || h < 0 || h >= _height) {
        printf("Invalid w, h in getting pixel\n");
        return 1;
    }
    _pixel_ptr[h*_width*_channels + w*_channels + (int)channel] = new_value;
    return 0;
}

/**
 * @description: To remap the image's histogram in certain channel to the target image.
 * @param {LabImage, CHANNEL(int)} The target image; channel  
 * @return: 0 - success; 1 - fail
 */
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

int LabImage::calc_neighbor(const int& w, const int&h, const int& size, double &mean_value, double &sig_value)
{
    int step = floor((double)size/2.0);
    int start_w = std::max(0, w - step), end_w = std::min(_width-1, w + step);
    int start_h = std::max(0, h - step), end_h = std::min(_height-1, h + step);
    mean_value = this->mean(start_w, end_w, start_h, end_h);
    sig_value = this->sig(start_w, end_w, start_h, end_h);
    return 0;
}

int LabImage::transfer(LabImage& src_img, const std::vector<std::pair<int, int>>& pixel_arr, const int& size, const double& luminance_ratio)
{
    const double src_sig_val = src_img.sig(pixel_arr, L);
#pragma omp parallel for
    for(int w = 0; w < _width; w ++)
    {
        for(int h = 0; h < _height; h ++)
        {
            int src_w = 0, src_h = 0;
            uint8_t pixel_val = 0, src_pixel_val = 0;
            double mean_val = 0, sig_val = 0;
            int best_w = 0, best_h = 0;
            double e = MAX_E;
            if(this->get_pixel(w, h, L, pixel_val)
            || this->calc_neighbor(w, h, size, mean_val, sig_val)){
                printf("Fail to transfer color.\n");
            }
            for(auto iter = pixel_arr.begin(); iter != pixel_arr.end(); iter ++)
            {
                src_w = iter->first;
                src_h = iter->second;
                src_img.get_pixel(src_w, src_h, L, src_pixel_val);
                double tmp_e = (double)abs(pixel_val - src_pixel_val) * luminance_ratio 
                    + (double)abs(sig_val - src_sig_val) * (1.0 - luminance_ratio);
                if(tmp_e < e) {
                    e = tmp_e;
                    best_w = src_w;
                    best_h = src_h;
                }
            }

            uint8_t a = 0, b = 0;
            if(src_img.get_pixel(best_w, best_h, A, a)
            || src_img.get_pixel(best_w, best_h, B, b)){
                printf("Fail to get source image channel.\n");
            }
            this->set_pixel(w, h, A, a);
            this->set_pixel(w, h, B, b);
        }
    }
    return 0;
}

#endif // !1COLORIZE_IMAGE_HANDLER