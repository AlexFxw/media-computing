/*
 * @Author: Fan Hsuan-Wei
 * @Date: 2019-12-19 11:29:55
 * @LastEditors  : Fan Hsuan-Wei
 * @LastEditTime : 2019-12-20 08:14:47
 * @Description: main function 
 */

#include <cstdio>
#include <string>
#include "transferer.hpp"


int main()
{
    std::string img_dir = "../img/colorize/";
    std::string target_name = img_dir+"gray_1.jpg";
    std::string src_name = img_dir+"color_1.jpg";
    std::string res_name = img_dir+"res_1.jpg";
    Transferer solver;
    if(solver.colorize(src_name, target_name, res_name)) {
        printf("Fail to colorize\n");
        return 1;
    }
    return 0;
}