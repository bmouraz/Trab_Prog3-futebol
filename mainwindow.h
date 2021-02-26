#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

#include "startupwindow.h"
#include "gps.h"
#include "player.h"
#include "field.h"
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);   // construtor da janela principal
    ~MainWindow();                              // destrutor da janela principal


private:
    Ui::MainWindow *ui;                 // objeto que define a interface gráfica
    StartupWindow *startDialog;         // dialog para configuração incial
    QList<Player*> *jogadores;          // lista com os jogadores do arquivo lido
    GPS *gps;                           // objeto gps onde os dados do arquivo serão tratados
    QList<QGeoCoordinate> *coordenadas;  // lista com as 5 coordenadas do campo

    int currentPlayer;  // define o jogador
    int currentHalf;    // define a parte do jogo
    int currentTime;    // define o último instante de tempo analisado

    QCPItemPixmap *bgCampo1; // desenho do campo de deslocamento
    QCPItemPixmap *bgCampo2; // desenho do campo de calor

    // colunas da tabela com os melhores jogadores
    enum Coluna{
        INDEX, DISTANCIA, VELOCIDADE
    };

    void setupData();       // configura o programa ao alterar o arquivo de dados
    void refresh();         // reinicia a interface da janela principal
    void setPlots();        // chama as funções para atualizar os gráficos
    void setOrderTable();   // organiza a tabela de jogadores ordenada
    void animateGraph();    // atualiza automaticamente a posição do jogador ao longo do tempo

private slots:
    void plotDisplacement(int player, int half, int time);  // plota o deslocamento do jogador
    void plotColorMap(int player, int half, int time);      // plota o mapa de calor do jogador
    void on_trackidvalue_currentIndexChanged(int index);    // altera os dados ao mudar o jogador
    void on_tempo1radioButton_released();                   // altera os dados ao selecionar o 1 tempo
    void on_tempo2radioButton_released();                   // altera os dados ao selecionar o 2 tempo
    void on_timeSlider_valueChanged(int value);             // altera os dados ao variar o slider de tempo
    void on_actionArquivo_triggered();                      // abre a janela para alterar o arquivo de dados
    void on_actionSobre_triggered();                        // abre a janela de créditos do software
    void on_escalacheckBox_stateChanged(int arg1);          // mostra ou esconde a grade e escala dos gráficos
    void on_animateButton_clicked();                        // ativa a animação dos gráficos

};

#endif // MAINWINDOW_H
