#ifndef AMBIENTE_H
#define AMBIENTE_H
#include "particula.h"
#include <vector>
#include <cmath>

using namespace std;

class ambiente
{
public:
    // construtor
    ambiente(int n1, double dens1, double E1, double alpha1, int pos01);

    void posT0(int n, int pos0);
    double L; //largura do ambiente

    // responsável pelo ciclo de vida do ambiente
    void integra();
    // retorna a localizacão da particula
    Tpos get_loc(int i){return vecParticula[i].get_loc();}
    // retorna o momento da particula
    Tmomento get_moment(int i){return vecParticula[i].get_moment();}
    // retorna o numero de particulas no ambiente
    int get_particula_size() {return vecParticula.size();}
    //calcula a distancia entre as particulas i e j
    double distancia(int i, int j);
    double distancia(double x, double y, int j);
    double dx,dy;
    void force();
    double energiaK();
    double energiaU();
    double bo; // Energia da configuração inicial
    double energiaM();
    double moment();
    void anulaMomento();
    vector <double> fx, fy;
    vector <double> U;
  //  vector <double> K;



private:
    vector <particula> vecParticula;
    vector <double> c,d;
    double C_alpha;
    int  n;
    double dens;
    double E; //energia do sistema
    double alpha;
    int pos0;
    double r_min;
    double Ntilde;
    double dim;

};

#endif // AMBIENTE_H
