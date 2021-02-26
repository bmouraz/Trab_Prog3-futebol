# GPSSoccer View

Um software capaz de interpretar dados de GPS para uso esportivo profissional em arenas ao ar livre. O código compõe o projeto final da disciplina de Programação III na Universidade Federal de Santa Catarina - Centro Tecnológico de Joinville.

## Começando

### Prerequisitos

Os seguintes repositórios são necessários:

```
sudo apt install qtbase5-dev
sudo apt install qtdeclarative5-dev
sudo apt install qtmultimedia5-dev
sudo apt install libqt5charts5-dev
sudo apt install qml-module-qtcharts    
sudo apt install qtpositioning5-dev ope
```

### Instalando

Na pasta onde os arquivos .pro foram extraídos, executar:

```
qmake
make
./TrabalhoFinal
``` 

## Como utilizar:

Carregue um arquivo .csv válido com os dados do gps do jogador.
ATENÇÃO: insira os horários de inicio e fim da partida com coerência, ou o programa se fechará.

## Construído com:

* [QMake 3.0](https://doc.qt.io/archives/3.3/qmake-manual-2.html) - Compilador
* [Qt5](http://doc.qt.io/qt-5/linux.html) - Framework para interface gráfica
* [QCustomPlot](http://www.qcustomplot.com/) - Gráficos

### Autores

* **Bruno Moura**
* **Mairon S. Wolniewicz**
