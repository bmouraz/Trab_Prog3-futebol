#include "mainwindow.h"
#include "ui_mainwindow.h"

// construtor da janela principal
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){

    startDialog = new StartupWindow(this);
    startDialog->setModal(true);
    startDialog->exec();
    if(startDialog->result()){
        ui->setupUi(this);
        setupData();
    }
}

// destrutor da janela principal
MainWindow::~MainWindow()
{
    delete ui;
    if(startDialog->result()){
        for(int i=0;i<jogadores->size();i++){
            delete jogadores->at(i);
        }
        delete jogadores;
        delete gps;
        delete coordenadas;
    }
}

// configura o programa ao alterar o arquivo de dados
void MainWindow::setupData(){
        currentPlayer = 0;
        currentHalf = 0;
        currentTime = 0;

        coordenadas = new QList<QGeoCoordinate>;
        coordenadas->append(startDialog->getA());
        coordenadas->append(startDialog->getB());
        coordenadas->append(startDialog->getC());
        coordenadas->append(startDialog->getD());
        coordenadas->append(startDialog->getO());

        jogadores = new QList<Player*>;
        gps = new GPS(jogadores,
                      startDialog->arq,
                      startDialog->getInicio1(),
                      startDialog->getFim1(),
                      startDialog->getInicio2(),
                      startDialog->getFim2());

        setPlots();
        setOrderTable();

        ui->timeSlider->setMaximum(jogadores->at(currentPlayer)->getParte(currentHalf)->size()); // define posição máxima do slider
        ui->timeDisplay->setTime(jogadores->at(currentPlayer)->getParte(currentHalf)->getTime().at(0)); // inicializa o mostrador de hora
        ui->dateDisplay->setDate(jogadores->at(currentPlayer)->getParte(currentHalf)->getDate().at(0)); // inicializa o mostrador de data
}

// reinicia a interface da janela principal
void MainWindow::refresh(){
    int n_jogadores = jogadores->size();
    ui->trackidvalue->clear();
    ui->orderTable->clear();
    for(int i=0;i<jogadores->size();i++){
        delete jogadores->at(i);
    }
    delete jogadores;
    delete bgCampo1;
    delete bgCampo2;
    delete coordenadas;
    for(int i=0;i<=n_jogadores;i++){
        ui->orderTable->removeRow(i);
    }
    setupData();
}

// chama as funções para atualizar os gráficos
void MainWindow::setPlots(){
    ui->plotDeslocamento->setCurrentLayer("background");
    bgCampo1 = new QCPItemPixmap(ui->plotDeslocamento);
    bgCampo1->topLeft->setCoords(-40,60);
    bgCampo1->bottomRight->setCoords(40,-60);
    bgCampo1->setScaled(true,Qt::IgnoreAspectRatio);
    bgCampo1->setPixmap(QPixmap(":/images/Soccer_Field_Transparant.png"));

    bgCampo2 = new QCPItemPixmap(ui->plotColorMap);
    bgCampo2->topLeft->setCoords(-40,60);
    bgCampo2->bottomRight->setCoords(40,-60);
    bgCampo2->setScaled(true,Qt::IgnoreAspectRatio);
    bgCampo2->setPixmap(QPixmap(":/images/Soccer_Field_Transparant.png"));

    plotDisplacement(currentPlayer, currentHalf, currentTime);
    plotColorMap(currentPlayer, currentHalf, currentTime);

    // esconde os eixos dos gráficos que mostram a escala do campo
    ui->plotDeslocamento->xAxis->setVisible(true);
    ui->plotDeslocamento->yAxis->setVisible(true);
    ui->plotColorMap->xAxis->setVisible(true);
    ui->plotColorMap->yAxis->setVisible(true);

    // determina os alcances do gráfico
    ui->plotDeslocamento->xAxis->setRange(-40, 40);
    ui->plotDeslocamento->yAxis->setRange(-60, 60);
}

