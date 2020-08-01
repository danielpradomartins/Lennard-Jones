#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <QString>
using namespace std;

class arquivos
{
public:
    arquivos();
    bool abre(string nome);
    bool abre(QString nome);

    bool leTudo(vector <vector <double> > *mat);
    void fechaLeitura();
    void fechaGravacao();
    void grava(string linha);
    void grava(double num);
    void grava(double numx,double numy);
    bool criaArq(string nomeArq);

private:
    string fileName;
    ifstream fp;    // variavel do arquivo de entrada
    ofstream qp;    // variavel do arquivo de saida

};

#endif // ARQUIVOS_H
