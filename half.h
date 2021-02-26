#ifndef HALF_H
#define HALF_H

#include <QObject>
#include <QGeoCoordinate>
#include <QDate>
#include <QList>
#include <cmath>

class Half : public QObject
{
    Q_OBJECT
public:
    // construtor padrão da classe Half
    explicit Half(QObject *parent = nullptr);

    // construtor da classe Half
    // recebe como parâmetro listas de strings com os dados de cada atividade
    Half(QList<QString> strLocalDate,
         QList<QString> strLocalTime,
         QList<QString> strLatitude,
         QList<QString> strNS,
         QList<QString> strLongitude,
         QList<QString> strEW,
         QList<QString> strSpeed);

    ~Half(); // destrutor da classe Half

    // funções para retornar listas com valores convertidos
    QList<QDate> getDate();
    QList<QTime> getTime();
    QList<QGeoCoordinate> getCoordenadas();
    QList<double> getSpeed();
    QList<double> getDistanciaEntrePontos();

    int size(); // retorna o número de amostras usadas na atividade

private:
    int tam; // número de amostras usados na atividade

    QList<QDate> *localDate;                // lista com as datas de cada amostra
    QList<QTime> *localTime;                // lista com as horas de cada amostra
    QList<QGeoCoordinate> *coordenadas;     // lista com as coordenadas geográficas percorriads pelo jogador na atividade
    QList<double> *speed;                   // lista com as valocidades do jogador durante a atividade
    QList<double> *distanciaEntrePontos;    // lista de double com as distâncias percorridas em cada amostra

    // funções que distribuem os dados das string
    void setDate(QList<QString> strLocalDate);
    void setTime(QList<QString> strLocalTime);
    void setCoord(QList<QString> strLatitude,
                  QList<QString> strLongitude,
                  QList<QString> strNS,
                  QList<QString> strEW);
    void setSpeed(QList<QString> strSpeed);
    void setDistanciaEntrePontos(); // calcula a distância percorrida em cada fração de tempo

signals:

public slots:
};

#endif // HALF_H
