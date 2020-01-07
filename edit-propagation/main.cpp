/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2019-12-20 19:22:19
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-07 12:18:57
 * @Description: main function for edit propagation.
 */

#include <string>
#include <iostream>
#include "edition.hpp"
#include "media_handler.hpp"
#include "kdtree.hpp"
#include "corner.hpp"

int main(int argc, const char **argv)
{
    Edition edition("./img/edit-propagation/edit_1.jpg");
    Image img("./img/edit-propagation/src_1.jpg");
    KDTree<ImageKD> *kdtree = KDTree<ImageKD>::CreateFromImage(img, edition);
    ImageKD lower(0, 0, 0, 0, 0);
    ImageKD upper(255, 255, 255, img.width, img.height);
    kdtree->build(lower, upper, edition);
    std::cout << "built the kd tree." << std::endl;
    Corners<ImageKD> *corners = new Corners<ImageKD>();
    kdtree->calc_corners(kdtree->root, corners);
    std::cout << "Finish!" << std::endl;
    return 0;
}