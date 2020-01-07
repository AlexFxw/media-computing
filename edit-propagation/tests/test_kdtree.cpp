/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-06 05:03:26
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 09:04:08
 * @Description: Testing the kdtree
 */

#include <gtest/gtest.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#define private public
#include "edition.hpp"
#include "kdtree.hpp"
#include "media_handler.hpp"
#include "kdvalue.hpp"

class TestKDTree : public testing::Test
{
protected:
    virtual void SetUp() override
    {
        kdtree = KDTree<ImageKD>::CreateFromImage(img, edition);
        ImageKD lower(0, 0, 0, 0, 0);
        ImageKD upper(255, 255, 255, img.width, img.height);
        // kdtree->build(lower, upper, edition);
    }
    virtual void TearDown() override
    {
        delete kdtree;
    }
    Image img = Image("../img/edit-propagation/src_1.jpg");
    Edition edition = Edition("../img/edit-propagation/edit_1.jpg");
    KDTree<ImageKD> *kdtree;
};

TEST_F(TestKDTree, Legal)
{
    EXPECT_EQ(img.width * img.height, (int)kdtree->data.size());
    std::cout << kdtree->data.size() << std::endl;
    bool flag[img.width][img.height];
    memset(flag, false, sizeof(flag));
    for (ImageKD &item : kdtree->data)
    {
        int w = item.get_value(KDIndex::w);
        int h = item.get_value(KDIndex::h);
        EXPECT_FALSE(flag[w][h]);
        flag[w][h] = true;
    }
}

TEST(TestKDNode, T_junction)
{
    ImageKD lower(0, 0, 0, 0, 0), upper(255, 255, 255, 300, 400);
    KDNode<ImageKD> *root, *left, *right;
    root = new KDNode<ImageKD>(lower, upper, 0);
    left = new KDNode<ImageKD>(lower, ImageKD(127, 255, 255, 300, 400), 1);
    right = new KDNode<ImageKD>(ImageKD(128, 255, 255, 300, 400), upper, 1);
    left->parent = root;
    right->parent = root;
    left->left = new KDNode<ImageKD>(lower, upper, 1);
    left->right = new KDNode<ImageKD>(lower, upper, 1);
    root->left = left;
    root->right = right;
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

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}