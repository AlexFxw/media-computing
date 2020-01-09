/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2019-12-20 01:38:57
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-09 16:37:40
 * @Description: Transferer provides API to transfer color to a gray scale image.
 */
#ifndef COLORIZE_TRANSFERER_H
#define COLORIZE_TRANSFERER_H

#include <string>
#include <vector>
#include "utils.hpp"
#include "image_handler.hpp"

class Transferer
{
private:
    LabImage _src, _target, _res;
    int _sample_num;
    double _luminance_raio;

public:
    Transferer();
    int colorize(const std::string &src_path, const std::string &target_path, const std::string &res_path);
    int colorize(const std::string &src_path, const std::string &target_path, const std::string &res_path, std::vector<Swatch> &swatches);
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

int Transferer::colorize(const std::string &src_path, const std::string &target_path, const std::string &res_path)
{
    if (_src.load(src_path) || _target.load(target_path))
    {
        // Fail to load image
        return 1;
    }

    if (_target.remap_histogram(_src, CHANNEL::L))
    {
        return 1;
    }
    printf("Remapped the image histogram.\n");

    std::vector<std::pair<int, int>> sample_arr;
    if (_src.jitter_sampling(_sample_num, sample_arr))
    {
        printf("Fail to sample source image\n");
        return 1;
    }
    printf("Jitter sampled.\n");

    if (_target.transfer(_src, sample_arr, 5, 0.5))
    {
        printf("Fail to transfer color.\n");
        return 1;
    }

    if (_target.save(res_path))
    {
        printf("Fail to save image.\n");
        return 1;
    }

    return 0;
}

int Transferer::colorize(const std::string &src_path, const std::string &target_path, const std::string &res_path, std::vector<Swatch> &swatches)
{
    if (_src.load(src_path) || _target.load(target_path))
    {
        // Fail to load image
        return 1;
    }

    for (auto &s : swatches)
    {
        if (_target.remap_histogram(_src, s, CHANNEL::L))
        {
            return 1;
        }
        std::vector<std::pair<int, int>> sample_arr;
        // 50 pixel in a swatch
        if (_src.jitter_sampling(50, sample_arr, s.src_minw, s.src_maxw, s.src_minh, s.src_maxh))
        {
            printf("Fail to sample source image\n");
            return 1;
        }
        if (_target.transfer(_src, sample_arr, 5, 0.5, s.target_minw, s.target_maxw, s.target_minh, s.target_maxh))
        {
            printf("Fail to transfer color.\n");
            return 1;
        }
    }

    if (_target.save(res_path))
    {
        printf("Fail to save image.\n");
        return 1;
    }
}


#endif // !COLORIZE_TRANSFERER_H