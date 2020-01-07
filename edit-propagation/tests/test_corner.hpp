/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-07 14:03:50
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 14:07:51
 * @Description: Test corner file
 */
#ifndef EDIT_TEST_CORNER
#define EDIT_TEST_CORNER

#include <gtest/gtest.h>
#include "corner.hpp"
#include "kdvalue.hpp"

class TestCorner: public testing::Test
{

};

TEST_F(TestCorner, TestMap)
{
    ImageKD tmp(0, 0, 255, 30, 40, 1.0);
    CornerInfo c(1, 0.3, 0);
    Corners<ImageKD> *corners = new Corners<ImageKD>();
    EXPECT_EQ(corners->editions.size(), 0);
    corners->set_corner(tmp);
    EXPECT_EQ(corners->editions.size(), 1);
    corners->set_corner(tmp);
    EXPECT_EQ(corners->editions.size(), 1);
    corners->add_corner(tmp, c);
    EXPECT_EQ(corners->editions.size(), 1);
}
#endif // EDIT_TEST_CORNER