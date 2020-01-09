/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 06:28:28
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-09 05:17:34
 * @Description: Implement of KD Tree 
 */

#ifndef KDTREE_KDTREE_H
#define KDTREE_KDTREE_H

#include <vector>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include "media_handler.hpp"
#include "kdvalue.hpp"
#include "edition.hpp"
#include "corner.hpp"
#include "utils.hpp"

template <class T>
struct KDNode
{
    static int threshold;
    KDNode *left, *right, *parent;
    std::vector<T> data;
    T lower_bound, upper_bound;
    int pivot;
    KDNode(T l, T u, int p) : left(NULL), right(NULL), parent(NULL), lower_bound(l), upper_bound(u), pivot(p) {}
    ~KDNode()
    {
        if (left != NULL)
            delete left;
        if (right != NULL)
            delete right;
        if (parent != NULL)
            delete parent;
    }

    KDNode<T> *sibling() const
    {
        if (parent == NULL)
            return NULL;
        if (parent->left == this)
            return parent->right;
        return parent->left;
    }

    bool has_child() const
    {
        return this->left != NULL && this->right != NULL;
    }

    bool contain_T_junction() const
    {
        if (parent == NULL || !this->has_child())
            return false;
        KDNode<T> *sibling = this->sibling();
        if (sibling->has_child())
            return false;
        return true;
    }

    std::vector<T> get_corners() const
    {
        int dim = T::dim(), num = 1 << dim;
        std::vector<T> corners;
        for (int n = 0; n < num; n++)
        {
            T tmp = lower_bound;
            int cnt = 0;
            for (int i = 0; i < dim; i++)
            {
                int mask = 1 << cnt;
                if (((n & mask) >> cnt) % 2 == 0)
                {
                    // Set this item to lower;
                    tmp.set_value(i, upper_bound.get_value(i));
                }
                cnt++;
            }
            corners.push_back(tmp);
        }
        return corners;
    }

    std::vector<T> get_T_junction(const int &dim)
    {
        // TODO: Test this function
        int fixed_pivot = this->parent->pivot;
        int div = upper_bound.get_value(fixed_pivot);
        assert(this->pivot != fixed_pivot);
        T new_kdvalue = upper_bound;
        int mid_value = (lower_bound.get_value(pivot) + upper_bound.get_value(pivot)) / 2;
        new_kdvalue.set_value(pivot, mid_value);
        std::vector<T> vec;
        int num = (1 << (dim - 2));
        if (num > 0)
        {
            for (int n = 0; n < num; n++)
            {
                int cnt = 0;
                T tmp = new_kdvalue;
                for (int i = 0; i < dim; i++)
                {
                    if (i == fixed_pivot || i == this->pivot)
                        continue;
                    uint8_t mask = 1 << cnt;
                    if (((n & mask) >> cnt) % 2 == 0)
                    {
                        // Set this item to lower;
                        tmp.set_value(i, lower_bound.get_value(i));
                    }
                    cnt++;
                }
                vec.push_back(tmp);
            }
        }
        return vec;
    }

    std::vector<T> get_data() const
    {
        if(!this->has_child())
        {
            return this->data;
        }
        std::vector<T> res = this->left->get_data(), right_res = this->right->get_data();
        for(auto &r : right_res)
        {
            res.push_back(r);
        }
        return res;
    }
};

template <class T>
class KDTree
{
private:
    int dimension;
    // std::vector<T> data;
    int cur_pivot;
    int threshold = 900;
    Float dist_threshold = 400;
    bool term_condition(int num, KDNode<T> *node, Edition &edit);
    void clear_vector(std::vector<T> &vt)
    {
        std::vector<T> vtTemp;
        vtTemp.swap(vt);
    }

public:
    KDNode<T> *root;
    KDTree();
    ~KDTree()
    {
        if (root != NULL)
            delete root;
    }
    KDNode<T> *create_node(Edition &edit, int pivot, T lower, T upper, std::vector<T> &kdvalues);
    int build(T &lower, T &upper, Edition &edit, std::vector<T> &data);
    // TODO: Calc corner value
    void calc_corners(KDNode<T> *node, Corners<T> *corners);
    void adjust_T_junctions(KDNode<T> *node, Corners<T> *corners);
    std::vector<T> get_pixels() const;
    void apply_edition(KDNode<T> *node, Corners<T> *corners);
};

template <class T>
KDTree<T>::KDTree() : root(NULL), dimension(T::dim()), cur_pivot(0)
{
}

