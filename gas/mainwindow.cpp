#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->caixaFechada = NULL;
    //Distribuicao aleatoria das particulas opcao default
    ui->radioButtonAL->setChecked(true);


    this->relogio = new QTimer(this);
    // conectando emissor e receptor do sinal
    connect(this->relogio,SIGNAL(timeout()),
            //            this,SLOT(on_action_Roda_triggered()));
            this,SLOT(on_play_clicked()));
}
//==============================================
MainWindow::~MainWindow() //metodo destrutor
{
    if(this->caixaFechada!=NULL) delete caixaFechada;
    delete ui;
}

//==============================================
void MainWindow::on_action_Roda_triggered()
{
    this->caixaFechada->integra();
    this->repaint(); // atualiza a tela
    this->relogio->start(1);
}
//==============================================
void MainWindow::on_play_clicked()
{
    this->arqPotential.grava(t , this->caixaFechada->energiaU());
    this->arqKinect.grava(t , this->caixaFechada->energiaK());
    this->arqEnergy.grava(t , this->caixaFechada->energiaM());
    this->arqMoment.grava(t , this->caixaFechada->moment());


    this->caixaFechada->integra();



    this->repaint(); // atualiza a tela
    this->relogio->start(0.00001);
    t++;
}
//==============================================
void MainWindow::on_actionPause_triggered()
{
    relogio->stop(); // pausa a animação
    this->arqPotential.fechaGravacao();
}
//==============================================
void MainWindow::on_pause_clicked()
{
    relogio->stop(); // pausa a animação
    this->arqPotential.fechaGravacao();
    this->arqKinect.fechaGravacao();
    this->arqMoment.fechaGravacao();
}

//==============================================
//Criando ambiente
void MainWindow::on_criar_clicked()
{
    //  srand(2); //semente

    //criacao de arquivos de saida
    this->arqPotential.criaArq("potential.out");
    this->arqKinect.criaArq("kinect.out");
    this->arqEnergy.criaArq("energy.out");
    this->arqMoment.criaArq("moment.out");


    //Ler dados de entrada
    int Npart = this->ui->Np->value();
    double dens = this->ui->densidade->value();
    double E = this->ui->energia->value();
    double a = this->ui->alpha->value();

    //tipo de distribuicao inicial das particulas (pos0=1:aleatoria ; pos0=2:quadrada ; pos0=3:triangular)
    int pos0;

    if(this->ui->radioButtonAL->isChecked() == true){
        pos0 = 1;
        delete caixaFechada;//se caixaFechada ja foi criado -> delete caixaFechada
        this->caixaFechada = new ambiente(Npart, dens, E, a, pos0);
        this->repaint(); //redesenha tudo na janela
    }

    if(this->ui->radioButtonQuad->isChecked() == true){
        pos0 = 2;
        //testa se sqrt(Npart) é inteiro quando a rede e' quadrada
        if( sqrt(double(Npart)) != round(sqrt(double(Npart))) ) {
            this->ui->dialog->appendPlainText("ERRO 1: Numero de particulas invalido");
            if(this->caixaFechada!=NULL) delete caixaFechada;
            this->caixaFechada = NULL;
            this->repaint();

        }
        else{
            if(this->caixaFechada!=NULL) delete caixaFechada;
            this->caixaFechada = new ambiente(Npart, dens, E, a, pos0);
            this->repaint();
        }
    }

    if(this->ui->radioButtonTri->isChecked() == true){
        pos0 = 3;
        //testa se sqrt(Npart) é inteiro quando a rede e' quadrada
        if( ( -1.0 + sqrt(1.0 + 8.0*Npart) ) / 2.0 != round(( -1.0 + sqrt(1.0 + 8.0*Npart) ) / 2.0) ) {
            this->ui->dialog->appendPlainText("ERRO 1: Numero de particulas invalido");
            if(this->caixaFechada!=NULL) delete caixaFechada;
            this->caixaFechada = NULL;
            this->repaint();

        }
        else{
            if(this->caixaFechada!=NULL) delete caixaFechada;
            this->caixaFechada = new ambiente(Npart, dens, E, a, pos0);
            this->repaint();
        }
    }


    //verifica se a configuracao inicial fornece uma energia potencial menor do que a Energia E fornecida
    if(this->caixaFechada!=NULL){
        if(this->caixaFechada->energiaU() > E){
            this->ui->dialog->appendPlainText("ERRO 2: Energia Potencial da config inicial é maior do que a enegia fornecida");
            delete caixaFechada;
            this->caixaFechada = NULL;
            this->repaint();
        }

    }

    //verifica se o parametro alpha eh menor que 12
    if(a >= 12.0){
        this->ui->dialog->appendPlainText("ERRO 0: O parametro alpha tem de ser menor que 12.");
        if(this->caixaFechada!=NULL) delete caixaFechada;
        this->caixaFechada = NULL;
        this->repaint();
    }
}




//==============================================

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter estojo;
    estojo.begin(this);
    estojo.setBrush(QColor(70,70,70)); //define cor
    double x,y,w,h;
    double fX,fY;
    x=this->ui->contorno->geometry().x()+1; //define x igual a posição x mais 1 do objeto contorno de janela.ui
    y=this->ui->contorno->geometry().y()+13;
    w=this->ui->contorno->geometry().width() - 3; //define w igual a largura menos 3 do objeto contorno de janela.ui
    h=this->ui->contorno->geometry().height() - 2;


    estojo.drawRect(x,y,w,h); //desenha um retângulo na posição(x,y), com w de largura e h de altura

    if(this->caixaFechada==NULL) //teste para verificar se o objeto caixaFexada foi criado
        return;

    fX= w/(caixaFechada->L); //define a fração x, usada para normalizar a posição x de acordo com tamanho de janela.ui
    fY= h/(caixaFechada->L);



    estojo.setBrush(QColor(0,255,0));
    for (int i = 0; i < caixaFechada->get_particula_size(); ++i) {
        // localização das particulas na janela
        double xj = x + fX*caixaFechada->get_loc(i).x;
        double yj = y + fY*caixaFechada->get_loc(i).y;

        // desenha uma particula
        estojo.drawEllipse(xj - (fX/2) , yj - (fY/2) , fX , fY);
    }
    estojo.end();


}
