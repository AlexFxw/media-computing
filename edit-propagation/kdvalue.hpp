/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 05:51:53
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-04 12:33:30
 * @Description: Define k-d value 
 */

#ifndef EDIT_KDVALUE_H
#define EDIT_KDVALUE_H

#include <cstdint>

class KDValue
{
public:
    virtual int get_value(const int &dim) const = 0;
};

class ImageKD : public KDValue
{
protected:
    static const int dimension = 5;

public:
    uint8_t r, g, b;
    uint16_t w, h;
    ImageKD() {}
    ImageKD(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _w, uint16_t _h): r(_r), g(_g), b(_b), w(_w), h(_h) {} 
    ImageKD operator+(const ImageKD &kd) {
        return ImageKD(r + kd.r, g + kd.g, b + kd.b, w + kd.w, h + kd.h);
    }
    ImageKD operator/(const int &n) {
        return ImageKD(r / n, g / n, b / n, w / n, h / n);
    }
    ~ImageKD() {}
    static int dim() { return dimension; }
    int get_value(const int &dim) const
    {
        if (dim == 0) { return r; }
        else if (dim == 1) { return g; }
        else if (dim == 2) { return b; }
        else if (dim == 3) { return w; }
        else if (dim == 4) { return h; }
        else { return 0; }
    }
    void set_value(const int &dim, int value)
    {
        if (dim == 0) { r = value; } 
        else if (dim == 1) { g = value; } 
        else if (dim == 2) { b = value; } 
        else if (dim == 3) { w = value; } 
        else if (dim == 4) { h = value; } 
    }
};

#endif // EDIT_KDVALUE_H