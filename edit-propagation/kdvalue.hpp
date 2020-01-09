/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 05:51:53
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-09 02:53:09
 * @Description: Define k-d value 
 */

#ifndef EDIT_KDVALUE_H
#define EDIT_KDVALUE_H

#include <cstdint>
#include <ostream>
#include <cmath>
#include "utils.hpp"

enum KDIndex
{
    r = 0,
    g = 1,
    b = 2,
    w = 3,
    h = 4,
    t = 5
};

class KDValue
{
public:
    Float e;
    Float weight;
    KDValue(Float _e = 0.0, Float _weight = 0.0) : e(_e), weight(_weight) {}
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
    ImageKD(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _w, uint16_t _h, Float _e = 0.0, Float _weight = 0.0) : KDValue(_e, _weight), r(_r), g(_g), b(_b), w(_w), h(_h) {}
    ImageKD create_new(const int &pivot, const int &new_value) const
    {
        ImageKD new_kd = *this;
        new_kd.set_value(pivot, new_value);
        return new_kd;
    }
    ImageKD operator+(const ImageKD &kd)
    {
        // TODO: Consider weight?
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
    Float affinity(const ImageKD &kd)
    {
        Float &&_r = (Float)(r - kd.r) / (Utils::theta_c * 255.0);
        Float &&_g = (Float)(g - kd.g) / (Utils::theta_c * 255.0);
        Float &&_b = (Float)(b - kd.b) / (Utils::theta_c * 255.0);
        Float &&_w = (Float)(w - kd.w) / (Float)(Utils::width * Utils::theta_p);
        Float &&_h = (Float)(h - kd.h) / (Float)(Utils::height * Utils::theta_p);
        Float color_dist = sqrt(_r * _r + _g * _g + _b * _b);
        Float pos_dist = sqrt(_w * _w + _h * _h);
        Float res = std::exp(-color_dist) * std::exp(-pos_dist);
        return res;
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
        else if (w < c.get_value(KDIndex::w))
            return true;
        else if (w > c.get_value(KDIndex::w))
            return false;
        else if (h < c.get_value(KDIndex::h))
            return true;
        else if (h > c.get_value(KDIndex::h))
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

class VideoKD : public KDValue
{
protected:
    static const int dimension = 6;

public:
    uint8_t r, g, b;
    uint16_t w, h;
    int t;
    VideoKD() : KDValue() {}
    VideoKD(uint8_t _r, uint8_t _g, uint8_t _b, uint16_t _w, uint16_t _h, int _t, Float _e = 0.0, Float _weight = 0.0) : KDValue(_e, _weight), r(_r), g(_g), b(_b), w(_w), h(_h), t(_t) {}
    VideoKD create_new(const int &pivot, const int &new_value) const
    {
        VideoKD new_kd = *this;
        new_kd.set_value(pivot, new_value);
        return new_kd;
    }
    VideoKD operator+(const VideoKD &kd)
    {
        // TODO: Consider weight?
        return VideoKD(unsigned(r) + unsigned(kd.r), unsigned(g) + unsigned(kd.g), unsigned(b + kd.b), w + kd.w, h + kd.h, t + kd.t);
    }
    VideoKD operator/(const int &n)
    {
        return VideoKD(unsigned(r) / n, unsigned(g) / n, unsigned(b) / n, w / n, h / n, t / n);
    }
    friend std::ostream &operator<<(std::ostream &output, const VideoKD &kd)
    {
        output << "r: " << unsigned(kd.r) << ", g: " << unsigned(kd.g) << ", b: " << unsigned(kd.b) << ", w: " << kd.w << ", h: " << kd.h;
        return output;
    }
    ~VideoKD() {}
    static int dim() { return dimension; }
    Float affinity(const VideoKD &kd)
    {
        Float &&_r = (Float)(r - kd.r) / (Utils::theta_c * 255.0);
        Float &&_g = (Float)(g - kd.g) / (Utils::theta_c * 255.0);
        Float &&_b = (Float)(b - kd.b) / (Utils::theta_c * 255.0);
        Float &&_w = (Float)(w - kd.w) / (Float)(Utils::width * Utils::theta_p);
        Float &&_h = (Float)(h - kd.h) / (Float)(Utils::height * Utils::theta_p);
        Float &&_t = (Float)(t - kd.t) / (Float)(Utils::theta_time * Utils::video_length);
        Float color_dist = sqrt(_r * _r + _g * _g + _b * _b);
        Float pos_dist = sqrt(_w * _w + _h * _h);
        Float t_dist = std::fabs(_t);
        Float res = std::exp(-color_dist) * std::exp(-pos_dist) * std::exp(-t_dist);
        return res;
    }
    int get_value(const int &dim) const
    {
        if (dim == KDIndex::r)
            return unsigned(r);
        else if (dim == KDIndex::g)
            return unsigned(g);
        else if (dim == KDIndex::b)
            return unsigned(b);
        else if (dim == KDIndex::w)
            return w;
        else if (dim == KDIndex::h)
            return h;
        else if (dim == KDIndex::t)
            return t;
        else
            return 0;
    }
    void set_value(const int &dim, unsigned int value)
    {
        if (dim == KDIndex::r)
            r = (uint8_t)value;
        else if (dim == KDIndex::g)
            g = (uint8_t)value;
        else if (dim == KDIndex::b)
            b = (uint8_t)value;
        else if (dim == KDIndex::w)
            w = value;
        else if (dim == KDIndex::h)
            h = value;
        else if (dim == KDIndex::t)
            t = value;
    }
    bool operator<(const VideoKD &c) const
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
        else if (w < c.get_value(KDIndex::w))
            return true;
        else if (w > c.get_value(KDIndex::w))
            return false;
        else if (h < c.get_value(KDIndex::h))
            return true;
        else if (h > c.get_value(KDIndex::h))
            return false;
        else if (t < c.get_value(KDIndex::t))
            return true;
        else if (t > c.get_value(KDIndex::t))
            return false;
        return false;
    }
    bool operator==(const VideoKD &c) const
    {
        return unsigned(r) == c.get_value(KDIndex::r) &&
               unsigned(g) == c.get_value(KDIndex::g) &&
               unsigned(b) == c.get_value(KDIndex::b) &&
               w == c.get_value(KDIndex::w) &&
               h == c.get_value(KDIndex::h) &&
               e == c.e &&
               t == c.t;
    }
};
#endif // EDIT_KDVALUE_H