/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-07 10:54:38
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-08 16:39:38
 * @Description: The Optimizer class to solve 
 */
#ifndef EDIT_Optimizer_HPP
#define EDIT_Optimizer_HPP

#include <vector>
#include <random>
#include <ctime>
#include "corner.hpp"
#include "utils.hpp"

template <class T>
class Optimizer
{
protected:
    Corners<T> *corners;
    std::vector<int> samples;
    std::vector<T> points;
    int c_ratio;
    void insert_vec(SpMat &sp, std::vector<T> &vec);
    SpMat create_sp_mat(const int &rows, const int &cols);
    SpMat sp_inv(const SpMat &sp, const int &n);

public:
    Optimizer(Corners<T> *cor, int _c_ratio = 1000) : corners(cor), c_ratio(_c_ratio) {}
    DsMat optimize();
    void apply_edition(const DsMat &res);
    void sampling();
};

template <class T>
void Optimizer<T>::insert_vec(SpMat &sp, std::vector<T> &vec)
{
    // TODO:
}

template <class T>
SpMat Optimizer<T>::create_sp_mat(const int &rows, const int &cols)
{
    SpMat sp;
    sp.resize(rows, cols);
    return sp;
}

template <class T>
SpMat Optimizer<T>::sp_inv(const SpMat &sp, const int &n)
{
    SpSolver solver;
    solver.compute(sp);
    if (solver.info() != Eigen::Success)
    {
        std::cout << "fail to compute the sparse matrix" << std::endl;
    }
    SpMat I(n, n);
    I.setIdentity();
    SpMat sp_inv = solver.solve(I);
    if (solver.info() != Eigen::Success)
    {
        std::cout << "fail to compute the sparse matrix" << std::endl;
    }
    return sp_inv;
}

template <class T>
DsMat Optimizer<T>::optimize()
{
    int n = corners->editions.size();
    SpMat g = create_sp_mat(n, 1);
    int cnt = 0;
    for (auto &item : corners->editions)
    {
        points.push_back(item.first);
        Float gi = item.second.g;
        if (gi > Utils::EPS)
        {
            g.insert(cnt, 0) = gi;
        }
        cnt++;
    }

    sampling();

    int m = samples.size();

    DsMat u = Eigen::MatrixXf::Zero(n, m);
    DsMat a = Eigen::MatrixXf::Zero(m, m);

    for (int i = 0; i < m; i++)
    {
        T &pi = points[i];
        for (int j = 0; j < n; j++)
        {
            Float z = pi.affinity(points[j]);
            u(j, i) = z;
            if (j < m)
            {
                a(j, i) = z;
            }
        }
    }

    DsMat u_trans = u.transpose();
    DsMat a_inv = a.inverse();
    DsMat in = Eigen::MatrixXf::Ones(n, 1);
    DsMat d = ((0.5 * Utils::w / Utils::lambda) + 1) * u * (a_inv * (u_trans * in));
    SpMat D = create_sp_mat(n, n);
    SpMat D_inv = create_sp_mat(n, n);
    for (int i = 0; i < n; i++)
    {
        D.insert(i, i) = d(i, 0);
        D_inv.insert(i, i) = (1.0) / d(i, 0);
    }
    SpMat su = u.sparseView();
    DsMat uau_t_g = Utils::w * (su * (a_inv * (u_trans * g)));
    DsMat mid_term = ((-1) * a + u_trans * (D_inv * su)).inverse();
    DsMat first_term = D_inv * uau_t_g;
    DsMat last_term = D_inv * (su * (mid_term * (u_trans * (D_inv * uau_t_g))));
    DsMat e_res = (0.5 / Utils::lambda) * (first_term - last_term);
    return e_res;
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

template <class T>
void Optimizer<T>::apply_edition(const DsMat &res)
{
    int n = corners->editions.size();
    for (int i = 0; i < n; i ++)
    {
        T &point = points[i];
        corners->editions[point].e = res(i, 0);
    }
}

#endif // EDIT_Optimizer_HPP