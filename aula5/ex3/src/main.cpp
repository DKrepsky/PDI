/*
 * 3) Implemente a seguinte aplicação da transformada Top Hat, que minimiza o
 * efeito da iluminação não uniforme na segmentação dos objetos de interesse.
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

  Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(50, 50));

  Mat res;
  morphologyEx(image, res, MORPH_TOPHAT, kernel);

  threshold(res, res, 0, 255, THRESH_BINARY | THRESH_OTSU);

  namedWindow("Imagem Final", WINDOW_AUTOSIZE);
  imshow("Imagem Final", res);

  waitKey(0);
  return 0;
}
