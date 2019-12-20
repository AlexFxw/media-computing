/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2019-12-20 01:38:57
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2019-12-20 07:56:01
 * @Description: Transferer provides API to transfer color to a gray scale image.
 */
#ifndef COLORIZE_TRANSFERER_H
#define COLORIZE_TRANSFERER_H

#include <string>
#include <vector>
#include "image_handler.hpp"

class Transferer
{
private:
    LabImage _src, _target, _res;
    int _sample_num;
    double _luminance_raio;

public:
    Transferer();
    int colorize(const std::string& src_path, const std::string& target_path, const std::string& res_path);
    ~Transferer();
};

Transferer::Transferer()
{
    _sample_num = 200;
    _luminance_raio = 0.5;
}

Transferer::~Transferer()
{
}

int Transferer::colorize(const std::string& src_path, const std::string& target_path, const std::string& res_path)
{
    if(_src.load(src_path) || _target.load(target_path)) {
        // Fail to load image
        return 1;
    }

    if(_target.remap_histogram(_src)) {
        return 1;
    }

    std::vector<std::pair<int, int>> sample_arr;
    if(_src.jitter_sampling(_sample_num, sample_arr)) {
        printf("Fail to sample source image\n");
        return 1;
    }

    if(_target.transfer(_src, sample_arr, 5, 0.5)) {
        printf("Fail to transfer color.\n");
        return 1;
    }

    if(_target.save(res_path)) {
        printf("Fail to save image.\n");
        return 1;
    }

    return 0;
}

#endif // !COLORIZE_TRANSFERER_H