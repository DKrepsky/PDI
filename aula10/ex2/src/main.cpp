/*
 * Implemente um exemplo que utilize o OTSU para binarizar as imagens contidas em um vídeo.
 *
 * Referencia:
 * - https://docs.opencv.org/3.4.0/d5/dc4/tutorial_video_input_psnr_ssim.html
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	VideoCapture video("../dados/1.mp4");

	if (!video.isOpened()) {
		cerr << "ERROR! Unable to open video\n";
		return -1;
	}

	double fps = video.get(CAP_PROP_FPS);
	double delay = (1.0 / fps) * 1000;

	namedWindow("Original", WINDOW_AUTOSIZE);
	namedWindow("Resultado", WINDOW_AUTOSIZE);

	Mat frame;
	Mat gray;
	Mat result;

	for (;;) {

		video >> frame;

		if (frame.empty())
			break;

		cvtColor(frame, gray, COLOR_BGR2GRAY);
		threshold(gray, result, 0, 255, THRESH_BINARY |THRESH_OTSU);

		imshow("Original", frame);
		imshow("Resultado", result);

		char c = (char) waitKey((int) delay);
		if (c == 27)
			break;
	}

	waitKey(0);
	return 0;
}
