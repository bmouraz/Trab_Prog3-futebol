#include "player.h"

// construtor padrão da classe Player
Player::Player(QObject *parent) : QObject(parent)
{    }

// construtor da classe Player
// recebe como parâmetro listas de strings usadas para preencher seus dados
Player::Player(QList<Half*> *parte){
    setPartes(parte);
    setTempos();
    setDistancia();
    setCorridas();
}

// destrutor da classe Player
Player::~Player(){
    delete tempo1;
    delete tempo2;
    delete tempoEfetivo;
    delete corridas18kmh;
    delete corridas20kmh;
    delete periodo18;
    delete periodo20;
    delete distanciaPercorrida;
    delete velocidadeMax;
    for(int i=0;i<2;i++){
        delete parte->at(i);
    }
    delete parte;
}

// calcula a distância percorrida pelo jogador durante a atividade
void Player::setDistancia(){
    distanciaPercorrida = new double(0);
    for(int j=0;j<2;j++){
        int tam = parte->at(j)->size();
        for(int i=0;i<tam-1;i++){
            *distanciaPercorrida+= parte->at(j)->getDistanciaEntrePontos().at(i);
        }
    }
}

// define o número de corridas maior que 18km/h, 20km/h e a velocidade máxima de cada jogador
void Player::setCorridas(){
    corridas18kmh = new int(0);
    corridas20kmh = new int(0);
    velocidadeMax = new double(0);

    periodo18 = new QTime(0,0,0);
    periodo20 = new QTime(0,0,0);

    int s; // segundos a serem adicionados

    double speed;
    for(int j=0;j<2;j++){
        int tam = parte->at(j)->size();
        for(int i=0;i<tam;i++){
            speed = parte->at(j)->getSpeed().at(i);
            if(speed > 18){
                *corridas18kmh+=1;
                if(i<tam){
                    s = parte->at(j)->getTime().at(i).secsTo(parte->at(j)->getTime().at(i+1));
                    *periodo18 = periodo18->addSecs(s);
                }
            }
            if(speed > 20){
                *corridas20kmh+=1;
                if(i<tam){
                    s = parte->at(j)->getTime().at(i).secsTo(parte->at(j)->getTime().at(i+1));
                    *periodo20 = periodo20->addSecs(s);
                }
            }
            if(speed > *velocidadeMax) *velocidadeMax = speed;
        }
    }
}

// define o período de cada atividade
void Player::setTempos(){
    tempo1 = new QTime(0,0,0);
    tempo2 = new QTime(0,0,0);
    tempoEfetivo = new QTime(0,0,0);

    int milisec1,milisec2;
    QTime ti1,tf1,ti2,tf2;

    ti1 = parte->at(0)->getTime().front();  // início da primeira atividade
    tf1 = parte->at(0)->getTime().back();   // final da segunda atividade
    milisec1 = ti1.msecsTo(tf1);            // tempo em milisegundos entre o início e o fim da atividade

    *tempo1 = tempo1->addMSecs(milisec1);   // adiciona milisegundos1 ao tempo1

    ti2 = parte->at(1)->getTime().front();  // início da segunda atividade
    tf2 = parte->at(1)->getTime().back();   // final da segunda atividade
    milisec2 = ti2.msecsTo(tf2);            // tempo em milisegundos entre o início e o fim da atividade

    *tempo2 = tempo2->addMSecs(milisec2);   // adiciona milisegundos2 ao tempo2

    *tempoEfetivo = tempoEfetivo->addMSecs(milisec1+milisec2); // adiciona milisegundos 1 e 2 ao tempoEfetivo
}

Half* Player::getParte(int i){
    return parte->at(i);
}

QTime Player::getTempo1(){
    return *tempo1;
}

QTime Player::getTempo2(){
    return *tempo2;
}

QTime Player::getTempoEfetivo(){
    return *tempoEfetivo;
}

int Player::getCorridas18kmh(){
    return *corridas18kmh;
}

int Player::getCorridas20kmh(){
    return *corridas20kmh;
}

double Player::getDistanciaPercorrida(){
    return *distanciaPercorrida;
}

double Player::getVelocidadeMax(){
    return *velocidadeMax;
}

QTime Player::getTempo18(){
    return *periodo18;
}

QTime Player::getTempo20(){
    return *periodo20;
}

void Player::setPartes(QList<Half*> *atv){
    parte = new QList<Half*>;
    parte->append(atv->at(0));
    parte->append(atv->at(1));
}
