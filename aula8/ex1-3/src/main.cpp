/*
 * Referências:
 * https://docs.opencv.org/2.4/modules/core/doc/drawing_functions.html
 */
#include <iostream>
#include <random>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

static std::random_device rdev { };
static default_random_engine engine { rdev() };

void uniforme(Mat& img, int a, int b, float p) {

  uniform_int_distribution<int> i_range { a, b };
  uniform_real_distribution<float> f_range { 0, 1 };

  uchar* pdata;

  for (pdata = img.data; pdata < (img.rows * img.cols) + img.data; pdata++) {
    float noise_prob = f_range(engine);

    if (noise_prob < p) {
      int r = i_range(engine);

      *pdata = saturate_cast<uchar>(*pdata + r);
    }
  }
}

void impulsivo(Mat& img, int a, int b, float pa, float pb) {

  uniform_real_distribution<float> range { 0, 1 };

  float p = pa + pb;
  float is_pa = pa / (pa + pb);

  uchar* pdata;
  for (pdata = img.data; pdata < (img.rows * img.cols) + img.data; pdata++) {
    float noise_prob = range(engine);

    if (noise_prob < p) {

      float a_prob = range(engine);

      if (a_prob < is_pa) {
        *pdata = saturate_cast<uchar>(*pdata + a);
      } else {
        *pdata = saturate_cast<uchar>(*pdata + b);
      }
    }
  }
}

void add_psnr(Mat& original, Mat& noisy) {
  double psnr = PSNR(original, noisy);

  string text = string(to_string(psnr));

  int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
  double fontScale = 1;
  int thickness = 2;
  int baseline = 0;

  Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);
  baseline += thickness;

  // Center text.
  int h = (noisy.cols - textSize.width) / 2;
  int v = (int) ((float) (noisy.rows + textSize.height) * 0.8);
  Point textOrg(h, v);

  // Draw text.
  putText(noisy, text, textOrg, fontFace, fontScale, Scalar::all(0), thickness,
      8);
}

int main(int argc, char** argv) {
  Mat image;
  image = imread("lenna.png", IMREAD_GRAYSCALE);

  // Exercício 1-a: Ruído aleatório: p = 0,95; distribuição uniforme com valores
  // entre -30 e 30.
  Mat e1a = image.clone();
  uniforme(e1a, -30, 30, 0.95);

  // Exercício 1-b: Ruído uniforme: a = 85; b = 100; p = 1/(b - a).
  Mat e1b = image.clone();
  uniforme(e1b, 85, 100, 1.0 / ((float) 100 - 85));

  // Exercício 1-c: Ruído impulsivo (“sal e pimenta”): a = 0; b = 255; Pa = Pb
  // = 0,1; (Pa + Pb = 0,2).
  Mat e1c = image.clone();
  impulsivo(e1c, -255, 255, 0.1, 0.1);

  // Exercício 2: Faça a filtragem das imagens com ruído do exercício 1
  // utilizando o filtro de mediana e bilateral. Apresente as imagens filtradas.
  Mat e2am;
  medianBlur(e1a, e2am, 5);

  Mat e2ab;
  bilateralFilter(e1a, e2ab, 5, 100, 100);

  Mat e2bm;
  medianBlur(e1b, e2bm, 5);

  Mat e2bb;
  bilateralFilter(e1b, e2bb, 5, 100, 100);

  Mat e2cm;
  medianBlur(e1c, e2cm, 5);

  Mat e2cb;
  bilateralFilter(e1c, e2cb, 5, 100, 100);

  // Exercício 3: Verifique o valor da métrica PSNR para as imagens do exercício
  // 1 e 2. Considere a imagem original como o parâmetro de referência da
  // métrica. Mostre o valor resultante para cada caso, observando a melhora da
  // pontuação após a filtragem.

  add_psnr(image, e1a);
  add_psnr(image, e2am);
  add_psnr(image, e2ab);

  add_psnr(image, e1b);
  add_psnr(image, e2bm);
  add_psnr(image, e2bb);

  add_psnr(image, e1c);
  add_psnr(image, e2cm);
  add_psnr(image, e2cb);

  // Mostra o resultado
  Mat resa;
  vector<Mat> mveca = { image, e1a, e2am, e2ab };
  hconcat(mveca, resa);

  namedWindow("Imagem A", WINDOW_AUTOSIZE);
  imshow("Imagem A", resa);

  Mat resb;
  vector<Mat> mvecb = { image, e1b, e2bm, e2bb };
  hconcat(mvecb, resb);

  namedWindow("Imagem B", WINDOW_AUTOSIZE);
  imshow("Imagem B", resb);

  Mat resc;
  vector<Mat> mvecc = { image, e1c, e2cm, e2cb };
  hconcat(mvecc, resc);

  namedWindow("Imagem C", WINDOW_AUTOSIZE);
  imshow("Imagem C", resc);

  waitKey(0);
  return 0;
}
