/*
 * Determine se a peça possui 6 ou 4 buracos. Se a peça contiver 6 buracos
 * classifique-a como tipo A, Se a peça contiver 4 buracos classifique-a como
 * tipo B, caso contrário rejeite a peça classificando a como tipo R.
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int numOfHoles(Mat &img) {


  return 0;
}

int main(int argc, char** argv) {
  Mat image;
  image = imread("4.png", IMREAD_COLOR);

  int holes = numOfHoles(image);

  switch (holes) {
  case 4:
    std::cout << "Tipo: B" << std::endl;
    break;

  case 5:
    std::cout << "Tipo: A" << std::endl;
    break;

  default:
    std::cout << "Tipo: R" << std::endl;
    break;
  }

  waitKey(0);
  return 0;
}
