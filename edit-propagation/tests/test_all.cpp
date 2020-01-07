/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2020-01-07 09:40:24
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 11:56:38
 * @Description: file content
 */

#include <gtest/gtest.h>
#include "test_kdtree.hpp"
#include "test_kdnode.hpp"
#include "test_utils.hpp"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}