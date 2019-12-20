/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2019-12-19 11:29:55
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2019-12-20 08:48:25
 * @Description: main function 
 */

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <cstdio>
#include <string>
#include "transferer.hpp"

void transfer_to_gray(const std::string& file_name)
{
    cv::Mat g = cv::imread(file_name);
    cv::cvtColor(g, g, cv::COLOR_BGR2GRAY);
    cv::imwrite(file_name, g);
}

int main()
{
    std::string img_dir = "../img/colorize/";
    for(int i = 1; i <= 2; i ++)
    {
        printf("Dealing with no.%d image...\n", i);
        std::string target_name = img_dir + "gray_" + std::to_string(i) + ".jpg";
        std::string src_name = img_dir + "color_" + std::to_string(i) + ".jpg";
        std::string res_name = img_dir + "res_" + std::to_string(i) + ".jpg";
        Transferer solver;
        if(solver.colorize(src_name, target_name, res_name)) {
            printf("Fail to colorize\n");
            return 1;
        }
    }
   return 0;
}