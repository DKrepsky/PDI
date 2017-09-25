#include <stdint.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

bool loadImage(char *path, Mat &img) {
  img = imread(path, IMREAD_COLOR);

  if (!img.data) {
    std::cout << "Imagem invalida" << std::endl;
    return false;
  }

  return true;
}

bool saveImage(char *path, Mat &img) {
  if (!imwrite(path, img)) {
    std::cout << "Falha ao salvar a imagem" << std::endl;
    return false;
  }

  return true;
}

void rgb2gray(Mat &src, Mat &dst) {

  dst = src.clone();

  MatIterator_<Vec3b> it = dst.begin<Vec3b>();
  MatIterator_<Vec3b> end = dst.end<Vec3b>();

  for (; it != end; ++it) {

    int mean = ((*it)[0] + (*it)[1] + (*it)[2]) / 3;
    (*it)[0] = mean;
    (*it)[1] = mean;
    (*it)[2] = mean;
  }
}

void applyGain(Mat &src, Mat &dst, int8_t gain) {
  dst = src + Scalar(gain, gain, gain);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "Informe o caminho da imagem" << std::endl;
    return -1;
  }

  Mat img;

  if (!loadImage(argv[1], img)) {
    return -1;
  }

  namedWindow("Imagem Original", WINDOW_AUTOSIZE);
  imshow("Imagem Original", img);

  Mat gray;
  rgb2gray(img, gray);

  Mat final;
  final = gray.clone();

  namedWindow("Imagem Resultante", WINDOW_AUTOSIZE);
  imshow("Imagem Resultante", final);

  // Process keypress.
  int8_t gain = 0;
  bool process = true;

  while (process) {
    uint8_t key = static_cast<uint8_t>(waitKey(0));

    switch (key) {
    // Use '+' character to increase gain.
    case 0x2B:
    case 0xAB:
      if (gain < 127)
        gain += 1;

      applyGain(gray, final, gain);
      imshow("Imagem Resultante", final);
      break;

      // Use '-' character to decrease gain.
    case 0x2D:
    case 0xAD:
      if (gain > -128)
        gain -= 1;

      applyGain(gray, final, gain);
      imshow("Imagem Resultante", final);
      break;

      // Use 'esc' key to exit.
      // Character 0xFF is returned when all windows are closed.
    case 0x1B:
    case 0xFF:
      process = false;
      break;

      // Press 's' to save the image.
    case 's':
      saveImage(argv[1], final);
      break;
    }
  }

  return 0;
}