// organiza a tabela de jogadores ordenada
void MainWindow::setOrderTable(){
    // número de jogadores no arquivo
    int n_jogadores = jogadores[0].size();

    // cria a gaveta com todos os jogadores
    for(int i=0;i<n_jogadores;i++){
        ui->trackidvalue->addItem(QString::number(i+1));
    }

    QStringList header;                                   // cabeçalho da tabela
    ui->orderTable->setColumnCount(3);                    // define o numero de colunas da tabela
    ui->orderTable->setRowCount(0);                       // define o numero de linhas inicial da tabela
    header << "Track ID" << "Dist. km" << "Vel. km/h";    // título do cabeçalho da tabela
    ui->orderTable->setHorizontalHeaderLabels(header);

    // insere linha a linha os valores da tabela
    for(int i=0; i<n_jogadores;i++){
        ui->orderTable->insertRow(ui->orderTable->rowCount());
        int currentLine = ui->orderTable->rowCount() -1;

        QTableWidgetItem* itemindex = new QTableWidgetItem;
        itemindex->setData(Qt::DisplayRole,i+1);
        ui->orderTable->setItem(currentLine,INDEX,itemindex);

        QTableWidgetItem* itemdist = new QTableWidgetItem;
        itemdist->setData(Qt::DisplayRole,(jogadores->at(i)->getDistanciaPercorrida()/1000.0));
        ui->orderTable->setItem(currentLine,DISTANCIA,itemdist);

        QTableWidgetItem* itemvel = new QTableWidgetItem;
        itemvel->setData(Qt::DisplayRole,(jogadores->at(i)->getVelocidadeMax()));
        ui->orderTable->setItem(currentLine,VELOCIDADE,itemvel);

    }
    ui->orderTable->sortByColumn(1,Qt::DescendingOrder); // organiza a tabela em ordem crescente
}

// mostra o deslocamento do jogador no campo
void MainWindow::plotDisplacement(int player, int half=0, int tDomain = 0){
    ui->plotDeslocamento->clearPlottables();    // limpa o gráfico anterior antes de um novo plot

    int n = ui->plotDeslocamento->width();      // define o número de pixels na horizontal da matriz do gráfico
    int m = ui->plotDeslocamento->height();     // define o número de pixels na vertical da matriz do gráfico


    // cria um gráfico de curvas e associa ao Widget plotDeslocamento
    QCPCurve *desloc = new QCPCurve(ui->plotDeslocamento->xAxis, ui->plotDeslocamento->yAxis);

    // aloca um campo com o jogador selecionado na parte da atividade selecionada
    Field *campo = new Field(coordenadas,jogadores->at(player),half,n,m,tDomain);

    // insere as coordenadas cartesianas do jogador na curva de deslocamento
    desloc->setData(campo->X(),campo->Y());

    // define a última posição do jogador no gráfico como um ponto
    QPen pen;
    pen.setColor(Qt::black);
    ui->plotDeslocamento->addGraph();
    ui->plotDeslocamento->graph(0)->setPen(pen);
    ui->plotDeslocamento->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    if(!(campo->X().isEmpty())){
        ui->plotDeslocamento->graph(0)->setData(QVector<double>(1,campo->X().last()),QVector<double>(1,campo->Y().last()));
    }

    ui->plotDeslocamento->replot(); // plota o gráfico após todas as configurações serem organizadas

    delete campo;                               // desaloca o campo
    ui->plotDeslocamento->removePlottable(0);   // desaloca a curva desloc
    ui->plotDeslocamento->removeGraph(0);       // desaloca o ponto do jogador
}

