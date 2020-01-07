/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 05:51:53
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 14:12:02
 * @Description: Define k-d value 
 */

#ifndef EDIT_KDVALUE_H
#define EDIT_KDVALUE_H

#include <cstdint>
#include <ostream>
#include <cmath>

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
    double e;
    KDValue(double _e = 0.0) : e(_e) {}
    virtual int get_value(const int &dim) const = 0;
    virtual void set_value(const int &dim, unsigned int value) = 0;
};

class ImageKD : public KDValue
{
protected:
    static const int dimension = 5;

public:
    uint8_t r, g, b;
    uint16_t w, h;
    ImageKD() : KDValue() {}
    ImageKD(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _w, uint16_t _h, double _e = 0.0) : KDValue(_e), r(_r), g(_g), b(_b), w(_w), h(_h) {}
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
    double affinity(const ImageKD& kd)
    {
        double dist = 0;
        dist += pow(r - kd.r, 2);
        dist += pow(g - kd.g, 2);
        dist += pow(b - kd.b, 2);
        dist += pow(w - kd.w, 2);
        dist += pow(h - kd.h, 2);
        dist = sqrt(dist);
        return std::exp(-dist);
    }
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
    void set_value(const int &dim, unsigned int value)
    {
        if (dim == 0)
            r = (uint8_t)value;
        else if (dim == 1)
            g = (uint8_t)value;
        else if (dim == 2)
            b = (uint8_t)value;
        else if (dim == 3)
            w = value;
        else if (dim == 4)
            h = value;
    }
    bool operator<(const ImageKD &c) const
    {
        // FIXME: Cannot directly call get_value with this, weird.
        if (unsigned(r) < c.get_value(KDIndex::r))
            return true;
        else if (unsigned(r) > c.get_value(KDIndex::r))
            return false;
        else if (unsigned(g) < c.get_value(KDIndex::g))
            return true;
        else if (unsigned(g) > c.get_value(KDIndex::g))
            return false;
        else if (unsigned(b) < c.get_value(KDIndex::b))
            return true;
        else if (unsigned(b) > c.get_value(KDIndex::b))
            return false;
        else if(w < c.get_value(KDIndex::w))
            return true;
        else if(w > c.get_value(KDIndex::w))
            return false;
        else if(h < c.get_value(KDIndex::h))
            return true;
        else if(h > c.get_value(KDIndex::h))
            return false;
        return false;
    }
    bool operator==(const ImageKD &c) const
    {
        return unsigned(r) == c.get_value(KDIndex::r) &&
               unsigned(g) == c.get_value(KDIndex::g) &&
               unsigned(b) == c.get_value(KDIndex::b) &&
               w == c.get_value(KDIndex::w) &&
               h == c.get_value(KDIndex::h) &&
               e == c.e;
    }
};

#endif // EDIT_KDVALUE_H