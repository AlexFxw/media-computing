/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2019-12-20 19:22:19
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-04 12:16:53
 * @Description: main function for edit propagation.
 */

#include <string>
#include "edition.hpp"
#include "media_handler.hpp"
#include "kdtree.hpp"

int main(int argc, const char **argv)
{
    Edition edition("../img/edit-propagation/edit_1.jpg");
    Image img("../img/edit-propagation/src_1.jpg");
    KDTree<ImageKD> *kdtree = KDTree<ImageKD>::CreateFromImage(img, edition);
    return 0;
}