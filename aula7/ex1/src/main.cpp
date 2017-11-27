/*
 * Utilizando OpenCV e a transformada de Hotelling, faça o alinhamento de um
 * objeto em uma imagem.
 *
 * Referencia:
 * - Ex. 2 aula 3.
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
  // Original
  Mat image;
  image = imread("1.png", IMREAD_COLOR);

  namedWindow("Original", WINDOW_AUTOSIZE);
  imshow("Original", image);

  // Segmentada
  Mat gray;
  cvtColor(image, gray, COLOR_BGR2GRAY);

  threshold(gray, gray, 50, 255, THRESH_BINARY_INV | THRESH_OTSU);

  Mat kernel = Mat::ones(4, 4, CV_8UC1);

  morphologyEx(gray, gray, MORPH_CLOSE, kernel);

  namedWindow("Seguimentada", WINDOW_AUTOSIZE);
  imshow("Seguimentada", gray);

  // PCA
  vector<Vec4i> hierarchy;
  vector<vector<Point> > contours;
  findContours(gray, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

  int sz = static_cast<int>(contours[0].size());

  Mat data_pts = Mat(sz, 2, CV_64FC1);

  for (int i = 0; i < data_pts.rows; ++i) {
    data_pts.at<double>(i, 0) = contours[0][i].x;
    data_pts.at<double>(i, 1) = contours[0][i].y;
  }

  //Perform PCA analysis
  PCA pca_analysis(data_pts, Mat(), CV_PCA_DATA_AS_ROW);

  //Store the center of the object
  Point cntr = Point(static_cast<int>(pca_analysis.mean.at<double>(0, 0)),
      static_cast<int>(pca_analysis.mean.at<double>(0, 1)));

  //Store the eigenvalues and eigenvectors
  vector<Point2d> eigen_vecs(2);
  vector<double> eigen_val(2);

  for (int i = 0; i < 2; ++i) {
    eigen_vecs[i] = Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
        pca_analysis.eigenvectors.at<double>(i, 1));
    eigen_val[i] = pca_analysis.eigenvalues.at<double>(i);
  }

  cout << "Eigen Val = [" << eigen_val[0] << ", " << eigen_val[1] << "]"
      << endl;
  cout << "Vec0 = [" << eigen_vecs[0].x << ", " << eigen_vecs[0].y << "]"
      << endl;
  cout << "Vec1 = [" << eigen_vecs[1].x << ", " << eigen_vecs[1].y << "]"
      << endl;

  int max_index = (eigen_val[0] > eigen_val[1]) ? 0 : 1;

  Mat A = Mat(2, 2, CV_64FC1);

  if (max_index == 0) {
    A.at<double>(0, 0) = eigen_vecs[0].y;
    A.at<double>(0, 1) = eigen_vecs[0].x;
    A.at<double>(1, 0) = eigen_vecs[1].y;
    A.at<double>(1, 1) = eigen_vecs[1].x;

  } else {
    A.at<double>(0, 0) = eigen_vecs[1].y;
    A.at<double>(0, 1) = eigen_vecs[1].x;
    A.at<double>(1, 0) = eigen_vecs[0].y;
    A.at<double>(1, 1) = eigen_vecs[0].x;
  }

  cout << "A = " << A << endl;

  int N = image.rows;
  int M = image.cols;

  Mat Y = Mat(2 * N, 2 * M, CV_8UC3);

  for (int row = 0; row < N; row++) {
    for (int col = 0; col < M; col++) {

      int new_row = round(
          A.at<double>(0, 1) * (col - cntr.x)
              + A.at<double>(0, 0) * (row - cntr.y));

      int new_col = round(A.at<double>(1, 1) * (col - cntr.x)
          + A.at<double>(1, 0) * (row - cntr.y));

      new_row += N;
      new_col += M;

      cout << "Row " << new_row << "Col " << new_col << endl;

      Y.at<Vec3b>(new_row, new_col) = image.at<Vec3b>(row, col);
    }
  }

  namedWindow("Resultado", WINDOW_AUTOSIZE);
  imshow("Resultado", Y);

  waitKey(0);
  return 0;
}
