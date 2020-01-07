/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 05:51:53
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 12:16:34
 * @Description: Define k-d value 
 */

#ifndef EDIT_KDVALUE_H
#define EDIT_KDVALUE_H

#include <cstdint>
#include <ostream>

enum KDIndex
{
    r = 0,
    g = 1,
    b = 2,
    w = 3,
    h = 4
};

class KDValue
{
public:
    virtual int get_value(const int &dim) const = 0;
    virtual void set_value(const int &dim, int value) = 0;
};

class ImageKD : public KDValue
{
protected:
    static const int dimension = 5;

public:
    uint8_t r, g, b;
    uint16_t w, h;
    ImageKD() {}
    ImageKD(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _w, uint16_t _h) : r(_r), g(_g), b(_b), w(_w), h(_h) {}
    ImageKD create_new(const int &pivot, const int &new_value) const
    {
        ImageKD new_kd = *this;
        new_kd.set_value(pivot, new_value);
        return new_kd;
    }
    ImageKD operator+(const ImageKD &kd)
    {
        return ImageKD(unsigned(r) + unsigned(kd.r), unsigned(g) + unsigned(kd.g), unsigned(b + kd.b), w + kd.w, h + kd.h);
    }
    ImageKD operator/(const int &n)
    {
        return ImageKD(unsigned(r) / n, unsigned(g) / n, unsigned(b) / n, w / n, h / n);
    }
    friend std::ostream &operator<<(std::ostream &output, const ImageKD &kd)
    {
        output << "r: " << unsigned(kd.r) << ", g: " << unsigned(kd.g) << ", b: " << unsigned(kd.b) << ", w: " << kd.w << ", h: " << kd.h;
        return output;
    }
    ~ImageKD() {}
    static int dim() { return dimension; }
    int get_value(const int &dim) const
    {
        if (dim == 0)
            return unsigned(r);
        else if (dim == 1)
            return unsigned(g);
        else if (dim == 2)
            return unsigned(b);
        else if (dim == 3)
            return w;
        else if (dim == 4)
            return h;
        else
            return 0;
    }
    void set_value(const int &dim, int value)
    {
        if (dim == 0)
            r = value;
        else if (dim == 1)
            g = value;
        else if (dim == 2)
            b = value;
        else if (dim == 3)
            w = value;
        else if (dim == 4)
            h = value;
    }
    bool operator<(const ImageKD &c) const
    {
        for (int i = 0; i < dimension; i++)
        {
            if (this->get_value(i) < c.get_value(i))
                return true;
            else if (this->get_value(i) > c.get_value(i))
                return false;
        }
        return true;
    }
};

#endif // EDIT_KDVALUE_H