/*
 * Faça um programa que: Mostre a quantidade de manchas na imagem; pinte de azul
 *  as manchas sem buracos; e pinte de verde as manchas com buracos. Pinte de
 *  vermelho a maior mancha, Pinte de amarelo a menor mancha,
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  Mat img;
  img = imread("fig.png", IMREAD_GRAYSCALE);

  Mat colorImg;
  cvtColor(img, colorImg, CV_GRAY2BGR);

  threshold(img, img, 0, 255, THRESH_BINARY_INV);

  // Encontra todas as manchas.
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours(img, contours, hierarchy, CV_RETR_CCOMP, CHAIN_APPROX_SIMPLE);

  int maxArea = 0;
  int minArea = numeric_limits<int>::max();
  int maxIndex = -1;
  int minIndex = -1;

  for (unsigned int i = 0; i < contours.size(); i++) {
    // Pinta de azul as manchas sem buraco e de verde as com buraco.
    if (hierarchy[i].val[2] < 0) {
      if (hierarchy[i].val[3] < 0) {
        floodFill(colorImg, Point(contours[i][0].x, contours[i][0].y),
            Scalar(255, 0, 0), nullptr, 10, 10, 8);
      } else {
        floodFill(colorImg, Point(contours[i][0].x, contours[i][0].y),
            Scalar(0, 255, 0), nullptr, 10, 10, 8);
      }
    }

    // Calcula a area das manchas.
    if (hierarchy[i].val[3] < 0) {
      int area = contourArea(contours[i]);

      if (area > maxArea) {
        maxArea = area;
        maxIndex = i;
      }
      if (area < minArea) {
        minArea = area;
        minIndex = i;
      }
    }
  }

  cout << "Total de manchas: " << contours.size() << endl;

  // Pinta maior e menor mancha.
  floodFill(colorImg, Point(contours[maxIndex][0].x, contours[maxIndex][0].y),
      Scalar(0, 0, 255), nullptr, 10, 10, 8);

  floodFill(colorImg, Point(contours[minIndex][0].x, contours[minIndex][0].y),
      Scalar(0, 255, 255), nullptr, 10, 10, 8);

  // Apresenta o resultado.
  namedWindow("Manchas", WINDOW_AUTOSIZE);
  imshow("Manchas", colorImg);

  waitKey(0);

  return 0;
}
