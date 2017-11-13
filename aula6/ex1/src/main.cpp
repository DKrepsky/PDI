/*
 * Dada uma imagem segmentada, implemente em OpenCV um programa que calcula os
 * descritores topológicos H, E, C.
 *
 * Referencia:
 * - Ex. 2 aula 3.
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  Mat image;
  image = imread("2.png", IMREAD_GRAYSCALE);

  threshold(image, image, 0, 255, THRESH_OTSU);

  image = Scalar(255) - image;

  // Encontra todos os contornos.
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours(image, contours, hierarchy, CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE);

  int H = 0;
  int C = 0;

  // Determina se é um buraco ou componente conexo;
  for (auto &it : hierarchy) {
    if (it.val[3] < 0) {
      C++;
    } else {
      H++;
    }
  }

  int E = C - H;

  cout << "H = " << H << "; C = " << C << "; E = " << E << ";" << endl;
  waitKey(0);

  return 0;
}
