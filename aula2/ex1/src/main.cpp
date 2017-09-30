/*
 * Exercício 1: Implemente, para uma imagem colorida e em níveis de Cinza, o
 * Zoom in e o Zoom out (aproximar/afastar).
 * Para o aumento e diminuição da imagem:
 *
 * - Implementação 1: Para cada pixel da imagem original transforme-o em 4 pixel
 *   da imagem aumentada e vice e versa (verifique o efeito desta transformação
 *   na imagem).
 *
 * - Implementação 2: Utilize a média entre os pixeis para aumentar a imagem
 *   (Zoom linear verifique o efeito desta transformação na imagem) 09/10/2017
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

/*
 * Amplia uma imagem em 4x (zoom in simples).
 *
 * Transforma cada pixel na imagem original em 4 pixels na imagem ampliada.
 *
 * Ex:
 *
 *                                 Com Zoom In
 *                          -------------------------
 *    Original              | 135 | 135 | 148 | 148 |
 * -------------            -------------------------
 * | 135 | 148 |            | 135 | 135 | 148 | 148 |
 * -------------     ->     -------------------------
 * | 186 | 155 |            | 186 | 186 | 155 | 155 |
 * -------------            -------------------------
 *           4x4            | 186 | 186 | 155 | 155 |
 *                          -------------------------
 *                                                8x8
 */
Mat applyZoomIn(Mat &img) {
  Mat result;

  result.create(img.size().width * 2, img.size().height * 2, img.type());

  for (int i = 0; i < img.size().width; i++) {
    for (int j = 0; j < img.size().height; j++) {

      Vec3b val = img.at<Vec3b>(i, j);

      int x = 2 * i;
      int y = 2 * j;

      result.at<Vec3b>(x, y) = val;
      result.at<Vec3b>(x + 1, y) = val;
      result.at<Vec3b>(x, y + 1) = val;
      result.at<Vec3b>(x + 1, y + 1) = val;
    }
  }

  return result;
}

/*
 * Realiza a decimação da imagem em 4x (zoom out simples).
 *
 * A cada 2x2 pixels na imagem original, somente o primeiro é copiado para a
 * imagem reduzida, os outros pixels são descartados.
 *
 * Ex:
 *
 *          Original
 * -------------------------
 * | 135 | 122 | 175 | 132 |          Com Zoom Out
 * -------------------------          -------------
 * | 155 | 145 | 179 | 163 |          | 135 | 175 |
 * -------------------------    ->    -------------
 * | 154 | 122 | 165 | 188 |          | 154 | 165 |
 * -------------------------          -------------
 * | 165 | 112 | 198 | 158 |                    4x4
 * -------------------------
 *                       8x8
 *
 */
Mat applyZoomOut(Mat &img) {
  Mat result;

  result.create(img.size().width / 2, img.size().height / 2, img.type());

  for (int i = 0; i < result.size().width; i++) {
    for (int j = 0; j < result.size().height; j++) {

      int x = 2 * i;
      int y = 2 * j;

      result.at<Vec3b>(i, j) = img.at<Vec3b>(x, y);
    }
  }

  return result;
}

/*
 * Amplia uma imagem em 4x com interpolação linear.
 *
 * O valor de cada 2x2 pixels na imagem aumentada é iguar ao valor da média de
 * 2x2 pixels na imagem original.
 *
 * Note que não é adicionado "padding" à imagem, assim, a última linha e última
 * coluna apresentam distorções.
 *
 * Ex. para o pixel (0,0):
 *
 * Pixels com zoom = (135 + 148 + 186 + 155) / 4 = 156.
 *
 *    Original               Com Zoom In
 * -------------            -----------------
 * | 135 | 148 |            | 156 | 156 | ...
 * -------------     ->     -----------------
 * | 186 | 155 |            | 156 | 156 | ...
 * -------------            -----------------
 *           4x4            | ... | ... | ...
 *                                       8x8
 */
Mat applyZoomInLinear(Mat &img) {
  Mat result;

  result.create(img.size().width * 2, img.size().height * 2, img.type());

  for (int i = 0; i < img.size().width; i++) {
    for (int j = 0; j < img.size().height; j++) {

      Vec3b mean;

      mean = img.at<Vec3b>(i, j) / 4;
      mean += img.at<Vec3b>(i + 1, j) / 4;
      mean += img.at<Vec3b>(i, j + 1) / 4;
      mean += img.at<Vec3b>(i + 1, j + 1) / 4;

      int x = 2 * i;
      int y = 2 * j;

      result.at<Vec3b>(x, y) = mean;
      result.at<Vec3b>(x + 1, y) = mean;
      result.at<Vec3b>(x, y + 1) = mean;
      result.at<Vec3b>(x + 1, y + 1) = mean;

    }
  }

  return result;
}

/*
 * Reduz uma imagem em 4x.
 *
 * A decimação é realizada com base na média dos 2x2 pixels da imagem original.
 *
 * Ex:
 *
 *          Original
 * -------------------------
 * | 135 | 122 | 175 | 132 |          Com Zoom Out
 * -------------------------          -------------
 * | 155 | 145 | 179 | 163 |          | 139 | 162 |
 * -------------------------    ->    -------------
 * | 154 | 122 | 165 | 188 |          | 138 | 177 |
 * -------------------------          -------------
 * | 165 | 112 | 198 | 158 |                    4x4
 * -------------------------
 *                       8x8
 */
Mat applyZoomOutLinear(Mat &img) {
  Mat result;

  result.create(img.size().width / 2, img.size().height / 2, img.type());

  for (int i = 0; i < result.size().width; i++) {
    for (int j = 0; j < result.size().height; j++) {

      int x = 2 * i;
      int y = 2 * j;

      Vec3b mean;

      mean = img.at<Vec3b>(x, y) / 4;
      mean += img.at<Vec3b>(x + 1, y) / 4;
      mean += img.at<Vec3b>(x, y + 1) / 4;
      mean += img.at<Vec3b>(x + 1, y + 1) / 4;

      result.at<Vec3b>(i, j) = mean;
    }
  }

  return result;
}

int main(int argc, char** argv) {

// Lê a imagem.
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

// Aplica zoom in e out.
  Mat zoomIn = applyZoomIn(image);

  Mat zoomOut = applyZoomOut(image);

  Mat zoomInLinear = applyZoomInLinear(image);

  Mat zoomOutLinear = applyZoomOutLinear(image);

// Apresenta a imagem original, ampliada e a reduzida.
  namedWindow("Imagem Original", WINDOW_AUTOSIZE);
  imshow("Imagem Original", image);

  namedWindow("ZoomIn", WINDOW_AUTOSIZE);
  imshow("ZoomIn", zoomIn);

  namedWindow("ZoomOut", WINDOW_AUTOSIZE);
  imshow("ZoomOut", zoomOut);

  namedWindow("ZoomIn Linear", WINDOW_AUTOSIZE);
  imshow("ZoomIn Linear", zoomInLinear);

  namedWindow("ZoomOut Linear", WINDOW_AUTOSIZE);
  imshow("ZoomOut Linear", zoomOutLinear);

  waitKey(0);

  return 0;
}
