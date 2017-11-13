/*
 * Apresente uma tabela que mostra todos os setes momentos centrais normalisados
 * e invariantes. Veja Gonzalez (2010) pagina 555. Não esqueça de apresentar os
 * dados no formato sinal(phi[i])log10(|phi[i]|).
 *
 * Referencia:
 * - Gonzalez (2010)
 */

#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double momentum(Mat &img, int p, int q) {

  int M = img.cols;
  int N = img.rows;

  double res = 0;

  for (int x = 0; x < M - 1; x++) {
    double a = pow(x, p);

    for (int y = 0; y < N - 1; y++) {

      double b = pow(y, q);

      res += a * b * img.at<uchar>(y, x);
    }
  }

  return res;
}

double mu(Mat &img, int p, int q) {

  double m_00 = momentum(img, 0, 0);
  double m_01 = momentum(img, 0, 1);
  double m_10 = momentum(img, 1, 0);

  double x_ = m_10 / m_00;
  double y_ = m_01 / m_00;

  int M = img.cols;
  int N = img.rows;

  double res = 0;

  for (int x = 0; x < M - 1; x++) {

    double a = pow(x - x_, p);

    for (int y = 0; y < N - 1; y++) {
      double b = pow(y - y_, q);

      res += a * b * img.at<uchar>(y, x);
    }
  }

  return res;
}

double eta(Mat &img, int p, int q) {

  double mu_pq = mu(img, p, q);
  double mu_00 = mu(img, 0, 0);

  double gamma = ((p + q) / 2) + 1;

  double res = mu_pq / pow(mu_00, gamma);

  return res;
}

void phi(Mat &img, double *phi_vec) {

  double eta_11 = eta(img, 1, 1);
  double eta_20 = eta(img, 2, 0);
  double eta_02 = eta(img, 0, 2);
  double eta_12 = eta(img, 1, 2);
  double eta_21 = eta(img, 2, 1);
  double eta_30 = eta(img, 3, 0);
  double eta_03 = eta(img, 0, 3);

  phi_vec[0] = eta_20 + eta_02;

  phi_vec[1] = pow(eta_20 - eta_02, 2) + (4 * pow(eta_11, 2));

  phi_vec[2] = pow(eta_30 - 3 * eta_12, 2) + pow(3 * eta_21 - eta_03, 2);

  phi_vec[3] = pow(eta_30 + eta_12, 2) + pow(eta_21 - eta_03, 2);

  phi_vec[4] = (eta_30 - 3 * eta_12) * (eta_30 + eta_12)
      * (pow(eta_30 + eta_12, 2) - 3 * pow(eta_21 + eta_03, 2))
      + (3 * eta_21 - eta_03) * (eta_21 + eta_03)
          * (3 * pow(eta_30 + eta_12, 2) - pow(eta_21 + eta_03, 2));

  phi_vec[5] = (eta_20 - eta_02)
      * (pow(eta_30 + eta_12, 2) - pow(eta_21 + eta_03, 2))
      + 4 * eta_11 * (eta_30 + eta_12) * (eta_21 + eta_03);

  phi_vec[6] = (3 * eta_21 - eta_03) * (eta_30 + eta_12)
      * (pow(eta_30 + eta_12, 2) - 3 * pow(eta_21 + eta_03, 2))
      + (3 * eta_12 - eta_30) * (eta_21 + eta_03)
          * (3 * pow(eta_30 + eta_12, 2) - pow(eta_21 + eta_03, 2));
}

void print_table(Mat &table) {

  cout << setw(12) << left << "Momento In.";
  cout << setw(12) << left << "Original";
  cout << setw(12) << left << "Trans.";
  cout << setw(12) << left << "Escala";
  cout << setw(12) << left << "Espelho";
  cout << setw(12) << left << "Rot. 45";
  cout << setw(12) << left << "Rot. 90";
  cout << endl;

  cout.precision(4);
  cout << showpoint;

  for (int i = 0; i < 7; i++) {

    double im_1 = copysign(log10(fabs(table.at<double>(i, 0))),
        table.at<double>(i, 0));
    double im_2 = copysign(log10(fabs(table.at<double>(i, 1))),
        table.at<double>(i, 1));
    double im_3 = copysign(log10(fabs(table.at<double>(i, 2))),
        table.at<double>(i, 2));
    double im_4 = copysign(log10(fabs(table.at<double>(i, 3))),
        table.at<double>(i, 3));
    double im_5 = copysign(log10(fabs(table.at<double>(i, 4))),
        table.at<double>(i, 4));
    double im_6 = copysign(log10(fabs(table.at<double>(i, 5))),
        table.at<double>(i, 5));

    cout << setw(12) << left << i + 1;
    cout << showpos;
    cout << setw(12) << left << im_1;
    cout << setw(12) << left << im_2;
    cout << setw(12) << left << im_3;
    cout << setw(12) << left << im_4;
    cout << setw(12) << left << im_5;
    cout << setw(12) << left << im_6;
    cout << noshowpos;
    cout << endl;
  }
}

int main(int argc, char** argv) {
  Mat image[6];

  image[0] = imread("0.png", IMREAD_GRAYSCALE);
  image[1] = imread("1.png", IMREAD_GRAYSCALE);
  image[2] = imread("2.png", IMREAD_GRAYSCALE);
  image[3] = imread("3.png", IMREAD_GRAYSCALE);
  image[4] = imread("4.png", IMREAD_GRAYSCALE);
  image[5] = imread("5.png", IMREAD_GRAYSCALE);

// A table é a tabela com os resultados, onde as colunas representam as
// imagens e as linhas os valores de phi
  Mat table;
  table.create(Size(6, 7), CV_64F);

// Vetor para armazenar os valores de phi calculados.
  double phi_vec[7];

// Calcula os 7 valores de phi e insere na tabela.
  for (int im = 0; im < 6; im++) {
    phi(image[im], phi_vec);

    for (int i = 0; i < 7; i++) {
      table.at<double>(i, im) = phi_vec[i];
    }
  }

  cout << endl << endl << endl;

  print_table(table);

  return 0;
}