template <class T>
KDNode<T> *KDTree<T>::create_node(Edition &edit, int pivot, T lower, T upper, std::vector<T> &kdvalues)
{
    KDNode<T> *node = new KDNode<T>(lower, upper, pivot);
    this->cur_pivot = pivot;
    int kd_num = kdvalues.size();
    if (this->term_condition(kd_num, node, edit))
    {
        /**
         * Terminate condition:
         * 1. Distance from edition is too far.
         * 2. The number within a cluster(node) is less enough.
         */
        node->data = kdvalues;
        // printf("Terminated. %d\n", (int)node->data.size());
        return node;
    }
    int lower_div = lower.get_value(pivot);
    int upper_div = upper.get_value(pivot);
    int mid = (upper_div + lower_div) / 2;
    T lm = upper, um = lower; // 除了对应的维修改外，左子树的新的上界的其他维应该是原先的上界
    lm.set_value(pivot, mid);
    um.set_value(pivot, mid + 1);
    // TODO: 会不会超 upper
    std::vector<T> left_kdvalues, right_kdvalues;
    for (auto &item : kdvalues)
    {
        if (item.get_value(cur_pivot) < mid)
        {
            left_kdvalues.push_back(item);
        }
        else
        {
            right_kdvalues.push_back(item);
        }
    }
    this->clear_vector(kdvalues);
    int next_pivot = (pivot + 1) % this->dimension;
    node->left = create_node(edit, next_pivot, lower, lm, left_kdvalues);
    node->right = create_node(edit, next_pivot, um, upper, right_kdvalues);
    node->left->parent = node;
    node->right->parent = node;
    return node;
}

template <class T>
int KDTree<T>::build(T &lower, T &upper, Edition &edit, std::vector<T> &data)
{
    root = this->create_node(edit, 0, lower, upper, data);
}

template <class T>
bool KDTree<T>::term_condition(int num, KDNode<T> *node, Edition &edit)
{
    if (num < this->threshold)
    {
        return true;
    }
    T mid_point = (node->lower_bound + node->upper_bound) / 2;
    // std::cout << "lower: " << node->lower_bound << std::endl;
    // std::cout << "upper: " << node->upper_bound << std::endl;
    if (node->upper_bound.get_value(cur_pivot) - node->lower_bound.get_value(cur_pivot) <= 1)
    {
        return true;
    }
    Float distance = edit.distance(mid_point);
    if (distance > this->dist_threshold)
    {
        return true;
    }
    return false;
}

template <class T>
void KDTree<T>::calc_corners(KDNode<T> *node, Corners<T> *corners)
{
    if (node->has_child())
    {
        calc_corners(node->left, corners);
        calc_corners(node->right, corners);
    }
    else
    {
        // this is leaf node.
        std::vector<T> corners_nodes = node->get_corners();
        int data_num = node->data.size();
        for (auto &c : corners_nodes)
        {
            // Initialize the corner value.
            // FIXME: need to test
            // corners->set_corner(c);
            corners->init_corner(c);
        }
        for (auto &c : corners_nodes)
        {
            Float s = 0, g = 0, w = 0;
            for (auto &d : node->data)
            {
                T opposite = Utils::get_opposite_corner<T>(c, node->lower_bound, node->upper_bound);
                // std::cout << c << " " << opposite << " " << d << std::endl;
                Float si = Utils::multi_interpolate<T>(d, c, opposite);
                s += si;
                g += d.e * si;
                w += d.weight * si;
            }
            // Because e is unknown before we calculate it.
            g = (s == 0) ? 0 : g / s;
            w = (s == 0) ? 0 : w / s;
            corners->add_corner(c, CornerInfo(0.0, g, s, w));
        }
    }
}

template <class T>
void KDTree<T>::adjust_T_junctions(KDNode<T> *node, Corners<T> *corners)
{
    if (node->contain_T_junction())
    {
        std::vector<T> t_vec = node->get_T_junction(this->dimension);
        int pivot = node->pivot;
        for (auto &t : t_vec)
        {
            T low = t.create_new(pivot, node->lower_bound.get_value(pivot));
            T upper = t.create_new(pivot, node->upper_bound.get_value(pivot));
            CornerInfo low_value = corners->get_value(low);
            CornerInfo upper_value = corners->get_value(upper);
            corners->set_corner(t, (low_value + upper_value) / 2);
        }
        this->clear_vector(t_vec);
    }
    if (node->has_child())
    {
        adjust_T_junctions(node->left, corners);
        adjust_T_junctions(node->right, corners);
    }
}

template <class T>
std::vector<T> KDTree<T>::get_pixels() const
{
    return this->root->get_data();
}

template <class T>
void KDTree<T>::apply_edition(KDNode<T> *node, Corners<T> *corners)
{
    if(node->has_child())
    {
        apply_edition(node->left, corners);
        apply_edition(node->right, corners);
    }
    else
    {
        std::vector<T> c_vec = node->get_corners();
        std::map<T, bool> flag;
        for (auto &d : node->data)
        {
            for(auto &c: c_vec)
            {
                T opposite = Utils::get_opposite_corner<T>(c, node->lower_bound, node->upper_bound);
                // std::cout << c << " " << opposite << " " << d << std::endl;
                Float si = Utils::multi_interpolate<T>(d, c, opposite);
                Float e = corners->get_value(c).e;
                d.e += si * e;
            }
        }
    }
}

#endif //KDTREE_KDTREE_H