// mostra o mapa de calor com as regiões mais percorridas pelo jogador
void MainWindow::plotColorMap(int player, int half, int time){
    ui->plotColorMap->clearPlottables();    // limpa o gráfico anterior antes de um novo plot

    // cria um mapa de cores e associa ao Widget plotColorMap
    QCPColorMap *colorMap = new QCPColorMap(ui->plotColorMap->xAxis,ui->plotColorMap->yAxis);

    colorMap->setInterpolate(false);    // não interpola as cores do gráfico

    // define o número de pixels da matriz do gráfico na escala definida
    int n = ui->plotDeslocamento->size().width()*0.15;
    int m = ui->plotDeslocamento->size().height()*0.15;

    colorMap->data()->setSize(n,m);                                 // define o tamanho em pixels do mapa
    colorMap->data()->setRange(QCPRange(-40,40),QCPRange(-60,60));  // define o alcance do gráfico
    colorMap->data()->fillAlpha(180);                               // adiciona transparência a todos os pixels do gráfico

    // cria o campo com os dados que serão mostrados no mapa
    Field *campo = new Field(coordenadas,jogadores->at(player),half,n,m,time);
    double **z = campo->getMatriz();                                // matriz de cores do campo
    for (int i=0; i<n; ++i)
    {
      for (int j=0; j<m; ++j)
      {
        // caso a cor tenha valor zero na matriz, tornar o pixel transparente
        if(z[i][j]==0) colorMap->data()->setAlpha(i,j,0);

        // insere a matriz de cores do campo na matriz de cores do mapa
        colorMap->data()->setCell(i, j, z[i][j]);
      }
    }

    colorMap->setGradient(QCPColorGradient::gpSpectrum);            // cor do mapa


    colorMap->rescaleDataRange();           // organiza a escala de cores
    ui->plotColorMap->rescaleAxes();        // escala mapa para se adequar aos valores de alcance do gráfico
    ui->plotColorMap->replot();             // plota o gráfico após todas as configurações serem organizadas
    delete campo;                           // desaloca o campo
    ui->plotColorMap->removePlottable(0);   // desaloca o mapa de cores
}

// atualiza automaticamente a posição do jogador ao longo do tempo
void MainWindow::animateGraph(){
    int tDomain = jogadores->at(currentPlayer)->getParte(currentHalf)->getTime().size();
    for(int t=0;t< tDomain ;t++){
        MainWindow::plotDisplacement(currentPlayer,currentHalf,t);
        MainWindow::plotColorMap(currentPlayer, currentHalf, t);
        ui->timeSlider->setValue(t);
    }
}

// altera os dados ao mudar o jogador
void MainWindow::on_trackidvalue_currentIndexChanged(int k)
{
    if(k>-1){
        ui->tempo1value->setText(jogadores->at(k)->getTempo1().toString());
        ui->tempo2value->setText(jogadores->at(k)->getTempo2().toString());
        ui->tempoEfetivovalue->setText(jogadores->at(k)->getTempoEfetivo().toString());
        ui->velmaxmsvalue->setText(QString::number(jogadores->at(k)->getVelocidadeMax()/3.6) + "  m/s");
        ui->velmaxkmhvalue->setText(QString::number(jogadores->at(k)->getVelocidadeMax()) + "  km/h");
        ui->corr18value->setText("  18 km/h:  " + QString::number(jogadores->at(k)->getCorridas18kmh()));
        ui->corr20value->setText("  20 km/h:  " + QString::number(jogadores->at(k)->getCorridas20kmh()));
        ui->time18Display->setTime(jogadores->at(k)->getTempo18());
        ui->time20Display->setTime(jogadores->at(k)->getTempo20());
        ui->distanciaPercorridavalue->setText(QString::number(jogadores->at(k)->getDistanciaPercorrida()/1000.0) + "   km");

        currentPlayer = k;

        // define posição máxima do slider
        ui->timeSlider->setMaximum(jogadores->at(currentPlayer)->getParte(currentHalf)->size());

        // atualiza os gráficos
        MainWindow::plotDisplacement(currentPlayer,currentHalf, currentTime);
        MainWindow::plotColorMap(currentPlayer, currentHalf, currentTime);
    }
}

