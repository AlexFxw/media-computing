/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-04 06:28:28
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-04 08:06:52
 * @Description: Implement of KD Tree 
 */

#ifndef KDTREE_KDTREE_H
#define KDTREE_KDTREE_H

#include <vector>
#include <algorithm>
#include "media_handler.hpp"
#include "kdvalue.hpp"

template <class T>
struct KDNode
{
    static int threshold;
    KDNode *left, *right, *parent;
    std::vector<T> data;
    T lower_bound, upper_bound;
    int pivot;

    KDNode() : left(NULL), right(NULL), parent(NULL) {}
    KDNode(T l, T u, int p): KDNode(), lower_bound(l), upper_bound(u), pivot(p) {}
    ~KDNode()
    {
        if (left != NULL)
            delete left;
        if (right != NULL)
            delete right;
        if (parent != NULL)
            delete parent;
        if (data_index != NULL)
            delete data_index;
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
    int threshold;
    bool kdvalue_less_cmp(const T &t1, const T &t2);
    bool term_condition(int num, const KDNode<T> *node, const Image &edit);

public:
    typedef typename std::vector<T>::iterator kd_iter;
    KDTree() : root(NULL), dimension(T::dim()), cur_pivot(0), threshold(256) {}
    KDTree(std::vector<T> &_data);
    ~KDTree()
    {
        if (root != NULL)
            delete root;
        data.clear();
    }
    static KDTree *CreateFromImage(const Image &img, const Image &edit);
    KDNode<T> *KDTree<T>::create_node(const Image &edit, int pivot, T lower, T upper, kd_iter start, kd_iter end); 
    ~KDTree();

    int build(const T &lower, const T &upper, const Image &edit);
};

template <class T>
KDTree<T>::KDTree(std::vector<T> &_data) : KDTree(), data(_data), root(NULL)
{
}

template <class T>
KDTree<T> *KDTree<T>::CreateFromImage(const Image &img, const Image &edit)
{
    // Only image KD here
    std::vector<T> img_kdvalue;
    for (int w = 0; w < img.width; w++)
    {
        for (int h = 0; h < img.height; h++)
        {
            uint8_t r = img.get_pixel(w, h, CHANNEL::R);
            uint8_t g = img.get_pixel(w, h, CHANNEL::G);
            uint8_t b = img.get_pixel(w, h, CHANNEL::B);
            img_kdvalue.push_back(T(r, g, b, w, h));
        }
    }
    return new KDTree(img_kdvalue);
}

template <class T>
KDNode<T>* KDTree<T>::create_node(const Image &edit, int pivot, T lower, T upper, kd_iter start, kd_iter end)
{
    KDNode<T> *node = new KDNode<T>(lower, upper);
    std::sort(start, end, this->kdvalue_less_cmp);
    int kd_num = std::distance(start, end);
    if(this->term_condition(kd_num, node, edit))
    {
        /**
         * Terminate condition:
         * 1. Distance from edition is too far.
         * 2. The number within a cluster(node) is less enough.
         */
        for (kd_iter iter = start; iter != end; iter ++)
        {
            node->data.push_back(*iter);
        }
        return node;
    }
    this->cur_pivot = pivot;
    int lower_div = lower.get_value(pivot);
    int upper_div = upper.get_value(pivot);
    int mid = (upper_div + lower_div) / 2;
    T mid_point = lower_div;
    mid_point.set_value(pivot, mid);
    kd_iter new_iter;
    for (kd_iter iter = start, iter != end; iter++)
    {
        if (iter->get_value(cur_pivot) >= mid) {
            new_iter = iter;
            break;
        }
    }
    node->left = create_node(edit, pivot + 1, lower, mid_point, start, new_iter);
    node->right = create_node(edit, pivot + 1, mid_point, end, new_iter, end);
    node->left->parent = node;
    node->right->parent = node;
    return node;
}

template <class T>
int KDTree<T>::build(const T &lower, const T &upper, const Image &edit)
{
    root = new KDNode<T>();
    KDNode<T> *node = root;
}

template <class T>
bool KDTree<T>::kdvalue_less_cmp(const T &t1, const T &t2)
{
    int &&p = this->cur_pivot;
    return t1.get_value(p) < t2.get_value(p);
}

template <class T>
bool KDTree<T>::term_condition(int num, const KDNode<T> *node, const Image &edit)
{
    if(num < this->threshold)
    {
        return true;
    }
    T mid_point = (node->lower_bound + node->upper_bound) / 2;
    // TODO: the distance from edition.
    return false;
}

#endif //KDTREE_KDTREE_H
