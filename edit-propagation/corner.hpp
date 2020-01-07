/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-07 06:22:02
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 10:48:13
 * @Description: The definition of corner points
 */

#ifndef EDIT_CORNER_HPP
#define EDIT_CORNER_HPP

#include <map>

struct CornerInfo
{
    double e, w, g;
    CornerInfo() : e(0), w(0), g(0) {}
    CornerInfo(double _e, double _w, double _g) : e(_e), w(_w), g(_g) {}
    CornerInfo operator+(const CornerInfo &v) { return CornerInfo(e + v.e, w + v.w, g + v.g); }
    CornerInfo operator/(const double &d) { return CornerInfo(e / d, w / d, g / d); }
};

template <class T>
class Corners
{
private:
    std::map<T, CornerInfo> editions;

public:
    Corners() {}
    ~Corners() {}
    void set_corner(const T &corner, const CornerInfo& value = CornerInfo());
    int add_cornder(const T &corner, const CornerInfo &value);
    CornerInfo get_value(const T &corner) const;
};

template <class T>
void Corners<T>::set_corner(const T &corner, const CornerInfo& value)
{
    editions[corner] = value;
}

template <class T>
int Corners<T>::add_cornder(const T &corner, const CornerInfo &value)
{
    editions[corner] = editions[corner] + value;
}

#endif // EDIT_CORNER_HPP