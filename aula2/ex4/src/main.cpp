/*
 * Exercício 4: Utilizando o Modelo HSV ou HSI faça a equalização do Histograma
 * de uma imagem.
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
  // Abre a imagem.
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

  // Transforma BGR -> HSV.
  Mat image_hsv;
  cvtColor(image, image_hsv, CV_BGR2HSV);

  // Processa o histograma.
  int width = image_hsv.size().width;
  int height = image_hsv.size().height;
  int N = width * height;

  // Calcula a PDF.
  int pdf[256];
  std::fill_n(pdf, 256, 0);

  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      int index = image_hsv.at<Vec3b>(row, col)[2];
      pdf[index]++;
    }
  }

  // Calcula a CDF normalizada.
  int cdf[256];
  std::fill_n(cdf, 256, 0);

  for (int i = 0; i < 256; i++) {
    int sum = 0;

    for (int j = 0; j <= i; j++) {
      sum += pdf[j];
    }

    cdf[i] = (sum * 255) / N;
  }

  // Aplica a equalização do histograma.
  Mat result;
  result = image_hsv.clone();

  // A nova cor é mapeada pelo valor da CDF da cor original.
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      int index = result.at<Vec3b>(row, col)[2];

      result.at<Vec3b>(row, col)[2] = cdf[index];
    }
  }

  // Retorna de HSV para BGR.
  Mat result_bgr;
  cvtColor(result, result_bgr, CV_HSV2BGR);

  // Apresenta os resultados.
  namedWindow("Imagem Original", WINDOW_AUTOSIZE);
  imshow("Imagem Original", image);

  namedWindow("Imagem Corrigida", WINDOW_AUTOSIZE);
  imshow("Imagem Corrigida", result_bgr);

  imwrite("../dados/result.jpg", result_bgr);

  waitKey(0);
  return 0;
}
