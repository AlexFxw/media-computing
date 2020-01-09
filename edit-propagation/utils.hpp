/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 07:25:57
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-09 04:26:31
 * @Description: file content
 */

#ifndef EDIT_UTILS_HPP 
#define EDIT_UTILS_HPP

#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/IterativeLinearSolvers>


typedef float Float;
typedef Eigen::SparseMatrix<Float> SpMat;
typedef Eigen::ConjugateGradient<SpMat> SpSolver;
typedef Eigen::MatrixXf DsMat;

class Utils
{
public:
    template <class T>
        static Float multi_interpolate(const T &point, const T &target, const T &opposite);
    template <class T>
        static T get_opposite_corner(const T &corner, const T &lower, const T &upper);
    static Float w; // 和用户的编辑相关的权重
    static Float lambda;
    static Float EPS;
    static Float theta_c;
    static Float theta_p;
    static Float theta_time;
    static int width, height;
    static int video_length;
    static int edit_frame;
};

Float Utils::w = 0.2;
Float Utils::lambda = 0.7;
Float Utils::EPS = 1e-3F;
Float Utils::theta_c = 1.0;
Float Utils::theta_p = 1.0;
Float Utils::theta_time = 1.0;
int Utils::width = 0;
int Utils::height = 0;
int Utils::video_length = 1;
int Utils::edit_frame = 1;

template <class T>
Float Utils::multi_interpolate(const T &point, const T &target, const T &opposite)
{
    int dim = T::dim();
    Float coord = 1.0;
    for (int i = 0; i < dim; i++)
    {
        coord *= (Float)std::abs(point.get_value(i) - target.get_value(i));
        coord /= (Float)std::abs(opposite.get_value(i) - target.get_value(i));
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