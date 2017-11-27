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

// Function declarations
void drawAxis(Mat&, Point, Point, Scalar, const float);
double getOrientation(const vector<Point> &, Mat&);

void drawAxis(Mat& img, Point p, Point q, Scalar colour,
    const float scale = 0.2) {
  double angle;
  double hypotenuse;
  angle = atan2((double) p.y - q.y, (double) p.x - q.x);  // angle in radians
  hypotenuse = sqrt(
      (double) (p.y - q.y) * (p.y - q.y) + (p.x - q.x) * (p.x - q.x));
//    double degrees = angle * 180 / CV_PI; // convert radians to degrees (0-180 range)
//    cout << "Degrees: " << abs(degrees - 180) << endl; // angle in 0-360 degrees range
  // Here we lengthen the arrow by a factor of scale
  q.x = (int) (p.x - scale * hypotenuse * cos(angle));
  q.y = (int) (p.y - scale * hypotenuse * sin(angle));
  line(img, p, q, colour, 1, CV_AA);
  // create the arrow hooks
  p.x = (int) (q.x + 9 * cos(angle + CV_PI / 4));
  p.y = (int) (q.y + 9 * sin(angle + CV_PI / 4));
  line(img, p, q, colour, 1, CV_AA);
  p.x = (int) (q.x + 9 * cos(angle - CV_PI / 4));
  p.y = (int) (q.y + 9 * sin(angle - CV_PI / 4));
  line(img, p, q, colour, 1, CV_AA);
}
double getOrientation(const vector<Point> &pts, Mat &img) {
  //Construct a buffer used by the pca analysis
  int sz = static_cast<int>(pts.size());

  Mat data_pts = Mat(sz, 2, CV_64FC1);

  for (int i = 0; i < data_pts.rows; ++i) {
    data_pts.at<double>(i, 0) = pts[i].x;
    data_pts.at<double>(i, 1) = pts[i].y;
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

  // Draw the principal components
  circle(img, cntr, 3, Scalar(255, 0, 255), 2);
  Point p1 = cntr
      + 0.02
          * Point(static_cast<int>(eigen_vecs[0].x * eigen_val[0]),
              static_cast<int>(eigen_vecs[0].y * eigen_val[0]));
  Point p2 = cntr
      - 0.02
          * Point(static_cast<int>(eigen_vecs[1].x * eigen_val[1]),
              static_cast<int>(eigen_vecs[1].y * eigen_val[1]));
  drawAxis(img, cntr, p1, Scalar(0, 255, 0), 1);
  drawAxis(img, cntr, p2, Scalar(255, 255, 0), 5);
  double angle = atan2(eigen_vecs[0].y, eigen_vecs[0].x);  // orientation in radians
  return angle;
}

int main(int argc, char** argv) {
  Mat image;
  image = imread("1.png", IMREAD_COLOR);

  namedWindow("Original", WINDOW_AUTOSIZE);
  imshow("Original", image);

  Mat gray;
  cvtColor(image, gray, COLOR_BGR2GRAY);

  threshold(gray, gray, 50, 255, THRESH_BINARY_INV | THRESH_OTSU);

  Mat kernel = Mat::ones(4, 4, CV_8UC1);

  morphologyEx(gray, gray, MORPH_CLOSE, kernel);

  namedWindow("Seguimentada", WINDOW_AUTOSIZE);
  imshow("Seguimentada", gray);

  vector<Vec4i> hierarchy;
  vector<vector<Point> > contours;
  findContours(gray, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

  for (size_t i = 0; i < contours.size(); ++i) {
    // Calculate the area of each contour
    double area = contourArea(contours[i]);

    // Ignore contours that are too small or too large
    if (area < 1e2 || 1e5 < area)
      continue;

    // Draw each contour only for visualisation purposes
//      drawContours(image, contours, static_cast<int>(i), Scalar(0, 0, 255), 2, 8, hierarchy, 0);

    // Find the orientation of each shape
    getOrientation(contours[i], image);
  }

  namedWindow("Resultado", WINDOW_AUTOSIZE);
  imshow("Resultado", image);

  waitKey(0);
  return 0;
}