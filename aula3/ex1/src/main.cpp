/*
 * Implemente os filtros no domínio do espaço: Média, Sobel, Mediana.
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
  Mat image;
  image = imread("lenna.jpg", IMREAD_COLOR);

  namedWindow("Imagem Original", WINDOW_AUTOSIZE);
  imshow("Imagem Original", image);



  waitKey(0);
  return 0;
}
