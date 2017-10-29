/*
 * 1) Usando morfologia matemática retire os ruídos das imagens. Quais operações
 * morfológicas aplicáveis?
 *
 * Referências:
 *  - https://docs.opencv.org/trunk/d9/d61/tutorial_py_morphological_ops.html
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  Mat kern = Mat::ones(5, 5, CV_8UC1);

  // Realiza abertura na primeira imagem.
  Mat img1;
  img1 = imread("1.png", IMREAD_GRAYSCALE);

  namedWindow("Imagem 1 Original", WINDOW_AUTOSIZE);
  imshow("Imagem 1 Original", img1);

  Mat res1;
  morphologyEx(img1, res1, MORPH_OPEN, kern);

  namedWindow("Imagem 1 Processada", WINDOW_AUTOSIZE);
  imshow("Imagem 1 Processada", res1);

  waitKey(0);

  // Realiza o fechamento na segunda imagem.
  Mat img2;
  img2 = imread("2.png", IMREAD_GRAYSCALE);

  namedWindow("Imagem 2 Original", WINDOW_AUTOSIZE);
  imshow("Imagem 2 Original", img2);

  Mat res2;
  morphologyEx(img2, res2, MORPH_CLOSE, kern);

  namedWindow("Imagem 2 Processada", WINDOW_AUTOSIZE);
  imshow("Imagem 2 Processada", res2);

  waitKey(0);

  // Aplica abertura e fechamento na imagem 3.
  Mat img3;
  img3 = imread("3.png", IMREAD_GRAYSCALE);

  namedWindow("Imagem 3 Original", WINDOW_AUTOSIZE);
  imshow("Imagem 3 Original", img3);

  Mat res3;
  morphologyEx(img3, res3, MORPH_OPEN, kern);
  morphologyEx(res3, res3, MORPH_CLOSE, kern);

  namedWindow("Imagem 3 Processada", WINDOW_AUTOSIZE);
  imshow("Imagem 3 Processada", res3);

  waitKey(0);
  return 0;
}
