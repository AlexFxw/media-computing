/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-07 06:22:02
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 06:33:13
 * @Description: The definition of corner points
 */

#ifndef EDIT_CORNER_HPP
#define EDIT_CORNER_HPP

#include <map>

struct Vec3
{
    double x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
    Vec3 operator+(const Vec3 &v) { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator/(const double &d) { return Vec3(x / d, y / d, z / d); }
};

template <class T>
class Corners:
{
private:
    std::map<T, Vec3> editions;

public:
    Corners() {}
    ~Corners() {}
    void init_corner(const T &corner);
    int add_cornder(const T &corner, const Vec3 &value);
};

template <class T>
void Corners<T>::init_corner(const T &corner)
{
    editions[corner] = Vec3();
}

template <class T>
int Corners<T>::add_cornder(const T &corner, const Vec3 &value)
{
    editions[corner] = editions[corner] + value;
}

#endif // EDIT_CORNER_HPP