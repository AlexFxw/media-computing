/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2019-12-20 01:38:57
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-09 17:27:22
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
    if (_src.load(src_path) || _target.load(target_path) || _res.load(target_path))
    {
        // Fail to load image
        return 1;
    }
    std::vector<std::pair<int, int>> swatch_samples;
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

        if (_target.jitter_sampling(50, sample_arr, s.target_minw, s.target_maxw, s.target_minh, s.target_maxh))
        {
            printf("Fail to sample swatch\n");
            return 1;
        }
        for (auto &s : sample_arr)
        {
            swatch_samples.push_back(s);
        }
    }
    int process_bar = 0;
#pragma omp parallel for
    for (int w = 0; w < _target.width(); w++)
    {
        for (int h = 0; h < _target.height(); h++)
        {
            process_bar++;
            printf("\rSwatch transfer process: %.2f %%", ((double)process_bar * 100.0 / (double)(_target.width() * _target.height())));
            double e = Utils::MAX_E;
            int neighbor_size = 2;
            // Error function:
            int best_w = w, best_h = h;
            for (auto s : swatch_samples)
            {
                int cnt = 0;
                double tmp_e = 0;
                int sample_w = s.first;
                int sample_h = s.second;
                for (int wi = -neighbor_size; wi <= neighbor_size; wi++)
                {
                    for (int hi = -neighbor_size; hi <= neighbor_size; hi++)
                    {
                        int sw = sample_w + wi;
                        int sh = sample_h + hi;
                        int tw = w + wi;
                        int th = h + hi;
                        uint8_t s_val, t_val;
                        if (_target.get_pixel(sw, sh, CHANNEL::L, s_val) || _target.get_pixel(tw, th, CHANNEL::L, t_val)) {
                            continue;
                        }
                        cnt ++;
                        tmp_e += (s_val - t_val) * (s_val - t_val);
                    }
                }
                if(cnt > 0) {
                    tmp_e /= (double)cnt;
                    if(tmp_e < e)
                    {
                        e = tmp_e;
                        best_w = sample_w;
                        best_h = sample_h;
                    }
                }
            }
            uint8_t a = 0, b = 0;
            if (_target.get_pixel(best_w, best_h, CHANNEL::A, a) || _target.get_pixel(best_w, best_h, CHANNEL::B, b))
            {
                printf("Fail to get source image channel.\n");
            }
            _res.set_pixel(w, h, A, a);
            _res.set_pixel(w, h, B, b);
        }
    }
    printf("\n");
    if (_res.save(res_path))
    {
        printf("Fail to save image.\n");
        return 1;
    }
}


#endif // !COLORIZE_TRANSFERER_H