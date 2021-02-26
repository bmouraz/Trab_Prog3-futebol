#include "gps.h"

#include <QFile>
#include <QTextStream>

// construtor padrão da classe GPS
GPS::GPS(QObject *parent) : QObject(parent){}

// construtor da classe GPS
// recebe como arguento uma lista de jogadores
GPS::GPS(QList<Player*> *jogadores, QFile *arquivo, QTime ti1, QTime tf1, QTime ti2, QTime tf2){

    // dados de tempo inicial e final
    tempoInicial1   = ti1;
    tempoFinal1     = tf1;
    tempoInicial2   = ti2;
    tempoFinal2     = tf2;

    leDados(arquivo);
    verificaTempos(jogadores);
}

// destrutor da classe GPS
GPS::~GPS(){
    delete csvDados;
}

// le os dados dos arquivos e organiza em uma tabela QStandardItemModel
void GPS::leDados(QFile *arquivo){
    // cria uma tabela para armazenar os dados do arquivo csv
    csvDados = new QStandardItemModel(this);

    // função para receber o texto do arquivo
    QTextStream in(arquivo);

    // organiza o cabeçalho da tabela
    QString header = in.readLine();
    csvDados->setHorizontalHeaderLabels(QStringList() << header.split(","));

    // lê os dados até o fim do arquivo csv
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QList<QStandardItem *> standardItemsList;
        // lê as linhas separadas por vírgula
        for (QString item : line.split(",")) {
            standardItemsList.append(new QStandardItem(item));
        }
        csvDados->insertRow(csvDados->rowCount(), standardItemsList);
    }
    arquivo->close();
}

// com base nos tempos inicial e final de cada parte da atividade, separar os dados para cada jogador
void GPS::verificaTempos(QList<Player*> *jogadores){
    int trackID = 1; // numero do jogador inicial
    int rows = csvDados->rowCount(); // numero de linhas na tabela
    int p = 1; // define a parte da atividade (1 ou 2)
    bool trocaPlayer; // verifica se um novo jogador foi alocado
    bool fixStart = false; // fixa o valor do início do setStrings
    bool fixEnd = false; // fixa o valor do final do setStrings
    int start=0;
    int end=0;
    for(int i=0;i<rows;i++){    // le toda a tabela
        trocaPlayer = false;
        // caso o valor da Track ID mude ou o contador alance o fim da tabela
        if((trackID!=getTrackID(i)) || (i==rows-1)){
            trocaPlayer = true;
            trackID = getTrackID(i);
        }

        // variável auxiliar para comparação de tempos em formato QTime
        QTime lt = QTime::fromString((csvDados->item(i,6))->text(),"h:m:s");

        // caso a atividade esteja na parte 1, verificar os intervalos iniciais e finais
        if(p==1){
//            qDebug()<<lt<<tempoInicial1<<fixEnd;
            // caso o tempo atual seja maior que o inicial, fixar o Start
            if(lt>=tempoInicial1 && fixStart == false){
                start = i;
                fixStart = true;
            }
            // caso o tempo atual seja maior ou igual que o final, fixar o End
            if(lt>=tempoFinal1 && fixEnd == false && fixStart == true){
                end = i;
                fixEnd = true;
                setStrings(start,end,true); // listas de strings armazenam os valores para o Track ID correspondente
                p=2; // ao fixar o final da parte 1, a parte 2 pode iniciar
                fixStart = false;
                pt1 = new Half(strLocalDate,
                               strLocalTime,
                               strLatitude,
                               strNS,
                               strLongitude,
                               strEW,
                               strSpeed);
            }
        }

        if(p==2){
            if(lt>=tempoInicial2 && fixStart == false){
                start = i;
                fixStart = true;
                fixEnd = false;
            }
            if(lt>=tempoFinal2 && fixEnd == false && fixStart == true){
                end = i-1;
                fixEnd = true;
                setStrings(start,end,true); // listas de strings armazenam os valores para o Track ID correspondente
                pt2 = new Half(strLocalDate,
                               strLocalTime,
                               strLatitude,
                               strNS,
                               strLongitude,
                               strEW,
                               strSpeed);
            }
        }

        // um novo jogador é alocado com as listas de string como parâmetro
        if(trocaPlayer == true){
            QList<Half*> *parte = new QList<Half*>; // aloca uma lista de Half
            parte->append(pt1);                     // insere a parte 1 na lista Half
            parte->append(pt2);                     // insere a parte 2 na lista Half
            Player *temp = new Player(parte);       // aloca um novo jogador com o parâmetro parte
            delete parte;                           // no construtor do jogador é realizada a cópia de parte, que deve ser deletada agora
            jogadores->append(temp);                // o novo jogador é adicionado à lista de jogadores
            start = i;                              // a próxima iteração inicia onde a última terminou
            fixStart = false;
            fixEnd = false;
            p = 1;
        }
    }
}

// retorona o valor do Track ID para reconhecimento do jogador
int GPS::getTrackID(int i){
    return ((csvDados->item(i,1))->text()).toInt();
}

// converte os valores QStandardItemModel para QList<QString>
void GPS::setStrings(int start, int end, bool troca){
    // zera todas as listas toda vez que um novo jogador é alocado
    if(troca==true){
        strLocalDate.clear();
        strLocalTime.clear();
        strLatitude.clear();
        strNS.clear();
        strLongitude.clear();
        strEW.clear();
        strSpeed.clear();
    }
    // insere os dados correspondentes em formato de texto nas listas
    for(int i=start;i<=end;i++){
        strLocalDate.append((csvDados->item(i,5))->text());
        strLocalTime.append((csvDados->item(i,6))->text());
        strLatitude.append((csvDados->item(i,8))->text());
        strNS.append((csvDados->item(i,9))->text());
        strLongitude.append((csvDados->item(i,10))->text());
        strEW.append((csvDados->item(i,11))->text());
        strSpeed.append((csvDados->item(i,13))->text());
    }
}
