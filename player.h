#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QGeoCoordinate>
#include <QDate>
#include <QTime>

#include "half.h"

class Player : public QObject
{
    Q_OBJECT
public:
    // construtor padrão da classe Player
    explicit Player(QObject *parent = nullptr);

    // construtor da classe Player que recebe os dados de cada atividade
    Player(QList<Half *> *atividades);

    ~Player(); // destrutor da classe Player

    // funções get para retornar os dados estatísticos
    QTime getTempo1();
    QTime getTempo2();
    QTime getTempoEfetivo();
    int getCorridas18kmh();
    int getCorridas20kmh();
    QTime getTempo18();
    QTime getTempo20();
    double getDistanciaPercorrida();
    double getVelocidadeMax();

    Half* getParte(int i); // retrona a parte (1 ou 2) em que o jogador participa

private:
    QList<Half*> *parte;                // armazena os dados das duas atividades realizadas
    QTime *tempo1;                      // tempo efetivo da primeira atividade
    QTime *tempo2;                      // tempo efetivo da segunda atividade
    QTime *tempoEfetivo;                // tempo efetivo total
    int *corridas18kmh;                 // numero de vezes que o jogador correu acima de 18km/h
    int *corridas20kmh;                 // numero de vezes que o jogador correu acima de 20km/h
    QTime *periodo18;                   // período total em que o jogador correu acima de 18 km/h
    QTime *periodo20;                   // período total em que o jogador correu acima de 20 km/h
    double *distanciaPercorrida;        // distancia percorrida total pelo jogador
    double *velocidadeMax;              // velocidade máxima atingida pelo jogador

    void setPartes(QList<Half *> *atv); // define as partes da atividade que o jogador participou
    void setTempos();                   // define os tempos de duração de cada atividade e total
    void setCorridas();                 // define o número de corridas maior que 18km/h, 20km/h e a velocidade máxima de cada jogador
    void setDistancia();                // calcula a distância percorrida pelo jogador durante a atividade

signals:

public slots:

};

#endif // PLAYER_H
