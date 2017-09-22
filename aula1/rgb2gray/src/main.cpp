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
  cvtColor(src, dst, COLOR_RGB2GRAY);
}

void applyGain(Mat &src, Mat &dst, double gain) {

  dst = src.mul(Scalar::all(gain));
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

  bool process = true;
  double gain = 1.0;

  while (process) {
    uint8_t key = static_cast<uint8_t>(waitKey(0));

    switch (key) {

    // Process '+' character to increase gain.
    case 0x2B:
    case 0xAB:
      gain += 0.1;
      applyGain(gray, final, gain);
      imshow("Imagem Resultante", final);
      break;

      // Process '-' character to decrease gain.
    case 0x2D:
    case 0xAD:
      gain -= +0.1;
      applyGain(gray, final, gain);
      imshow("Imagem Resultante", final);
      break;

      // Process escape character to exit.
      // Character 0xFF is returned when all windows are closed.
    case 0x1B:
    case 0xFF:
      process = false;
      break;
    }

    // Process 's' character to save the image.
    case 's':

    break;

  }
  return 0;
}
