/*
 * Implemente um exemplo que utilize o detector de bordas Canny em um processamento de vídeo
 *
 * Referencia:
 * - https://docs.opencv.org/3.4.0/da/d5c/tutorial_canny_detector.html
 * - https://docs.opencv.org/3.4.0/d5/dc4/tutorial_video_input_psnr_ssim.html
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const float THRESHOLD = 50;

void abs_diff(Mat &base, Mat &frame, Mat &out) {
	for (int row = 0; row < base.rows; row++) {
		for (int col = 0; col < base.cols; col++) {

			float base_val = base.at<uchar>(row, col);
			float frame_val = frame.at<uchar>(row, col);

			float diff = fabs(base_val - frame_val);

			if (diff > THRESHOLD) {
				if (out.at<uchar>(row, col) < 255)
					out.at<uchar>(row, col) = out.at<uchar>(row, col) + 5;
			}
		}
	}
}

void pos_diff(Mat &base, Mat &frame, Mat &out) {
	for (int row = 0; row < base.rows; row++) {
		for (int col = 0; col < base.cols; col++) {

			float base_val = base.at<uchar>(row, col);
			float frame_val = frame.at<uchar>(row, col);

			float diff = base_val - frame_val;

			if (diff > THRESHOLD) {
				if (out.at<uchar>(row, col) < 255)
					out.at<uchar>(row, col) = out.at<uchar>(row, col) + 5;
			}
		}
	}
}

void neg_diff(Mat &base, Mat &frame, Mat &out) {
	for (int row = 0; row < base.rows; row++) {
		for (int col = 0; col < base.cols; col++) {

			float base_val = base.at<uchar>(row, col);
			float frame_val = frame.at<uchar>(row, col);

			float diff = base_val - frame_val;

			if (diff < -THRESHOLD) {
				if (out.at<uchar>(row, col) < 255)
					out.at<uchar>(row, col) = out.at<uchar>(row, col) + 5;
			}
		}
	}
}

void diff_diff(Mat &base, Mat &frame, Mat &out) {
	for (int row = 0; row < base.rows; row++) {
		for (int col = 0; col < base.cols; col++) {

			float base_val = base.at<uchar>(row, col);
			float frame_val = frame.at<uchar>(row, col);

			float diff = fabs(base_val - frame_val);

			if (diff > THRESHOLD) {
				out.at<uchar>(row, col) = 255;
			} else {
				out.at<uchar>(row, col) = 0;
			}
		}
	}
}

int main(int argc, char** argv) {

	VideoCapture video("2.mp4");

	if (!video.isOpened()) {
		cout << "Nao foi possivel abrir o video" << endl;
		return -1;
	}

	double fps = video.get(CAP_PROP_FPS);
	double delay = (1.0 / fps) * 1000;

	Mat frame;
	Mat base;

	base = Mat::zeros(Size(500, 500), CV_32F);

	// Gera imagem base.
	for (int i = 0; i < 10; i++) {
		video >> frame;

		if (frame.empty()) {
			cout << "Nao foi possivel gerar a imagem base";
			return -1;
		}

		resize(frame, frame, Size(500, 500));
		cvtColor(frame, frame, CV_BGR2GRAY);

		medianBlur(frame, frame, 5);

		frame.convertTo(frame, CV_32F, 1.0 / 255.0);

		add(base, frame / 10.0, base);
	}

	base.convertTo(base, CV_8U, 255);
	imshow("base", base);

	// Calcula e mostra as diferencas.
	Mat gray;
	Mat abs = Mat::zeros(Size(500, 500), CV_8U);
	Mat pos = Mat::zeros(Size(500, 500), CV_8U);
	Mat neg = Mat::zeros(Size(500, 500), CV_8U);
	Mat differece = Mat::zeros(Size(500, 500), CV_8U);

	for (;;) {
		video >> frame;

		if (frame.empty())
			break;

		resize(frame, frame, Size(500, 500));
		cvtColor(frame, gray, COLOR_BGR2GRAY);

		medianBlur(frame, frame, 5);

		abs_diff(base, gray, abs);
		pos_diff(base, gray, pos);
		neg_diff(base, gray, neg);
		diff_diff(base, gray, differece);

		Mat kern = Mat::ones(3, 3, CV_8UC1);
		morphologyEx(differece, differece, MORPH_OPEN, kern);
		morphologyEx(differece, differece, MORPH_CLOSE, kern);

		imshow("Original", frame);
		imshow("Abs Diff", abs);
		imshow("Pos Diff", pos);
		imshow("Neg Diff", neg);
		imshow("Diff", differece);

		char c = (char) waitKey((int) delay);
		if (c == 27)
			break;
	}

	waitKey(0);
	return 0;
}
