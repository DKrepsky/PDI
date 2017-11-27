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

int main(int argc, char** argv) {
  Mat img;
  img = imread("1.png", IMREAD_COLOR);

  namedWindow("Original", WINDOW_AUTOSIZE);
  imshow("Original", img);

  Mat gray;
  cvtColor(img, gray, COLOR_BGR2GRAY);

  namedWindow("Cinza Normal", WINDOW_AUTOSIZE);
  imshow("Cinza Normal", gray);

  int N = img.rows;
  int M = img.cols;

  Mat data = Mat(N * M, 3, CV_64FC1);

  for (int row = 0; row < N; row++) {
    for (int col = 0; col < M; col++) {
      data.at<double>(row * N + col, 0) = img.at<Vec3b>(row, col).val[0];
      data.at<double>(row * N + col, 1) = img.at<Vec3b>(row, col).val[1];
      data.at<double>(row * N + col, 2) = img.at<Vec3b>(row, col).val[2];
    }
  }

  PCA pca_analysis(data, Mat(), CV_PCA_DATA_AS_ROW);

  //Store the center of the object
  vector<double> mean(3);
  mean[0] = pca_analysis.mean.at<double>(0, 0);
  mean[1] = pca_analysis.mean.at<double>(0, 1);
  mean[2] = pca_analysis.mean.at<double>(0, 2);

  cout << "Mean [" << mean[0] << ", " << mean[1] << ", " << mean[2] << "]"
      << endl;

  //Store the eigenvalues and eigenvectors
  vector<Point3d> eigen_vecs(3);
  vector<double> eigen_val(3);

  for (int i = 0; i < 3; ++i) {
    eigen_vecs[i] = Point3d(pca_analysis.eigenvectors.at<double>(i, 0),
        pca_analysis.eigenvectors.at<double>(i, 1),
        pca_analysis.eigenvectors.at<double>(i, 2));

    eigen_val[i] = pca_analysis.eigenvalues.at<double>(i);
  }

  int max_index =
      (eigen_val[0] > eigen_val[1]) ?
          ((eigen_val[0] > eigen_val[2]) ? 0 : 2) :
          ((eigen_val[1] > eigen_val[2]) ? 1 : 2);

  cout << "Max eigen val " << eigen_val[max_index] << endl;

  vector<double> A(3);
  A[0] = eigen_vecs[max_index].x;
  A[1] = eigen_vecs[max_index].y;
  A[2] = eigen_vecs[max_index].z;

  cout << "Max vec [" << A[0] << ", " << A[1] << ", " << A[2] << "]" << endl;

  Mat Y = Mat(N, M, CV_8UC1);

  for (int row = 0; row < N; row++) {
    for (int col = 0; col < M; col++) {
      Y.at<uchar>(row, col) = A[0] * (img.at<Vec3b>(row, col).val[0] - mean[0])
          + A[1] * (img.at<Vec3b>(row, col).val[1] - mean[1])
          + A[2] * (img.at<Vec3b>(row, col).val[2] - mean[2]);
    }
  }

  namedWindow("Cinza Max", WINDOW_AUTOSIZE);
  imshow("Cinza Max", Y);

  waitKey(0);

  return 0;
}
