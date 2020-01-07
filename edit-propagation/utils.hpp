/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 07:25:57
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 15:55:54
 * @Description: file content
 */

#ifndef EDIT_UTILS_HPP 
#define EDIT_UTILS_HPP

#include <cmath>

class Utils
{
public:
    template <class T>
        static double multi_interpolate(const T &point, const T &target, const T &opposite);
    template <class T>
        static T get_opposite_corner(const T &corner, const T &lower, const T &upper);
    static double w; // 和用户的编辑相关的权重
    static double lambda;
};

double Utils::w = 0.8;
double Utils::lambda = 0.7;

template <class T>
double Utils::multi_interpolate(const T &point, const T &target, const T &opposite)
{
    int dim = T::dim();
    double coord = 1.0;
    for (int i = 0; i < dim; i++)
    {
        coord *= (double)std::abs(point.get_value(i) - target.get_value(i));
        coord /= (double)std::abs(opposite.get_value(i) - target.get_value(i));
    }
    return coord;
}

template <class T>
T Utils::get_opposite_corner(const T &corner, const T &lower, const T &upper)
{
    T opposite = corner;
    int dim = T::dim();
    for (int i = 0; i < dim; i ++)
    {
        int l = lower.get_value(i), u = upper.get_value(i);
        if(opposite.get_value(i) == l)
        {
            opposite.set_value(i, u);
        }
        else {
            opposite.set_value(i, l);
        }
    }
    return opposite;
}

#endif