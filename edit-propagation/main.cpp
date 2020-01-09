/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2019-12-20 19:22:19
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2020-01-09 04:33:44
 * @Description: main function for edit propagation.
 */

#include <string>
#include <iostream>
#include "utils.hpp"
#include "edition.hpp"
#include "media_handler.hpp"
#include "kdtree.hpp"
#include "corner.hpp"
#include "optimizer.hpp"


void apply_edition_img(Image *img, const KDTree<ImageKD> *kdtree, Edition &edition)
{
    std::vector<ImageKD> edit_res = kdtree->get_pixels();
    for(auto &kd: edit_res)
    {
        int r = kd.get_value(KDIndex::r);
        int g = kd.get_value(KDIndex::g);
        int b = kd.get_value(KDIndex::b);
        int w = kd.get_value(KDIndex::w);
        int h = kd.get_value(KDIndex::h);
        Float e = kd.e;
        int ng = 0, nr = 0, nb = 0;
        edition.transfer(e, r, g, b, nr, ng, nb);
        img->set_pixel(w, h, CHANNEL::R, nr);
        img->set_pixel(w, h, CHANNEL::G, ng);
        img->set_pixel(w, h, CHANNEL::B, nb);
    }
}

void image_demo()
{
    Image img("./img/edit-propagation/src_1.jpg");
    Edition edition("./img/edit-propagation/edit_1.jpg");
    KDTree<ImageKD> *kdtree = new KDTree<ImageKD>();
    std::vector<ImageKD> kdvalues = edition.get_imagekd(img);
    ImageKD lower(0, 0, 0, 0, 0);
    ImageKD upper(255, 255, 255, img.width, img.height);
    Utils::width = img.width;
    Utils::height = img.height;
    kdtree->build(lower, upper, edition, kdvalues);

    std::cout << "Built the kd tree." << std::endl;
    Corners<ImageKD> *corners = new Corners<ImageKD>();
    kdtree->calc_corners(kdtree->root, corners);
    std::cout << "Calculated the corner value!" << std::endl;
    std::cout << "  - corners num: " << corners->editions.size() << std::endl;
    Optimizer<ImageKD> *solver = new Optimizer<ImageKD>(corners);
    DsMat res = solver->optimize();
    solver->apply_edition(res);
    std::cout << "Solved editions in corner points." << std::endl;
    kdtree->adjust_T_junctions(kdtree->root, corners);
    std::cout << "Adjusted the T junctions." << std::endl;
    kdtree->apply_edition(kdtree->root, corners);
    std::cout << "Applied edition to kd-tree." << std::endl;
    Image *res_img = new Image(Utils::width, Utils::height);
    apply_edition_img(res_img, kdtree, edition);
    res_img->save("./img/edit-propagation/res_1.jpg");
    std::cout << "Finish!" << std::endl;
}

void save_video_frame()
{
    std::string video_name("./img/edit-propagation/candle.mp4");
    Video video;
    video.save_frame(video_name, "./img/edit-propagation/candle_frame1.jpg", 1);
}

void video_demo()
{
    int edit_frame = 1;
    Utils::edit_frame = edit_frame;
    Image src_frame("./img/edit-propagation/candle_frame1.jpg");
    Edition edition("./img/edit-propagation/candle_edit1.jpg");
    std::string video_name("./img/edit-propagation/candle.mp4");
    std::vector<VideoKD> kdvec = edition.get_videokd(video_name, edit_frame);
    // std::cout << sizeof(kdvec) << std::endl;
    // std::cout << kdvec.size() << std::endl;
    KDTree<VideoKD> *kdtree = new KDTree<VideoKD>();
    VideoKD lower(0, 0, 0, 0, 0, 0);
    VideoKD upper(255, 255, 255, Utils::width, Utils::height, Utils::video_length);
    kdtree->build(lower, upper, edition, kdvec);
}

int main(int argc, const char **argv)
{
    image_demo();
    // save_video_frame();
    // video_demo();
    return 0;
}