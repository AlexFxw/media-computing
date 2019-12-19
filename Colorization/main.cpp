/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2019-12-19 11:29:55
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2019-12-19 11:32:38
 * @Description: main function 
 */

#include <cstdio>
#include <string>
#include "image_handler.hpp"


int main()
{
    std::string src_name = "../img/lab1_gray1.png";
    std::string res_name = "../img/lab1_res1.png";
    LabImage *lab_image = new LabImage();
    lab_image->load(src_name); 
    lab_image->save(res_name);
    printf("Hello, colorization!\n");
    delete lab_image;
    return 0;
}