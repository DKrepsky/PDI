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

int main(int argc, char** argv) {

	VideoCapture video("../dados/2.mp4");

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
	Mat edges;
	Mat result;
	int lowThreshold = 50;
	int ratio = 2;
	int kernel_size = 3;

	for (;;) {
		video >> frame;

		if (frame.empty())
			break;

		cvtColor(frame, gray, COLOR_BGR2GRAY);
		blur(gray, edges, Size(3, 3));
		Canny(edges, edges, lowThreshold, lowThreshold * ratio, kernel_size);

		imshow("Original", frame);
		imshow("Resultado", edges);

		char c = (char) waitKey((int) delay);
		if (c == 27)
			break;
	}

	waitKey(0);
	return 0;
}
