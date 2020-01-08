/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-06 05:03:26
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 09:43:44
 * @Description: Testing the kdtree
 */

#ifndef EDIT_TEST_KDTREE
#define EDIT_TEST_KDTREE

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


#endif // EDIT_TEST_KDTREE