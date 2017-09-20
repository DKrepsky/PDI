#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Informe o caminho da imagem" << std::endl;
    return -1;
  }

  Mat image;
  image = imread(argv[1], IMREAD_COLOR);

  if (!image.data) {
    std::cout << "Imagem invalida" << std::endl;
    return -1;
  }

  Mat result = image.clone();

  result = result.mul(Scalar::all(0.5));

//  Core *= Scalar(50);
  namedWindow("Imagem Original", WINDOW_AUTOSIZE);
  imshow("Imagem Original", image);

  namedWindow("Imagem Resultante", WINDOW_AUTOSIZE);
  imshow("Imagem Resultante", result);

  int key = 0;

  do {
    key = waitKey(0);
  } while (key == 0);

  return 0;
}
