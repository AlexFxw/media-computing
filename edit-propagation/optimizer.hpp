/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-07 10:54:38
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 18:32:16
 * @Description: The Optimizer class to solve 
 */
#ifndef EDIT_Optimizer_HPP
#define EDIT_Optimizer_HPP

#include <eigen3/Eigen/Dense>
#include <vector>
#include <random>
#include <ctime>
#include "corner.hpp"

template <class T>
class Optimizer
{
protected:
    Corners<T> *corners;
    std::vector<int> samples;
    std::vector<T> points;
    int c_ratio;

public:
    Optimizer(Corners<T> *cor, int _c_ratio = 1000) : corners(cor), c_ratio(_c_ratio) {}
    void optimize();
    void sampling();
};

template <class T>
void Optimizer<T>::optimize()
{
    int c_num = corners->editions.size();
    Eigen::MatrixXf g = Eigen::MatrixXf::Zero(1, c_num);
    Eigen::MatrixXf e = Eigen::MatrixXf::Zero(1, c_num);
    int cnt = 0;
    for (auto &item : corners->editions)
    {
        points.push_back(item.first);
        g(0, cnt) = item.second.g;
        cnt++;
    }

    sampling();
    int n = c_num, m = samples.size();
    printf("n: %d, m: %d\n", n, m);
    Eigen::MatrixXf u = Eigen::MatrixXf::Zero(n, m);
    Eigen::MatrixXf a = Eigen::MatrixXf::Zero(m, m);

    for (int k = 0; k < m; k++)
    {
        T &t = points[samples[k]];
        for (int i = 0; i < n; i++)
        {
            u(i, k) = t.affinity(points[i]);
            if (i < m)
            {
                a(i, k) = u(i, k);
            }
        }
    }
    Eigen::MatrixXf u_trans = u.transpose();
    Eigen::MatrixXf a_inv = a.inverse();
    Eigen::MatrixXf id = Eigen::MatrixXf::Ones(n, 1);
    Eigen::MatrixXf d = ((0.5 / Utils::lambda) * Utils::w + 1) * (u * (a_inv * (u_trans * id)));
    Eigen::MatrixXf d_inv = id; 
    for(int i = 0; i < n; i ++) {
        d_inv(i, 0) = 1.0 / d(i, 0);
    }
    auto D = d.asDiagonal();
    auto D_inv_diag = d_inv.asDiagonal();
    // auto D_inv = D_inv_diag.toDenseMatrix();

    Eigen::MatrixXf mid = ((-1)*a + u_trans * (D_inv_diag * u)).inverse();
    auto res = (D_inv_diag * (u * (mid * (u_trans * D_inv_diag)))).asDiagonal();
    
    // FIXME: Cannot add.
    // e = (0.5 / Utils::lambda) *
    //     (D_inv_diag - res * (u * (a_inv * u_trans)) * Utils::w * g;

    for (int i = 0; i < c_num; i++)
    {
        float &ei = e(0, i);
        T &point = points[i];
        corners->set_e(point, ei);
    }
}

template <class T>
void Optimizer<T>::sampling()
{
    int c_num = points.size();
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> u_rand(0, c_num);
    int sample_num = c_num / c_ratio;
    int value = 0;
    for (int i = 0; i < sample_num; i++)
    {
        int value = u_rand(generator);
        samples.push_back(value);
    }
}
#endif // EDIT_Optimizer_HPP