/*
 * Apresente uma segmentação de imagens coloridas utilizando o algoritmo k-means
 * com 3, 4, 5 e 6 clusters.
 *
 * Referencia:
 * - Ex. 2 aula 3.
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Scalar Colors[] = { Scalar(255, 255, 255), Scalar(255, 00, 0), Scalar(0, 255,
		0), Scalar(0, 0, 255), Scalar(255, 255, 0), Scalar(0, 255, 255) };

void apply_kmeans(Mat &img, Mat &res, int k) {
	// Transforma a imagem em um formato adequado.
	Mat buffer;
	buffer.create(img.rows * img.cols, 3, CV_32F);

	int index = 0;
	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {
			Vec3b val = img.at<Vec3b>(row, col);
			buffer.at<float>(index, 0) = static_cast<float>(val[0]) / 255.0;
			buffer.at<float>(index, 1) = static_cast<float>(val[1]) / 255.0;
			buffer.at<float>(index, 2) = static_cast<float>(val[2]) / 255.0;
			index++;
		}
	}

	TermCriteria criteria(TermCriteria::EPS + TermCriteria::COUNT, 10000,
			0.0001);

	Mat labels, centers;
	kmeans(buffer, k, labels, criteria, 10, KMEANS_RANDOM_CENTERS, centers);

	res.create(img.rows, img.cols, CV_8UC3);

	index = 0;
	for (int row = 0; row < img.rows; row++) {
		for (int col = 0; col < img.cols; col++) {

			int label = labels.at<int>(index);

			Vec3b val;
//			val[0] = centers.at<float>(label, 0) * 255.0;
//			val[1] = centers.at<float>(label, 1) * 255.0;
//			val[2] = centers.at<float>(label, 2) * 255.0;

			val[0] = Colors[label][0];
			val[1] = Colors[label][1];
			val[2] = Colors[label][2];

			res.at<Vec3b>(row, col) = val;
			index++;
		}
	}
}

int main(int argc, char** argv) {
	// Carrega a imagem original.
	Mat img;
	img = imread("../dados/1.jpg", IMREAD_COLOR);

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", img);

	Mat k3, k4, k5, k6;
	apply_kmeans(img, k3, 3);
	apply_kmeans(img, k4, 4);
	apply_kmeans(img, k5, 5);
	apply_kmeans(img, k6, 6);

	namedWindow("K = 3", WINDOW_AUTOSIZE);
	imshow("K = 3", k3);

	namedWindow("K = 4", WINDOW_AUTOSIZE);
	imshow("K = 4", k4);

	namedWindow("K = 5", WINDOW_AUTOSIZE);
	imshow("K = 5", k5);

	namedWindow("K = 6", WINDOW_AUTOSIZE);
	imshow("K = 6", k6);
	waitKey(0);
	return 0;
}
