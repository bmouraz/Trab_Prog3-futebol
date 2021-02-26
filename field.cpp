#include "field.h"

// construtor padrão do campo
Field::Field() : matriz(nullptr) {}

// construtor do campo para um jogador em determinada parte da atividade
Field::Field(QList<QGeoCoordinate> *coord, Player *jogador, int parte, int _n, int _m, int _tDomain){
    tDomain = _tDomain;
    n = _n;
    m = _m;
    setField(coord);
    setXY(jogador,parte);
    geraMatriz();
}

// destrutor do campo
Field::~Field(){
    deletaMatriz();
}

// define os vértices do campo de futebol e o domínio das direções X e Y
// em um caso genérico, essas variáveis poderiam ser alteradas para qualquer campo
// neste caso usamos apenas as coordenadas do campo do JEC, retiradas do Google Maps
void Field::setField(QList<QGeoCoordinate> *coord){
    A = coord->at(0); // vértice inferior esquerdo
    B = coord->at(1); // vértice inferior direito
    C = coord->at(2); // vértice superior direito
    D = coord->at(3); // vértice superior esquerdo
    O = coord->at(4); // centro do campo

    xDomain = 80;
    yDomain = 120;
}

// define as coordenadas cartesianas onde o jogador se posicionou
// o centro do campo é escolhido como a origem do gráfico
void Field::setXY(Player *jogador, int parte){
    int tam = jogador->getParte(parte)->size();

    double theta;

    double AB = A.distanceTo(B);
    double BC = B.distanceTo(C);

    QPointF AOr(AB*0.5,BC*0.5); // posição relativa da origem em relação ao vértice inferior esquerdo do campo

    // vetores com as posições X e Y dentro de campo
    x = QVector<double>(tam);
    y = QVector<double>(tam);

    for(int i=0;i<tam;i++){
        // coordenada E define a posição do jogador no campo
        E = jogador->getParte(parte)->getCoordenadas().at(i);

        double AE = A.distanceTo(E); // distância em metros entre os pontos A e E
        double BE = B.distanceTo(E); // distância em metros entre os pontos B e E

        // lei dos cossenos é aplicada para encontrar o ângulo entre o eixo horizontal e o vetor AE
        theta = acos((pow(AE,2)+pow(AB,2)-pow(BE,2))/(2.0*AE*AB));

        QPointF AEr(AE*cos(theta),AE*sin(theta)); // definine o vetor AE

        // relação entre os vetores AE e AO resulta na posição OE (X e Y relativos ao centro do campo)
        x[i] = AEr.x() - AOr.x();
        y[i] = AEr.y() - AOr.y();
    }
}

// cria a matriz que irá preencher o mapa de calor
void Field::geraMatriz(){

    // aloca a matriz e preenche com zeros
    // +2 para evitar problemas com o pincel do mapa de calor
    matriz = new double*[n+2];
    for(int i=0;i<n+2;i++){
        matriz[i] = new double[m+2];
        for(int j=0;j<m+2;j++){
            matriz[i][j] = 0;
        }
    }

    // prenche a matriz com valores onde o jogador se posicionou

    // proporção dos pixels
    double pX = xDomain/n;
    double pY = yDomain/m;
    int u,v; // posição em pixels

    for(int i=0;i<tDomain;i++){

        u = (xDomain*0.5 + x[i])/pX;
        v = (yDomain*0.5 + y[i])/pY;

        // correção caso os pixels estejam fora do intervalo do campo
        if(u<1) u=2;
        if(u>=n) u=n-2;
        if(v<1) v=2;
        if(v>=m) v=m-2;

        // gera o pincel do mapa de calor (matriz 3x3)
        matriz[u][v] += 1; // pixel central (onde o jogador está posicionado)
        // pixels periféricos verticais e horizontais
        matriz[u][v-1] += 0.5;
        matriz[u][v+1] += 0.5;
        matriz[u-1][v] += 0.5;
        matriz[u+1][v] += 0.5;
        //pixels periféricos diagonais
        matriz[u-1][v-1] += 0.25;
        matriz[u-1][v+1] += 0.25;
        matriz[u+1][v-1] += 0.25;
        matriz[u+1][v+1] += 0.25;
    }
}


// retorna o vetor de valores de X que foram ocupados num período tDomain
QVector<double> Field::X(){
    QVector<double> t(tDomain);
    for(int i=0;i<tDomain;i++){
        t[i]=x[i];
    }
    return t;
}

// retorna o vetor de valores de Y que foram ocupados num período tDomain
QVector<double> Field::Y(){
    QVector<double> t(tDomain);
    for(int i=0;i<tDomain;i++){
        t[i]=y[i];
    }
    return t;
}

// retorna a matriz com os pontos onde o jogador passou
double** Field::getMatriz(){
    return matriz;
}

// deleta toda a matriz do mapa de calor
void Field::deletaMatriz(){
    for(int i=0;i<n+2;i++){
        delete[] matriz[i];
    }
    delete[] matriz;
}
