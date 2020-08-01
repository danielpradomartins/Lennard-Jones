#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "ambiente.h"
#include "arquivos.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_action_Roda_triggered();

    void on_actionPause_triggered();

    void on_play_clicked();

    void on_criar_clicked();

    void on_pause_clicked();

private:
    Ui::MainWindow *ui;
    ambiente *caixaFechada;
    QTimer *relogio;

    arquivos arqPotential, arqKinect, arqEnergy, arqMoment;
    int t = 0; //varialvel para contagem do numero de passos dados (tempo).

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
