#include "arquivos.h"
#include <QStringList>

arquivos::arquivos()
{
}

bool arquivos::abre(string nome)
{
    fp.open(nome.c_str());
    return fp.is_open();
}

bool arquivos::abre(QString nome)
{
    fp.open(nome.toLocal8Bit());
    return fp.is_open();

}

bool arquivos::leTudo(vector<vector<double> > *mat)
{
    double aux;

    mat->clear();

    while(!fp.eof()) // enquanto não falhar
    {
        string linha;
        getline(fp,linha);
        QString Qlinha(linha.c_str()); // converte linha de string para QString
        QStringList elementos; //cria uma lista de string
        elementos = Qlinha.split(";");
        vector <double> linAux;
        for(int i=0;i<elementos.size();i++)
        {
            aux=elementos[i].toDouble(); //to double converte uma string para número
            linAux.push_back(aux);
        }

        if(linAux.size()>1)
            mat->push_back(linAux);
    }

    return fp.eof(); // se chegou no fim do arquivo ta legal!
}

void arquivos::fechaLeitura()
{
    this->fp.close();
}

void arquivos::fechaGravacao()
{
    this->qp.close();
}

void arquivos::grava(string linha)
{
    qp << linha <<endl;
}

void arquivos::grava(double num)
{
    qp << num <<endl;
}

void arquivos::grava(double numx, double numy)
{
//    qp << numx ;
    qp << numx << '\t'  << numy <<endl;
}

bool arquivos::criaArq(string nomeArq)
{
    qp.open(nomeArq.c_str());
    return qp.is_open();
}
