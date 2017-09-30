//============================================================================
// Name        : eclipse_opencv.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "Informe o caminho da imagem" << std::endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1], 1);

	if (!image.data) {
		std::cout << "Imagem invalida" << std::endl;
		return -1;
	}

	namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", image);

	// Process keypress.
	bool process = true;

	while (process) {
		uint8_t key = static_cast<uint8_t>(waitKey(0));

		switch (key) {
		// Use '+' character to increase gain.
		case 0x2B:
		case 0xAB:
			break;

			// Use '-' character to decrease gain.
		case 0x2D:
		case 0xAD:
			break;

			// Use 'esc' key to exit.
			// Character 0xFF is returned when all windows are closed.
		case 0x1B:
		case 0xFF:
			process = false;
			break;
		}

		return 0;
	}
}
