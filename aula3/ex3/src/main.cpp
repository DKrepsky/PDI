/*
 * Determine se a peça possui 6 ou 4 buracos. Se a peça contiver 6 buracos
 * classifique-a como tipo A, Se a peça contiver 4 buracos classifique-a como
 * tipo B, caso contrário rejeite a peça classificando a como tipo R.
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  Mat image;
  image = imread("6.png", IMREAD_GRAYSCALE);

  threshold(image, image, 0, 255, THRESH_BINARY);

  vector<vector<Point> > contours;
  findContours(image, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

  switch (contours.size()) {
  case 4:
    cout << "Tipo: B" << endl;
    break;

  case 6:
    cout << "Tipo: A" << endl;
    break;

  default:
    cout << "Tipo: R" << endl;
    break;
  }

  waitKey(0);
  return 0;
}
