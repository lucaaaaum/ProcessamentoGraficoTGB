#pragma once
#include <functional>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

class Filter {
private:
  std::string name;
  std::string description;
  std::function<void(cv::Mat &)> filterFunction;

public:
  Filter(std::string name, std::string description,
         std::function<void(cv::Mat &)> func)
      : name(name), description(description), filterFunction(func) {}

  void apply(cv::Mat &image);

  std::string getName() const { return name; }

  std::string getDescription() const { return description; }

  static std::vector<Filter> presets() {
    return {grayScaleFilter(), blurFilter(),      boxBlurFilter(),
            medianFilter(),    sharpenFilter(),   laplacianFilter(),
            cannyFilter(),     negativeFilter(),  thresholdFilter(),
            brightnessContrastFilter()};
  }

  static Filter grayScaleFilter() {
    return Filter("Gray Scale",
                  "Converte a imagem para tons de cinza, combinando os canais "
                  "B, G e R pela luminância de cada pixel. Sem parâmetros.",
                  [](cv::Mat &image) {
                    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
                    cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
                  });
  }

  static Filter blurFilter() {
    return Filter("Blur",
                  "Suavização gaussiana: cada pixel vira uma média ponderada "
                  "da vizinhança, com pesos maiores perto do centro. "
                  "Parâmetros: tamanho do kernel (5x5) e desvio padrão "
                  "(calculado a partir do kernel).",
                  [](cv::Mat &image) {
                    cv::GaussianBlur(image, image, cv::Size(5, 5), 0);
                  });
  }

  static Filter boxBlurFilter() {
    return Filter("Box Blur",
                  "Suavização por média simples: cada pixel vira a média "
                  "aritmética da vizinhança, com todos os pesos iguais. "
                  "Parâmetro: tamanho do kernel (5x5).",
                  [](cv::Mat &image) {
                    cv::blur(image, image, cv::Size(5, 5));
                  });
  }

  static Filter medianFilter() {
    return Filter("Mediana",
                  "Suavização que substitui cada pixel pela mediana da "
                  "vizinhança. Remove ruído sal e pimenta preservando bordas. "
                  "Parâmetro: tamanho do kernel (5, sempre ímpar).",
                  [](cv::Mat &image) { cv::medianBlur(image, image, 5); });
  }

  static Filter sharpenFilter() {
    return Filter("Sharpen",
                  "Realce que reforça as diferenças entre um pixel e seus "
                  "vizinhos, deixando contornos e detalhes mais nítidos. "
                  "Parâmetro: kernel 3x3 com 5 no centro e -1 nos vizinhos "
                  "diretos.",
                  [](cv::Mat &image) {
                    cv::Mat kernel = (cv::Mat_<float>(3, 3) << 0, -1, 0, -1, 5,
                                      -1, 0, -1, 0);
                    cv::filter2D(image, image, -1, kernel);
                  });
  }

  static Filter laplacianFilter() {
    return Filter("Laplaciano",
                  "Realce de bordas pela segunda derivada da intensidade: "
                  "regiões uniformes ficam escuras e transições bruscas ficam "
                  "claras. Opera em tons de cinza. Parâmetro: tamanho da "
                  "abertura (3).",
                  [](cv::Mat &image) {
                    cv::Mat gray, laplacian;
                    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
                    cv::Laplacian(gray, laplacian, CV_16S, 3);
                    cv::convertScaleAbs(laplacian, gray);
                    cv::cvtColor(gray, image, cv::COLOR_GRAY2BGR);
                  });
  }

  static Filter cannyFilter() {
    return Filter("Canny",
                  "Detector de bordas de Canny: suaviza, calcula o gradiente "
                  "e mantém só as bordas finas e contínuas, em branco sobre "
                  "preto. Parâmetros: limiar inferior (100) e superior (200).",
                  [](cv::Mat &image) {
                    cv::Mat gray, edges;
                    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
                    cv::Canny(gray, edges, 100, 200);
                    cv::cvtColor(edges, image, cv::COLOR_GRAY2BGR);
                  });
  }

  static Filter negativeFilter() {
    return Filter("Negativo",
                  "Inverte as cores: cada canal passa a valer 255 menos o "
                  "valor original. Sem parâmetros.",
                  [](cv::Mat &image) { cv::bitwise_not(image, image); });
  }

  static Filter thresholdFilter() {
    return Filter("Limiarização",
                  "Converte para tons de cinza e deixa cada pixel preto ou "
                  "branco, conforme esteja abaixo ou acima do limiar. "
                  "Parâmetros: limiar (127) e valor máximo (255).",
                  [](cv::Mat &image) {
                    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
                    cv::threshold(image, image, 127, 255, cv::THRESH_BINARY);
                    cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
                  });
  }

  static Filter brightnessContrastFilter() {
    return Filter("Brilho/Contraste",
                  "Ajuste linear de cada pixel pela fórmula alfa * pixel + "
                  "beta. Alfa controla o contraste e beta o brilho. "
                  "Parâmetros: alfa (1.3) e beta (20).",
                  [](cv::Mat &image) { image.convertTo(image, -1, 1.3, 20); });
  }
};
