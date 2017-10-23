/*
 * A- Fazer a TF de uma imagem, zerar as bordas da matriz e em seguida
 * calcular a inversa para obter a nova imagem processada.
 *
 * B- Fazer a TF de uma imagem, zerar os cantos da matriz resultante da
 * TF e em seguida calcular a inversa para obter a nova imagem processada.
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

void plot_spectrum(Mat &real, Mat &imag) {
  Mat mag;

  magnitude(real, imag, mag);

  // Apply log.
  mag += Scalar::all(1);
  log(mag, mag);

  shift(mag);

  normalize(mag, mag, 0, 1, NORM_MINMAX);

  namedWindow("|DFT|", WINDOW_AUTOSIZE);
  imshow("|DFT|", mag);

  Mat pha;
  phase(real, imag, pha);
  shift(pha);
  normalize(pha, pha, 0, 1, NORM_MINMAX);

  namedWindow("/_DFT", WINDOW_AUTOSIZE);
  imshow("/_DFT", pha);
}

void ex1a(Mat &img) {
  // Cria um vetor de matriz para abrigar os valores complexos gerados pela dft.
  Mat planes[] = { Mat_<float>(img), Mat::zeros(img.size(), CV_32F) };
  Mat complex;

  merge(planes, 2, complex);

  // Calcula a dft e separa em duas matrizes. planes[0] = Re{complex},
  // planes[1] = Im{complex}.
  dft(complex, complex);
  split(complex, planes);

  int N = img.rows;
  int M = img.cols;

  // Zera as bordas do espectro.
  for (int row = 0; row < N; row++) {
    planes[0].at<float>(row, 0) = 0.0;
    planes[0].at<float>(row, M - 1) = 0.0;

    planes[1].at<float>(row, 0) = 0.0;
    planes[1].at<float>(row, M - 1) = 0.0;
  }

  for (int col = 0; col < M; col++) {
    planes[0].at<float>(0, col) = 0.0;
    planes[0].at<float>(N - 1, col) = 0.0;

    planes[1].at<float>(0, col) = 0.0;
    planes[1].at<float>(N - 1, col) = 0.0;
  }

  // Apresenta o espectro após a operação.
  plot_spectrum(planes[0], planes[1]);

  // Calcula e mostra a idft.
  merge(planes, 2, complex);

  Mat inverse;

  idft(complex, inverse, DFT_SCALE | DFT_REAL_OUTPUT);
  normalize(inverse, inverse, 0, 1, NORM_MINMAX);

  namedWindow("(1-A) IDFT", WINDOW_AUTOSIZE);
  imshow("(1-A) IDFT", inverse);
}

void ex1b(Mat &img) {
  // Cria um vetor de matriz para abrigar os valores complexos gerados pela dft.
  Mat planes[] = { Mat_<float>(img), Mat::zeros(img.size(), CV_32F) };
  Mat complex;

  merge(planes, 2, complex);

  // Calcula a dft e separa em duas matrizes. planes[0] = Re{complex},
  // planes[1] = Im{complex}.
  dft(complex, complex);
  split(complex, planes);

  int N = img.rows;
  int M = img.cols;

  // Zera o nível DC.
  planes[0].at<float>(0, 0) = 0.0;
  planes[1].at<float>(0, 0) = 0.0;

  planes[0].at<float>(0, M - 1) = 0.0;
  planes[1].at<float>(0, M - 1) = 0.0;

  planes[0].at<float>(N - 1, 0) = 0.0;
  planes[1].at<float>(N - 1, 0) = 0.0;

  planes[0].at<float>(N - 1, M - 1) = 0.0;
  planes[1].at<float>(N - 1, M - 1) = 0.0;

  // Apresenta o espectro após a operação.
  plot_spectrum(planes[0], planes[1]);

  // Calcula e mostra a idft.
  merge(planes, 2, complex);

  Mat inverse;

  idft(complex, inverse, DFT_SCALE | DFT_REAL_OUTPUT);
  normalize(inverse, inverse, 0, 1, NORM_MINMAX);

  namedWindow("(1-A) IDFT", WINDOW_AUTOSIZE);
  imshow("(1-A) IDFT", inverse);
}

int main(int argc, char** argv) {

  Mat img;
  img = imread("../teste2.png", IMREAD_GRAYSCALE);

  int optX = getOptimalDFTSize(img.cols);
  int optY = getOptimalDFTSize(img.rows);

  Mat padded;
  copyMakeBorder(img, padded, 0, optY - img.rows, 0, optX - img.cols,
      BORDER_CONSTANT, Scalar::all(0));

  ex1a(padded);
  waitKey(0);

  ex1b(padded);
  waitKey(0);

  return 0;
}

