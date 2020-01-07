/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-07 11:44:34
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 12:02:49
 * @Description: file content
 */

#ifndef EDIT_TEST_UTILS
#define EDIT_TEST_UTILS

#include <gtest/gtest.h>
#include <iostream>
#include "kdvalue.hpp"
#include "utils.hpp"

class TestUtils : public testing::Test
{
    virtual void SetUp() override
    {
    }
    virtual void TearDown() override
    {
    }
};

TEST_F(TestUtils, test_opposite)
{
    ImageKD lower(0, 0, 0, 0, 0), upper(255, 255, 255, 300, 400);
    ImageKD point(0, 0, 255, 0, 400);
    ImageKD res = Utils::get_opposite_corner<ImageKD>(point, lower, upper);
    std::cout << res << std::endl;
    EXPECT_EQ(1, 1);
}

#endif // EDIT_TEST_UTILS