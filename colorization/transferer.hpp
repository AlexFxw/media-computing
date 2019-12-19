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

public:
    Transferer();
    int colorize(const std::string& src_path, const std::string& target_path, const std::string& res_path);
    int align_images();
    ~Transferer();
};

Transferer::Transferer()
{
    _sample_num = 200;
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

    std::vector<std::pair<int, int>> sample_arr;
    if(_src.jitter_sampling(_sample_num, sample_arr)) {
        return 1;
    }
    if(_target.remap_histogram(_src)) {
        return 1;
    }
    return 0;
}

#endif // !COLORIZE_TRANSFERER_H