/*
 * Faça um programa que: Mostre a quantidade de manchas na imagem; pinte de azul
 *  as manchas sem buracos; e pinte de verde as manchas com buracos. Pinte de
 *  vermelho a maior mancha, Pinte de amarelo a menor mancha,
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
  Mat image;
  image = imread("fig.png", IMREAD_GRAYSCALE);

  namedWindow("Imagem Original", WINDOW_AUTOSIZE);
  imshow("Imagem Original", image);

  waitKey(0);

  return 0;
}
