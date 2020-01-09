/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-07 06:22:02
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-09 02:17:13
 * @Description: The definition of corner points
 */

#ifndef EDIT_CORNER_HPP
#define EDIT_CORNER_HPP

#include <map>
#include <ostream>
#include "utils.hpp"

struct CornerInfo
{
    Float e, g, u, weight;
    CornerInfo(Float _e = 0, Float _g = 0, Float _u = 0, Float _w = 0) : e(_e), g(_g), u(_u), weight(_w) {}
    CornerInfo operator+(const CornerInfo &v) { return CornerInfo(e + v.e, g + v.g, u + v.u, w + v.weight); }
    CornerInfo operator/(const Float &d) { return CornerInfo(e / d, g / d, u / d, weight / d); }
    friend std::ostream &operator<<(std::ostream &out, const CornerInfo &c)
    {
        out << "Corner info: "
            << "e- " << c.e << " g- " << c.g << " u- " << c.u << " weight- " << c.weight;
        return out;
    }
};

template <class T>
class Corners
{
public:
    Corners() {}
    ~Corners() {}
    std::map<T, CornerInfo> editions;
    void set_corner(const T &corner, const CornerInfo &value = CornerInfo());
    int add_corner(const T &corner, const CornerInfo &value);
    void set_e(const T &corner, Float &_e) { editions[corner].e = _e; }
    CornerInfo get_value(const T &corner);
};

template <class T>
void Corners<T>::set_corner(const T &corner, const CornerInfo &value)
{
    editions[corner] = value;
}

template <class T>
int Corners<T>::add_corner(const T &corner, const CornerInfo &value)
{
    editions[corner] = editions[corner] + value;
}

template <class T>
CornerInfo Corners<T>::get_value(const T &corner)
{
    return editions[corner];
}
#endif // EDIT_CORNER_HPP