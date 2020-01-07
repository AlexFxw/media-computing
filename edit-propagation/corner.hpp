/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-07 06:22:02
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 13:46:47
 * @Description: The definition of corner points
 */

#ifndef EDIT_CORNER_HPP
#define EDIT_CORNER_HPP

#include <map>
#include <ostream>

struct CornerInfo
{
    double e, g, u;
    CornerInfo(double _e = 0, double _g = 0, double _u = 0) : e(_e), g(_g), u(_u) {}
    CornerInfo operator+(const CornerInfo &v) { return CornerInfo(e + v.e, g + v.g, u + v.u); }
    CornerInfo operator/(const double &d) { return CornerInfo(e / d, g / d, u / d); }
    friend std::ostream &operator<<(std::ostream &out, const CornerInfo &c)
    {
        out << "Corner info: "
            << "e- " << c.e << " g- " << c.g << " u- " << c.u;
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
CornerInfo Corners<T>::get_value(const T&corner)
{
    return editions[corner];
}
#endif // EDIT_CORNER_HPP