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

  for (int row = 0; row < img.size().height; row++) {
    // y é o indice da linha na nova imagem.
    int y = 2 * row;

    for (int col = 0; col < img.size().width; col++) {
      // x é o indice da coluna na nova imagem.
      int x = 2 * col;

      Vec3b val = img.at<Vec3b>(row, col);

      result.at<Vec3b>(y, x) = val;
      result.at<Vec3b>(y + 1, x) = val;
      result.at<Vec3b>(y, x + 1) = val;
      result.at<Vec3b>(y + 1, x + 1) = val;
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

  for (int row = 0; row < result.size().height; row++) {
    int y = 2 * row;

    for (int col = 0; col < result.size().width; col++) {

      int x = 2 * col;

      result.at<Vec3b>(row, col) = img.at<Vec3b>(y, x);
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

  int width = img.size().width;
  int height = img.size().height;

  result.create(height * 2, width * 2, img.type());

  // Horizontal
  for (int row = 0; row < height - 1; row++) {
    int y = 2 * row;

    for (int col = 0; col < width - 1; col++) {
      int x = 2 * col;

      Vec3b pixel = img.at<Vec3b>(row, col);
      Vec3b nextPixel = img.at<Vec3b>(row, col + 1);
      Vec3b meanPixel = pixel / 2 + nextPixel / 2;

      result.at<Vec3b>(y, x) = pixel;
      result.at<Vec3b>(y, x + 1) = meanPixel;
    }
  }

  // Borda vertical direita.
  for (int row = 0; row < height; row++) {
    int y = 2 * row;
    Vec3b pixel = img.at<Vec3b>(row, width - 1);

    result.at<Vec3b>(y, 2 * width - 1) = pixel;
    result.at<Vec3b>(y, 2 * width - 2) = pixel;
  }

  for (int row = 0; row < 2*height - 1; row++) {
    int y = 2 * row;

    for (int col = 0; col < width; col++) {
      int x = 2 * col;

      Vec3b pixel = result.at<Vec3b>(row, col);
      Vec3b nextPixel = result.at<Vec3b>(row + 1, col);
      Vec3b meanPixel = pixel / 2 + nextPixel / 2;

      result.at<Vec3b>(y + 1, x) = meanPixel;

      pixel = img.at<Vec3b>(row, col + 1);
      nextPixel = img.at<Vec3b>(row + 1, col + 1);
      meanPixel = pixel / 2 + nextPixel / 2;

      result.at<Vec3b>(y + 1, x + 1) = meanPixel;
    }
  }
//
//  // Borda horizontal inferior.
//  for (int col = 0; col < width * 2; col++) {
//    result.at<Vec3b>(height * 2 - 1, col) = result.at<Vec3b>(height * 2 - 2,
//        col);
//  }

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

// Apresenta a imagem original, ampliada e a reduzida.
  namedWindow("Imagem Original", WINDOW_AUTOSIZE);
  imshow("Imagem Original", image);

  namedWindow("ZoomIn", WINDOW_AUTOSIZE);
  imshow("ZoomIn", zoomIn);

  namedWindow("ZoomOut", WINDOW_AUTOSIZE);
  imshow("ZoomOut", zoomOut);

  namedWindow("ZoomIn Linear", WINDOW_AUTOSIZE);
  imshow("ZoomIn Linear", zoomInLinear);

  waitKey(0);

  return 0;
}
