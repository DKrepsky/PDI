/*
 * Implemente filtragem na frequência, utilizando a gaussiana como filtro passa
 * baixa ou passa alta ou passa faixa. Escolha uma filtragem.
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void shift(Mat &dft) {
  // Remove 1 pixel se a dft tiver numero impar de pixels.
  dft = dft(Rect(0, 0, dft.cols & -2, dft.rows & -2));

  // Troca os quadrantes para deixar DC no centro.
  int cx = dft.cols / 2;
  int cy = dft.rows / 2;
  Mat q0(dft, Rect(0, 0, cx, cy));
  Mat q1(dft, Rect(cx, 0, cx, cy));
  Mat q2(dft, Rect(0, cy, cx, cy));
  Mat q3(dft, Rect(cx, cy, cx, cy));
  Mat tmp;
  q0.copyTo(tmp);
  q3.copyTo(q0);
  tmp.copyTo(q3);
  q1.copyTo(tmp);
  q2.copyTo(q1);
  tmp.copyTo(q2);
}

void plot_spectrum(Mat &real, Mat &imag, string text) {
  Mat mag;

  magnitude(real, imag, mag);

  // Apply log.
  mag += Scalar::all(1);
  log(mag, mag);

  shift(mag);

  normalize(mag, mag, 0, 1, NORM_MINMAX);

  string magTitle = "|DFT| - ";
  magTitle.append(text);

  namedWindow(magTitle, WINDOW_AUTOSIZE);
  imshow(magTitle, mag);

  Mat pha;
  phase(real, imag, pha);
  shift(pha);
  normalize(pha, pha, 0, 1, NORM_MINMAX);

  string phaTitle = "/_DFT - ";
  phaTitle.append(text);

  namedWindow(phaTitle, WINDOW_AUTOSIZE);
  imshow(phaTitle, pha);
}

// Calcula o valor da gaussiana com variância s no ponto (u, v).
float coeff(float u, float v, float s) {
  float c = exp(-(u * u + v * v) / (2 * s * s));

  return c;
}

Mat createLowPassGaussianFilter(Size size, float sigma) {

  Mat mask;
  mask.create(size, CV_32F);

  for (int u = 0; u < size.height; u++) {
    for (int v = 0; v < size.width; v++) {
      float unorm = (float) (u - size.height / 2) / ((float) size.height / 2);
      float vnorm = (float) (v - size.width / 2) / ((float) size.width / 2);

      mask.at<float>(u, v) = coeff(unorm, vnorm, sigma);
    }
  }

//  normalize(mask, mask, 0, 1, NORM_MINMAX);

  return mask;
}

int main(int argc, char** argv) {
  Mat img;
  img = imread("../teste.png", IMREAD_GRAYSCALE);

  namedWindow("Imagem Original", WINDOW_AUTOSIZE);
  imshow("Imagem Original", img);

  // Adiciona pixels as bordas se necessário.
  int optX = getOptimalDFTSize(img.cols);
  int optY = getOptimalDFTSize(img.rows);

  Mat padded;
  copyMakeBorder(img, padded, 0, optY - img.rows, 0, optX - img.cols,
      BORDER_CONSTANT, Scalar::all(0));

  // Constroi o filtro passa-baixas.
  Mat filter;
  filter = createLowPassGaussianFilter(img.size(), 0.01);

  namedWindow("Filter Mask", WINDOW_AUTOSIZE);
  imshow("Filter Mask", filter);

  shift(filter);

  // Cria o filtro com valores complexos para a função mulSpectrum.
  Mat fplanes[] = { Mat_<float>(filter), Mat::zeros(padded.size(), CV_32F) };
  Mat fcomplex;

  merge(fplanes, 2, fcomplex);

  // Calcula a dft da imagem.
  Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
  Mat complex;

  merge(planes, 2, complex);

  dft(complex, complex);
  split(complex, planes);

  // Mostra o spectro antes do filtro.
  plot_spectrum(planes[0], planes[1], "Original");

  // Aplica o filtro.
  mulSpectrums(complex, fcomplex, complex, 0);

  // Plota o espectro após o filtro.
  split(complex, planes);
  plot_spectrum(planes[0], planes[1], "Filtrado");

  // Reconstroi a imagem.
  Mat inverse;

  idft(complex, inverse, DFT_SCALE | DFT_REAL_OUTPUT);
  normalize(inverse, inverse, 0, 1, NORM_MINMAX);

  namedWindow("Imagem Filtrada", WINDOW_AUTOSIZE);
  imshow("Imagem Filtrada", inverse);

  waitKey(0);

  return 0;
}
