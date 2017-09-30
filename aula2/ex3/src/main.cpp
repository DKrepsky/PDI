/*
 * Exercício 3: Faça a equalização do Histograma de uma imagem em níveis de
 * cinza.
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
  image = imread(argv[1], IMREAD_GRAYSCALE);

  if (!image.data) {
    std::cout << "Imagem invalida" << std::endl;
    return -1;
  }

  // Processa o histograma.
  int width = image.size().width;
  int height = image.size().height;
  int N = width * height;

  // Calcula a PDF.
  int pdf[256];
  std::fill_n(pdf, 256, 0);

  uchar *pixel = image.data;

  for (int i = 0; i < N; i++) {
    pdf[pixel[i]]++;
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
  result.create(height, width, CV_8UC1);

  uchar *rpixel = result.data;

  // A nova cor é mapeada pelo valor da CDF da cor original.
  for (int i = 0; i < N; i++) {
    rpixel[i] = cdf[pixel[i]];
  }

  // Apresenta os resultados.
  namedWindow("Imagem Original", WINDOW_AUTOSIZE);
  imshow("Imagem Original", image);

  namedWindow("Imagem Corrigida", WINDOW_AUTOSIZE);
  imshow("Imagem Corrigida", result);

  imwrite("../dados/result.jpg", result);

  waitKey(0);
  return 0;
}
