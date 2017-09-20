#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "uso: notimg <imagem>" << std::endl;
    return -1;
  }

  int flag = IMREAD_COLOR;

  if (argc == 3) {
    if (argv[2][0] == 'G')
      flag = IMREAD_GRAYSCALE;
  }

  Mat image;
  image = imread(argv[1], flag);

  if (!image.data) {
    std::cout << "[ERRO] Imagem nao encontrada" << std::endl;
    return -1;
  }

  namedWindow("Imagem Original", WINDOW_AUTOSIZE);
  imshow("Imagem Original", image);

  Mat lookUpTable(1, 256, CV_8U);
  uchar* p = lookUpTable.ptr();
  for (int i = 0; i < 256; ++i)
    p[i] = 255 - i;

  Mat neg;
  LUT(image, lookUpTable, neg);

  namedWindow("Imagem Negativa", WINDOW_AUTOSIZE);
  imshow("Imagem Negativa", neg);

  waitKey(0);

  return 0;
}
