/*
 * Exercício 5: Implemente a seguinte transformação s[k] = T(k,L,H) em uma
 * imagem em níveis de cinza.
 * Onde:
 *
 * k - é a cor antiga da imagem
 * s - é a nova cor transformada da imagem
 * T(k) obedece a seguinte formulação:
 * Dado 2 parâmetros L e H, onde:
 * L e H são cores variando de 0 a 255.
 * L é sempre menor ou igual a H.
 * todo valor de k que for menor que L vira 0 (preto)
 * todo valor de k que for igual ou maior que H vira 255 (branco)
 * todo valor entre L e H a transformação de t(k) obedecerá a
 * equação da reta cujo pontos extremos são (L,0) e (H,255), ou seja dado k
 * calcule (k, t(k)).
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

  // Recebe o valor de L.
  std::cout << "Digite o valor de L [0 - 255]: ";
  int L;
  std::cin >> L;

  if ((L > 255) || (L < 0)) {
    std::cout << "Valor inválido" << std::endl;
    return -1;
  }

  // Recebe o valor de H.
  std::cout << "Digite o valor de H [0 - 255]: ";
  int H;
  std::cin >> H;

  if ((H > 255) || (H < L)) {
    std::cout << "Valor inválido" << std::endl;
    return -1;
  }

  // Calcula lookup table.
  uchar table[256];

  for (int i = 0; i < 256; i++) {
    if (i < L) {
      table[i] = 0;
    } else if (i > H) {
      table[i] = 255;
    } else {
      table[i] = (H / L) * (i - L);
    }
  }

  // Aplica a transformada.
  int height = image.size().height;
  int width = image.size().width;

  Mat result;
  result.create(height, width, CV_8UC1);

  // Ponteiro para os pixels.
  uchar *pimage = image.data;
  uchar *presult = result.data;

  int size = height * width;

  // Aplica os valores da lookup table.
  for (int i = 0; i < size; i++) {
    uchar index = pimage[i];
    presult[i] = table[index];
  }

  // Mostra o resultado.
  namedWindow("Original", WINDOW_AUTOSIZE);
  imshow("Original", image);

  namedWindow("Processada", WINDOW_AUTOSIZE);
  imshow("Processada", result);

  imwrite("../dados/result.jpg", result);

  waitKey(0);
  return 0;
}
