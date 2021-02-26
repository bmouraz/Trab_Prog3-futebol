#ifndef STARTUPWINDOW_H
#define STARTUPWINDOW_H

#include <QDialog>
#include <QFileDialog>
#include <QGeoCoordinate>

namespace Ui {
class StartupWindow;
}

class StartupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StartupWindow(QWidget *parent = 0);
    ~StartupWindow();

    void setCoordinates();  // configura as coordenadas do campo
    // retornam os vértices e o centro do campo
    QGeoCoordinate getA();
    QGeoCoordinate getB();
    QGeoCoordinate getC();
    QGeoCoordinate getD();
    QGeoCoordinate getO();
    QTime getInicio1();     // retorna o inicio do primeiro tempo
    QTime getFim1();        // returna o final do primeiro tempo
    QTime getInicio2();     // retorna o inicio do segundo tempo
    QTime getFim2();        // retorna o final do segundo tempo
    QFile *arq;

private slots:
    void on_fileButton_clicked(); // abre a janela para inserção de arquivo
    void on_buttonBox_accepted(); // armazena os tempos definidos na dialog

private:
    Ui::StartupWindow *ui;
    QTime *inicio1;
    QTime *fim1;
    QTime *inicio2;
    QTime *fim2;

    QGeoCoordinate *A;
    QGeoCoordinate *B;
    QGeoCoordinate *C;
    QGeoCoordinate *D;
    QGeoCoordinate *O;

    int loaded;

};

#endif // STARTUPWINDOW_H
