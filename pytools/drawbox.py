'''
@Author: Fan Hsuan-Wei
@Date: 2020-01-09 22:07:10
@LastEditors  : Fan Hsuan-Wei
@LastEditTime : 2020-01-10 01:50:29
@Description: file content
'''
import cv2

if __name__ == "__main__":
    img_dir = "../img/colorize/"
    src_name = "color_5"
    target_name = "gray_5"
    img_type = ".jpg"
    src_img = cv2.imread(img_dir + src_name + img_type, cv2.IMREAD_COLOR)
    target_img = cv2.imread(img_dir + target_name + img_type, cv2.IMREAD_COLOR)
    swatches = list()
    swatches.append((((736, 351), (799, 413), (390, 428), (520, 520)), (0, 255, 0)))
    swatches.append((((793, 93), (919, 184), (342, 109), (381, 154)), (0,0, 255)))
    with open(f"{img_dir}swatch_5.txt", "w") as f:
        for s in swatches:
            boxes = s[0]
            color = s[1]
            f.write(f"{boxes[0][0]} {boxes[0][1]} {boxes[1][0]} {boxes[1][1]} {boxes[2][0]} {boxes[2][1]} {boxes[3][0]} {boxes[3][1]}\n")
            cv2.rectangle(src_img, boxes[0], boxes[1], color, 2)
            cv2.rectangle(target_img, boxes[2], boxes[3], color, 2)
    cv2.imshow(src_name, src_img);
    cv2.waitKey()
    cv2.imshow(target_name, target_img);
    cv2.waitKey()
    cv2.imwrite(f"{img_dir}{src_name}_box{img_type}", src_img)
    cv2.imwrite(f"{img_dir}{target_name}_box{img_type}", target_img)