// altera os dados ao selecionar o primeiro tempo
void MainWindow::on_tempo1radioButton_released()
{
    currentHalf = 0;    // seleciona a primeira metade da atividade

    // define posição máxima do slider
    ui->timeSlider->setMaximum(jogadores->at(currentPlayer)->getParte(currentHalf)->size());
    // atualiza o valor de hora nos dados do jogo
    ui->timeDisplay->setTime(jogadores->at(currentPlayer)->getParte(currentHalf)->getTime().at(currentTime));

    // atualiza os gráficos
    MainWindow::plotDisplacement(currentPlayer,currentHalf,currentTime);
    MainWindow::plotColorMap(currentPlayer, currentHalf, currentTime);
}

// altera os dados ao selecionar o segundo tempo
void MainWindow::on_tempo2radioButton_released()
{
    currentHalf = 1;    // seleciona a segunda metade da atividade

    // define posição máxima do slider
    ui->timeSlider->setMaximum(jogadores->at(currentPlayer)->getParte(currentHalf)->size());
    // atualiza o valor de hora nos dados do jogo
    ui->timeDisplay->setTime(jogadores->at(currentPlayer)->getParte(currentHalf)->getTime().at(currentTime));

    // atualiza os gráficos
    MainWindow::plotDisplacement(currentPlayer,currentHalf,currentTime);
    MainWindow::plotColorMap(currentPlayer, currentHalf, currentTime);
}

// altera os dados ao variar o slider de tempo
void MainWindow::on_timeSlider_valueChanged(int value)
{
    currentTime = value;    // atualiza o valor do tempo atual

    // atualiza o valor de hora nos dados do jogo
    ui->timeDisplay->setTime(jogadores->at(currentPlayer)->getParte(currentHalf)->getTime().at(currentTime));

    // atualiza os gráficos
    MainWindow::plotDisplacement(currentPlayer,currentHalf,currentTime);
    MainWindow::plotColorMap(currentPlayer, currentHalf, currentTime);

}

// abre a janela para alterar o arquivo de dados
void MainWindow::on_actionArquivo_triggered()
{
    startDialog = new StartupWindow(this);
    startDialog->setModal(true);
    startDialog->exec();
    if(startDialog->result()) refresh();
}

// abre a janela de créditos do software
void MainWindow::on_actionSobre_triggered()
{
    QMessageBox::about(this,"Sobre","Universidade Federal de Santa Catarina\nCentro Tecnológico de Joinville\nProjeto final de Programação III\nDesenvolvido por\nBruno Moura - Engenharia Mecatrônica\nMairon S. Wolniewicz - Engenharia Aeroespacial");
}

// mostra ou esconde a grade e escala dos gráficos
void MainWindow::on_escalacheckBox_stateChanged(int arg1)
{
    ui->plotDeslocamento->xAxis->setVisible(arg1);
    ui->plotDeslocamento->yAxis->setVisible(arg1);
    ui->plotColorMap->xAxis->setVisible(arg1);
    ui->plotColorMap->yAxis->setVisible(arg1);

    // atualiza os gráficos
    MainWindow::plotDisplacement(currentPlayer,currentHalf, currentTime);
    MainWindow::plotColorMap(currentPlayer, currentHalf, currentTime);
}

// ativa a animação dos gráficos
void MainWindow::on_animateButton_clicked()
{
    ui->animateButton->setText("Aguarde");
    ui->tempo1radioButton->setEnabled(false);
    ui->tempo2radioButton->setEnabled(false);
    ui->trackidvalue->setEnabled(false);
    ui->escalacheckBox->setEnabled(false);
    ui->animateButton->setEnabled(false);
    animateGraph();
    ui->tempo1radioButton->setEnabled(true);
    ui->tempo2radioButton->setEnabled(true);
    ui->trackidvalue->setEnabled(true);
    ui->escalacheckBox->setEnabled(true);
    ui->animateButton->setEnabled(true);
    ui->animateButton->setText("Animar");
}
