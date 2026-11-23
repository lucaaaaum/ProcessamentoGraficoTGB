#pragma once
#include "channel.h"
#include <functional>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

class Filter {
private:
  std::string name;
  std::string description;
  std::function<void(cv::Mat &)> filterFunction;
  std::function<void(cv::Mat &, const cv::Mat &)> operation;
  cv::Mat operand;
  std::string operandName;
  Channel channels = Channel::All;
  bool channelSelectable = true;

  void run(cv::Mat &image);

public:
  Filter(std::string name, std::string description,
         std::function<void(cv::Mat &)> func)
      : name(name), description(description), filterFunction(func) {}

  static Filter
  operationFilter(std::string name, std::string description,
                  std::function<void(cv::Mat &, const cv::Mat &)> op) {
    Filter filter(name, description, nullptr);
    filter.operation = op;
    return filter;
  }

  void apply(cv::Mat &image);

  std::string getName() const { return name; }

  std::string getDescription() const { return description; }

  Channel getChannels() const { return channels; }

  void setChannels(Channel value) {
    if (channelSelectable) {
      channels = value;
    }
  }

  bool supportsChannelSelection() const { return channelSelectable; }

  bool requiresOperand() const { return static_cast<bool>(operation); }

  std::string getOperandName() const { return operandName; }

  void setOperand(const cv::Mat &image, const std::string &imageName) {
    operand = image;
    operandName = imageName;
  }

  static std::vector<Filter> presets() {
    return {grayScaleFilter(),   blurFilter(),
            boxBlurFilter(),     medianFilter(),
            sharpenFilter(),     laplacianFilter(),
            cannyFilter(),       negativeFilter(),
            thresholdFilter(),   brightnessContrastFilter(),
            additionOperation(), subtractionOperation(),
            blendingOperation()};
  }

  static Filter additionOperation() {
    return operationFilter(
        "Adição",
        "Soma pixel a pixel a imagem atual com uma segunda imagem escolhida "
        "do disco. Valores acima de 255 ficam em 255, então o resultado "
        "tende a clarear. Parâmetro: segunda imagem, redimensionada para o "
        "tamanho da atual.",
        [](cv::Mat &image, const cv::Mat &other) {
          cv::add(image, other, image);
        });
  }

  static Filter subtractionOperation() {
    return operationFilter(
        "Subtração",
        "Subtrai pixel a pixel uma segunda imagem escolhida do disco da "
        "imagem atual. Valores abaixo de 0 ficam em 0, então só sobram as "
        "regiões mais claras que a segunda imagem. Parâmetro: segunda "
        "imagem, redimensionada para o tamanho da atual.",
        [](cv::Mat &image, const cv::Mat &other) {
          cv::subtract(image, other, image);
        });
  }

  static Filter blendingOperation() {
    return operationFilter(
        "Blending",
        "Mistura a imagem atual com uma segunda imagem escolhida do disco "
        "pela fórmula alfa * atual + beta * segunda. Parâmetros: segunda "
        "imagem, redimensionada para o tamanho da atual, alfa (0.5) e beta "
        "(0.5).",
        [](cv::Mat &image, const cv::Mat &other) {
          cv::addWeighted(image, 0.5, other, 0.5, 0, image);
        });
  }

  static Filter grayScaleFilter() {
    Filter filter("Gray Scale",
                  "Converte a imagem para tons de cinza, combinando os canais "
                  "B, G e R pela luminância de cada pixel. Atua sempre nos "
                  "três canais. Sem parâmetros.",
                  [](cv::Mat &image) {
                    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
                    cv::cvtColor(image, image, cv::COLOR_GRAY2BGR);
                  });
    filter.channelSelectable = false;
    return filter;
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
