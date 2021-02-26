#ifndef FIELD_H
#define FIELD_H

#include <QWidget>
#include <QGeoCoordinate>
#include <cmath>
#include "mainwindow.h"

class Field
{
public:
    Field(); // construtor padrão do campo
    Field(QList<QGeoCoordinate> *coord, Player *jogador,int parte, int _n, int _m, int time); // construtor do campo para um jogador em determinada parte da atividade
    ~Field(); // destrutor do campo

    QVector<double> X(); // retorna o vetor de valores de X que foram ocupados num período tDomain
    QVector<double> Y(); // retorna o vetor de valores de Y que foram ocupados num período tDomain

    double **getMatriz(); // retorna a matriz com os pontos onde o jogador passou
private:
    // coordenadas do campo do JEC, retiradas do Google Maps
    // referência norte para cima
    QGeoCoordinate A; // vértice inferior esquerdo
    QGeoCoordinate B; // vértice inferior direito
    QGeoCoordinate C; // vértice superior direito
    QGeoCoordinate D; // vértice superior esquerdo
    QGeoCoordinate O; // centro do campo
    QGeoCoordinate E; // posição inicial do jogador

    // vetores com as posições cartesianas do jogador
    QVector<double> x;
    QVector<double> y;

    int n; // número horizontal de pixels para o gráfico
    int m; // número vertical de pixels para o gráfico
    double xDomain; // tamanho do campo em metros na direção x
    double yDomain; // tamanho do campo em metros na direção y
    int tDomain;  // limite de tempo para geração do campo
    double **matriz; // matriz para o gráfico do mapa de calor

    void setField(QList<QGeoCoordinate> *coord); // define os vértices do campo de futebol
    void setXY(Player *jogador,int parte); // define as coordenadas cartesianas onde o jogador se posicionou
    void geraMatriz(); // cria a matriz que irá preencher o mapa de calor
    void deletaMatriz(); // deleta toda a matriz do mapa de calor
};

#endif // FIELD_H
