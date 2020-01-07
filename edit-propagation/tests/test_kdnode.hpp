/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-07 09:42:33
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 13:19:41
 * @Description: file content
 */

#ifndef EDIT_TEST_KDNODE
#define EDIT_TEST_KDNODE

#include <gtest/gtest.h>
#include <vector>
#include "kdtree.hpp"
#include "media_handler.hpp"

class TestKDNode : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        ImageKD lower(0, 0, 0, 0, 0), upper(255, 255, 255, 300, 400);
        root = new KDNode<ImageKD>(lower, upper, 0);
        left = new KDNode<ImageKD>(lower, ImageKD(127, 255, 255, 300, 400), 1);
        right = new KDNode<ImageKD>(ImageKD(128, 255, 255, 300, 400), upper, 1);
        left->parent = root;
        right->parent = root;
        left->left = new KDNode<ImageKD>(lower, upper, 1);
        left->right = new KDNode<ImageKD>(lower, upper, 1);
        root->left = left;
        root->right = right;
    }
    virtual void TearDown() override
    {
    }
    KDNode<ImageKD> *root, *left, *right;
};


TEST_F(TestKDNode, T_junction)
{
    EXPECT_EQ(right, root->left->sibling());
    EXPECT_TRUE(left->contain_T_junction());
    std::vector<ImageKD> kd_vec = left->get_T_junction(5);
    for(auto &kd:kd_vec)
    {
        // std::cout << kd << std::endl;
        EXPECT_EQ(127, kd.get_value(0));
        EXPECT_EQ(127, kd.get_value(1));
    }
    EXPECT_EQ(kd_vec.size(), 1 << 3);
}

TEST_F(TestKDNode, get_corners)
{
    std::vector<ImageKD> kd_vec = right->get_corners();
    EXPECT_EQ(kd_vec.size(), 1 << 5);
    kd_vec = left->get_corners();
    EXPECT_EQ(kd_vec.size(), 1 << 5);
    kd_vec = root->get_corners();
    EXPECT_EQ(kd_vec.size(), 1 << 5);
}

TEST_F(TestKDNode, set_image_kdvalue)
{
    ImageKD kd(0, 0, 0, 300, 400);
    EXPECT_EQ(kd.get_value(0), 0);
    kd.set_value(0, 24);
    EXPECT_EQ(kd.get_value(0), 24);
}

#endif // EDIT_TEST_KDNODE