/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-09 13:33:45
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-09 13:35:27
 * @Description: file content
 */
#ifndef COLORIZE_UTILS_H
#define COLORIZE_UTILS_H

#include <vector>

struct Swatch
{
    int src_minw, src_maxw, src_minh, src_maxh;
    int target_minw, target_maxw, target_minh, target_maxh;
};

class Utils
{
public:
    static std::vector<Swatch> get_swatch(std::string &file_name);
};

#endif // !COLORIZE_UTILS_H
