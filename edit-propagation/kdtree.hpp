/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 06:28:28
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 08:57:58
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
    std::vector<T> get_T_junction(const int &dim) const
    {
        // TODO: Test thsi function
        int fixed_pivot = this->parent->pivot;
        int div = upper_bound.get_value(fixed_pivot);
        assert(this->pivot != fixed_pivot);
        T new_kdvalue = upper_bound;
        int mid_value = (lower_bound.get_value(pivot) + upper_bound.get_value(pivot)) / 2;
        new_kdvalue.set_value(pivot, mid_value);
        std::vector<T> vec;
        int num = (1 << (dim - 2));
        if(num > 0) 
        {
            for (int n = 0; n < num; n ++)
            {
                int cnt = 0;
                T tmp = new_kdvalue;
                for (int i = 0; i < dim; i++)
                {
                    if(i == fixed_pivot || i == this->pivot)
                        continue;
                    uint8_t mask = 1 << cnt;
                    if(((n & mask) >> cnt) % 2 == 0)
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
};

template <class T>
class KDTree
{
private:
    KDNode<T> *root;
    int dimension;
    std::vector<T> data;
    int cur_pivot;
    int threshold = 256;
    double dist_threshold = 600;
    bool term_condition(int num, KDNode<T> *node, Edition &edit);
    void clear_vector(std::vector<T> &vt)
    {
        std::vector<T> vtTemp;
        vtTemp.swap(vt);
    }

public:
    KDTree(std::vector<T> &_data);
    ~KDTree()
    {
        if (root != NULL)
            delete root;
        data.clear();
    }
    static KDTree *CreateFromImage(Image &img, Edition &edit);
    KDNode<T> *create_node(Edition &edit, int pivot, T lower, T upper, std::vector<T> kdvalues);
    int build(T &lower, T &upper, Edition &edit);
};

template <class T>
KDTree<T>::KDTree(std::vector<T> &_data) : root(NULL), dimension(T::dim()), cur_pivot(0), data(_data)
{
}

template <class T>
KDTree<T> *KDTree<T>::CreateFromImage(Image &img, Edition &edit)
{
    // Only image KD here
    std::vector<T> img_kdvalue;
    for (int w = 0; w < img.width; w++)
    {
        for (int h = 0; h < img.height; h++)
        {
            int r = img.get_pixel(w, h, CHANNEL::R);
            int g = img.get_pixel(w, h, CHANNEL::G);
            int b = img.get_pixel(w, h, CHANNEL::B);
            img_kdvalue.push_back(T(r, g, b, w, h));
        }
    }
    return new KDTree(img_kdvalue);
}

template <class T>
KDNode<T> *KDTree<T>::create_node(Edition &edit, int pivot, T lower, T upper, std::vector<T> kdvalues)
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
        printf("Terminated. %d\n", (int)node->data.size());
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
int KDTree<T>::build(T &lower, T &upper, Edition &edit)
{
    root = this->create_node(edit, 0, lower, upper, this->data);
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
    double distance = edit.distance(mid_point);
    if (distance > this->dist_threshold)
    {
        return true;
    }
    return false;
}

#endif //KDTREE_KDTREE_H
