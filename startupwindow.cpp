#include "startupwindow.h"
#include "ui_startupwindow.h"

StartupWindow::StartupWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartupWindow)
{
    ui->setupUi(this);
    ui->background->setPixmap(QPixmap(":/images/Soccer_Field_Transparant.png"));
    ui->background->setScaledContents(true);
}

StartupWindow::~StartupWindow()
{
    if(loaded){
        delete arq;
        delete inicio1;
        delete fim1;
        delete inicio2;
        delete fim2;
        delete A;
        delete B;
        delete C;
        delete D;
        delete O;
    }
    delete ui;
}

void StartupWindow::setCoordinates(){
    A = new QGeoCoordinate(ui->AlineEdit->text().toDouble(),ui->AlineEdit_2->text().toDouble());
    B = new QGeoCoordinate(ui->BlineEdit->text().toDouble(),ui->BlineEdit_2->text().toDouble());
    C = new QGeoCoordinate(ui->ClineEdit->text().toDouble(),ui->ClineEdit_2->text().toDouble());
    D = new QGeoCoordinate(ui->DlineEdit->text().toDouble(),ui->DlineEdit_2->text().toDouble());
    O = new QGeoCoordinate(ui->OlineEdit->text().toDouble(),ui->OlineEdit_2->text().toDouble());
}

QGeoCoordinate StartupWindow::getA(){
    return *A;
}

QGeoCoordinate StartupWindow::getB(){
    return *B;
}

QGeoCoordinate StartupWindow::getC(){
    return *C;
}

QGeoCoordinate StartupWindow::getD(){
    return *D;
}

QGeoCoordinate StartupWindow::getO(){
    return *O;
}

QTime StartupWindow::getInicio1(){
    return *inicio1;
}

QTime StartupWindow::getFim1(){
    return *fim1;
}

QTime StartupWindow::getInicio2(){
    return *inicio2;
}

QTime StartupWindow::getFim2(){
    return *fim2;
}

void StartupWindow::on_fileButton_clicked()
{
    // armazena o arquivo endereço do arquivo .csv na variável filename
    QString filename = QFileDialog::getOpenFileName(this,tr("Abrir Arquivo"),tr("/home/"),tr("CSV File (*.csv)"));
    arq = new QFile(filename); // armazena o arquivo a partir do endereço na variável arq
    if(!arq->open(QFile::ReadOnly | QFile::Text)){
        ui->lineEdit->setText("");
        ui->timeBox->setEnabled(false);
        ui->buttonBox->setEnabled(false);
        ui->coordBox->setEnabled(false);
        loaded=0;
    }else{
        // caso o arquivo seja lido com sucesso, é possível definir os tempos de jogo
        ui->lineEdit->setText(filename);
        ui->timeBox->setEnabled(true);
        ui->buttonBox->setEnabled(true);
        ui->coordBox->setEnabled(true);
        loaded=1;
    }
}

// armazena os tempos definidos pelo usuário nas devidas variáveis
void StartupWindow::on_buttonBox_accepted()
{
    inicio1 = new QTime(ui->inicio1timeEdit->time());
    fim1 = new QTime(ui->fim1timeEdit->time());
    inicio2 = new QTime(ui->inicio2timeEdit->time());
    fim2 = new QTime(ui->fim2timeEdit->time());
    setCoordinates();
}
