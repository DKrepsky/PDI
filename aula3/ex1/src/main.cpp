/*
 * Implemente os filtros no domínio do espaço: Média, Sobel, Mediana.
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

void applyMedia(Mat &img) {
  Mat media;

  Mat kmedia = Mat::ones(3, 3, CV_32F) * (1.0 / 9.0);

  filter2D(img, media, img.depth(), kmedia);

  namedWindow("Filtro Media", WINDOW_AUTOSIZE);
  imshow("Filtro Media", media);
  imwrite("media.png", media);
}

void applyMediana(Mat &img) {
  Mat mediana;

  medianBlur(img, mediana, 3);

  namedWindow("Filtro Mediana", WINDOW_AUTOSIZE);
  imshow("Filtro Mediana", mediana);
  imwrite("mediana.png", mediana);
}

void applySobel(Mat &img) {
  Mat sobel;

  Sobel(img, sobel, img.depth(), 1, 1);

  namedWindow("Filtro Sobel", WINDOW_AUTOSIZE);
  imshow("Filtro Sobel", sobel);
  imwrite("sobel.png", sobel);
}

int main(int argc, char** argv) {
  Mat image;
  image = imread("lenna.png", IMREAD_COLOR);

  namedWindow("Imagem Original", WINDOW_AUTOSIZE);
  imshow("Imagem Original", image);

  applyMedia(image);

  applyMediana(image);

  applySobel(image);

  waitKey(0);
  return 0;
}
