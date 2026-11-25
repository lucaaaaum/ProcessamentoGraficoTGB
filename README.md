# ProcessamentoGraficoTGB

Protótipo de um aplicativo de edição de imagens e vídeo inspirado nos *stories* do Instagram, desenvolvido para o Grau B da disciplina Processamento Gráfico: Fundamentos (Unisinos). A aplicação é escrita em C++ com OpenCV para o processamento de imagens e Qt 6 para a interface gráfica.

## Funcionalidades

- **Modo Foto**: importa uma imagem do disco (menu *Arquivo → Importar*) ou congela o frame atual da webcam com o botão *Tirar Foto*.
- **Modo Vídeo**: liga e desliga a webcam com o botão *Ligar/Desligar*, aplicando os filtros em tempo real.
- **Pipeline de filtros**: os filtros são aplicados em sequência, cada um sobre o resultado do anterior. Eles podem ser reordenados ou removidos no painel *Pipeline*.
- **Descrição dos filtros**: ao escolher um filtro, a janela de seleção mostra o que ele faz e quais parâmetros usa.
- **Seleção de canais**: cada filtro pode ser aplicado na imagem inteira ou só nos canais R, G ou B. A caixa *Exibir canal* mostra apenas o canal escolhido.
- **Stickers**: dez stickers PNG com canal alfa. Escolha um no painel *Stickers*, ajuste o tamanho e clique na foto para posicioná-lo. Disponível no Modo Foto.
- **Salvar e Resetar**: *Salvar* grava a imagem resultante em PNG ou JPEG. *Resetar* remove todos os filtros e stickers e volta à imagem original.

### Filtros

| Filtro | Tipo |
| --- | --- |
| Gray Scale | Conversão de cor |
| Blur (Gaussiano) | Suavização |
| Box Blur | Suavização |
| Mediana | Suavização |
| Sharpen | Realce |
| Laplaciano | Realce |
| Canny | Detecção de bordas |
| Negativo | Transformação de intensidade |
| Limiarização | Segmentação |
| Brilho/Contraste | Transformação de intensidade |

### Operações com duas imagens

| Operação | Descrição |
| --- | --- |
| Adição | Soma pixel a pixel com uma segunda imagem |
| Subtração | Diferença pixel a pixel com uma segunda imagem |
| Blending | Mistura ponderada entre as duas imagens |

Ao adicionar uma operação, o programa pede a segunda imagem. Ela é redimensionada para o tamanho da imagem principal.

### Stickers

Os stickers ficam em `assets/stickers/` e são embutidos no executável pelo sistema de recursos do Qt (`assets/assets.qrc`). Para sobrepor um sticker, o canal alfa do PNG vira uma máscara: os pixels com alfa acima de 127 são copiados por cima da imagem e os demais são ignorados, deixando a foto aparecer. Os stickers são desenhados depois da pipeline de filtros, então não são afetados por ela.

## Compilação

### Com Nix

O repositório tem um `flake.nix` com todas as dependências:

```sh
nix develop
cmake -B build -G Ninja
cmake --build build
./build/tgb
```

### Sem Nix

Dependências: CMake 3.16+, compilador com suporte a C++20, Qt 6 (módulos Widgets e OpenGLWidgets) e OpenCV 4.

```sh
cmake -B build
cmake --build build
./build/tgb
```

## Estrutura

```
assets/stickers/     imagens PNG dos stickers
src/app/             Controller: liga a fonte de imagem, a pipeline e os stickers
src/capturing/       fontes de imagem (webcam e imagem estática)
src/processing/      filtros, pipeline e sobreposição de stickers
src/ui/              janela principal, canvas e painéis laterais
```

## Créditos

Os stickers são emojis do [OpenMoji](https://openmoji.org/), o projeto de emojis open source da HfG Schwäbisch Gmünd, licenciados sob [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/).
