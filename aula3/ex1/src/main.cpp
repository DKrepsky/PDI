/*
 * Implemente os filtros no domínio do espaço: Média, Sobel, Mediana.
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

void applyMedia(Mat &img) {
  Mat media;
//  media.create(img.size(), img.type());

  Mat kmedia = Mat::ones(3, 3, CV_32F) * (1.0 / 9.0);

  filter2D(img, media, img.depth(), kmedia);

  namedWindow("Filtro Media", WINDOW_AUTOSIZE);
  imshow("Filtro Media", media);
  imwrite("media.png", media);
}

void applyMediana(Mat &img) {
  Mat mediana;
//  mediana.create(img.size(), img.type());

  medianBlur(img, mediana, 3);

  namedWindow("Filtro Mediana", WINDOW_AUTOSIZE);
  imshow("Filtro Mediana", mediana);
  imwrite("mediana.png", mediana);
}

void applySobel(Mat &img) {

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
