#include "half.h"

Half::Half(QObject *parent) : QObject(parent){ }

Half::Half(QList<QString> strLocalDate,
           QList<QString> strLocalTime,
           QList<QString> strLatitude,
           QList<QString> strNS,
           QList<QString> strLongitude,
           QList<QString> strEW,
           QList<QString> strSpeed){

    tam = int(strLocalTime.size());

    setDate(strLocalDate);
    setTime(strLocalTime);
    setCoord(strLatitude,
             strLongitude,
             strNS,
             strEW);
    setSpeed(strSpeed);
    setDistanciaEntrePontos();
}

Half::~Half(){
    delete localDate;
    delete localTime;
    delete coordenadas;
    delete speed;
    delete distanciaEntrePontos;
}

// converte QList<QString> para QList<QDate>
void Half::setDate(QList<QString> strLocalDate){
    localDate = new QList<QDate>;
    for(int i=0; i<tam;i++){
        localDate->append(QDate::fromString(strLocalDate[i],"yyyy/M/d"));

    }
}

// converte QList<QString> para QList<QTime>
void Half::setTime(QList<QString> strLocalTime){
    localTime = new QList<QTime>;
    for(int i=0;i<tam;i++){
        localTime->append(QTime::fromString(strLocalTime[i],"h:m:s"));
    }
}

// converte QList<QString> para QList<QGeoCoordinate>
void Half::setCoord(QList<QString> strLatitude,
                      QList<QString> strLongitude,
                      QList<QString> strNS,
                      QList<QString> strEW){

    coordenadas = new QList<QGeoCoordinate>;

    for(int i=0;i<tam;i++){
        // os dados de coordenadas da tabela são valores double positivos
        double lat = strLatitude[i].toDouble();
        double lon = strLongitude[i].toDouble();
        // caso as coordenadas estejam nos hemisférios Sul e Oeste
        // é necessário tornar os valores negativos
        if(strNS[i]=='S') lat*=-1;
        if(strEW[i]=='W') lon*=-1;
        QGeoCoordinate temp(lat,lon);
        coordenadas->append(temp);
    }
}

// converte os valores de velocidade para QList<double>
void Half::setSpeed(QList<QString> strSpeed){
    speed = new QList<double>;
    for(int i=0;i<tam;i++){
        speed->append(strSpeed[i].toDouble());
    }
}

// calcula as distancias percorridas em  cada fração de tempo
void Half::setDistanciaEntrePontos(){
    distanciaEntrePontos = new QList<double>;

    for(int i=0;i<tam-1;i++){
        QGeoCoordinate coord1 = getCoordenadas().at(i);
        QGeoCoordinate coord2 = getCoordenadas().at(i+1);
        distanciaEntrePontos->append(coord1.distanceTo(coord2));
    }
}

QList<QDate> Half::getDate(){
    return *localDate;
}

QList<QTime> Half::getTime(){
    return *localTime;
}

QList<QGeoCoordinate> Half::getCoordenadas(){
    return *coordenadas;
}

QList<double> Half::getSpeed(){
    return *speed;
}


QList<double> Half::getDistanciaEntrePontos(){
    return *distanciaEntrePontos;
}

int Half::size(){
    return tam-1;
}
