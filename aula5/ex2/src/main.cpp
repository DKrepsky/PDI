/*
 * 2) Dado uma imagem binária e os conceitos de processamento de
 * imagens morfológicas, implemente um detector de bordas.
 *
 * Referências:
 *  - https://docs.opencv.org/trunk/d9/d61/tutorial_py_morphological_ops.html
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  Mat image;
  image = imread("1.png", IMREAD_GRAYSCALE);

  namedWindow("Imagem Original", WINDOW_AUTOSIZE);
  imshow("Imagem Original", image);

  Mat kernel = Mat::ones(3, 3, CV_8UC1);

  Mat res;
  morphologyEx(image, res, MORPH_GRADIENT, kernel);

  namedWindow("Imagem Processada", WINDOW_AUTOSIZE);
  imshow("Imagem Processada", res);

  waitKey(0);
  return 0;
}
