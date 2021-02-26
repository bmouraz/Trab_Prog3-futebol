#ifndef GPS_H
#define GPS_H

#include <QObject>
#include <QStandardItemModel>

#include "player.h"

class GPS : public QObject
{
    Q_OBJECT

public:
    explicit GPS(QObject *parent = nullptr); // construtor padrão da classe GPS
    // construtor da classe GPS
    GPS(QList<Player*> *jogadores,
        QFile *arquivo,
        QTime ti1,
        QTime tf1,
        QTime ti2,
        QTime tf2);
    ~GPS(); // destrutor da classe GPS 

private:
    QStandardItemModel *csvDados; // tabela com os dados do arquivo csv

    Half *pt1;
    Half *pt2;

    // dados de tempo, poderão ser alterados na interface OCORRERÃO MUDANÇAS AQUI
    QTime tempoInicial1;
    QTime tempoFinal1;
    QTime tempoInicial2;
    QTime tempoFinal2;

    // strings com dados retirados do arquivo csv
    QList<QString> strLocalDate;
    QList<QString> strLocalTime;
    QList<QString> strLatitude;
    QList<QString> strNS;
    QList<QString> strLongitude;
    QList<QString> strEW;
    QList<QString> strSpeed;

    void leDados(QFile *arquivo); // le os dados dos arquivos e organiza em uma tabela QStandardItemModel

    void verificaTempos(QList<Player*> *jogadores); // com base nos tempos inicial e final de cada parte da atividade, separar os dados para cada jogador
    int getTrackID(int i); // retorona o valor do Track ID para reconhecimento do jogador
    void setStrings(int start, int end, bool trocaPlayer); // converte os valores QStandardItemModel para QList<QString>
};

#endif // GPS_H
