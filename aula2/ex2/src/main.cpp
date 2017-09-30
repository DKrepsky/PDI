/*
 * Exercício 2: Binarize uma imagem em níveis de cinza o parâmetro do limiar de
 * binarização é um nível de cinza dado pelo usuário.
 */
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
  // Lê a imagem.
  if (argc < 2) {
    std::cout << "Informe o caminho da imagem" << std::endl;
    return -1;
  }

  Mat image;
  image = imread(argv[1], IMREAD_GRAYSCALE);

  if (!image.data) {
    std::cout << "Imagem invalida" << std::endl;
    return -1;
  }

  // Recebe o limiar do usuário.
  std::cout << "Digite o limiar [0 - 255]: ";
  int limiar;
  std::cin >> limiar;

  if ((limiar > 255) || (limiar < 0)) {
    std::cout << "Limiar inválido" << std::endl;
    return -1;
  }

  // Aplica o limiar.
  uchar *pixel = image.data;
  int size = image.size().width * image.size().height;

  for (int i = 0; i < size; i++) {
    pixel[i] = (pixel[i] > (uchar) limiar) ? 255 : 0;
  }

// Mostra o resultado.
  namedWindow("Display Image", WINDOW_AUTOSIZE);
  imshow("Display Image", image);

  waitKey(0);
}
