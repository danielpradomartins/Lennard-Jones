#include "ambiente.h"
#include <algorithm>


ambiente::ambiente(int n1, double dens1, double E1, double alpha1, int pos01)
{
    n = n1;
    dens = dens1;
    E = E1;
    alpha = alpha1;
    pos0 = pos01;
    dim = 2.0;


    //------------

    if(alpha / dim > 1.0){
        Ntilde = (alpha/ dim) / ((alpha/ dim) -1.0);
    }

    if(alpha / dim == 1.0){
        Ntilde = log (n);
    }

    if(alpha / dim < 1.0){
        Ntilde = ( pow( n, (1.0 - alpha/dim)) ) / (1.0 - alpha/dim);
    }

    //-------------



    L = sqrt(n/dens);  //largura da caixa
    r_min = pow((12.0 / alpha) , (1.0 / (12.0-alpha) ) );  // posicao do poco de potencial
   // r_min = 2.0;

    //constante do potencial de Mie
    C_alpha = ( pow( ((pow(12.0,12.0)) / (pow(alpha,alpha)) ) , (1.0 / (12.0-alpha)) ) ) / (12.0-alpha);
    C_alpha = C_alpha  / Ntilde;



    //constantes do metodo de yoshida de quarta ordem
    for(int i=0; i < 6; i++)
    {
        c.push_back(0);
        d.push_back(0);
    }

    double aux;
    aux= 2.0 - pow(2.0,(1.0/3.0));
    c[2] = 1.0 / (2.0*aux);
    c[3] = (1.0-pow(2.0,(1.0/3.0))) / (2.0*aux);
    c[4] = c[3];
    c[5] = c[2];

    d[2] = 1.0 / aux;
    d[3] = -pow(2.0,(1.0/3.0)) / aux;
    d[4] = d[2];
    d[5] = 0.0;


    //distribuicao das particulas e criacao das mesmas
    this->posT0(n,pos0);

    //cria vetor forca
    for(int i=0; i < vecParticula.size(); ++i)
    {
        fx.push_back(0);
        fy.push_back(0);
    }

    //Anulando o momento linear 15/10/15
    this->anulaMomento();


    //Ajuste da energia cinetica de modo a satisfazer a energia total por partícula fornecida
    for(int i=0; i < vecParticula.size(); ++i)
    {
        U.push_back(0);
    }
    double fitk, px, py;
    //fitk = ( E - this->energiaU() ) / this->energiaK();
    // O termo energiaU() que aparece em (E + U0) eh apenas um deslocamento da energia especificada
    // considerando a energia potencial  inicial como ponto de referencia
    bo = 0.0;
    fitk = ( (E + this->energiaU()) - this->energiaU() ) / this->energiaK();
    fitk = sqrt(fitk);
    for (int i = 0; i < vecParticula.size(); ++i){
        px = fitk * this->vecParticula[i].get_moment().px;
        py = fitk * this->vecParticula[i].get_moment().py;
        this->vecParticula[i].set_Momentx(px);
        this->vecParticula[i].set_Momenty(py);
    }
    bo = this->energiaU();

}
//==================Configuracao da Posicao Inicial das Particulas============================
void ambiente::posT0(int n, int pos0)
{
    double pos_max = L;
    double xi,yi;
    double w;
    int control;
    control = pos0;

    switch (control) {
    //Configuracao inicial aleatoria
    case 1:
        for (int i = 0; i < n; ++i) {
            xi= 0.25*pos_max + 0.5*pos_max*(double)rand()/RAND_MAX;
            yi= 0.25*pos_max + 0.5*pos_max*(double)rand()/RAND_MAX;
            //confere se existe sobreposicao das particulas
            for (int j = 1; j <= vecParticula.size();){
                if(this->distancia(xi,yi,j-1) > 1.5) j++;
                else {
                    xi= 0.25*pos_max + 0.5*pos_max*(double)rand()/RAND_MAX;
                    yi= 0.25*pos_max + 0.5*pos_max*(double)rand()/RAND_MAX;
                    j=1;
                }
            }
            particula padrao(L,xi,yi);
            this->vecParticula.push_back(padrao);
        }
        break;

        //Configuracao inicial rede quadrada
    case 2:

        w = L/2.0 - r_min*(sqrt((double)n)-1.0)/2.0; //O 2.0 multiplicando e' a distancia entre as particulas


        for (int i = 0; i < sqrt(double(n)); ++i) {
            xi = (i*r_min) + w;
            for (int j = 0; j < sqrt(double(n)); ++j) {
                yi = (j*r_min) + w;
                particula padrao(L,xi,yi);
                this->vecParticula.push_back(padrao);
            }
        }
        break;

        //Configuração inicial triangular
    case 3:
        double l, c;
     w = L/2.0;
     l = (-1.0 + sqrt(1.0 + 8.0*n) ) / 2.0; //l = numero de linhas e n = numero de particulas
     c = w-l*(r_min/2.0)*sqrt(3.0)/2.0;          //c = constante para centralizar o triangulo
                                             //o termo $(l-1.0)*sqrt(3.0)/2.0$ eh a altura do triangulo isoceles

     for (int i = 1; i <= l; ++i) {
         yi = ((i-1.0)*r_min*sqrt(3.0)/2.0) + c;// + (w-(l-1.0)*sqrt(3.0)/2.0)
         for (int j = 0; j < i; ++j) {
             xi = (j*r_min) + (w-(i-1.0)*r_min/2.0);
             particula padrao(L,xi,yi);
             this->vecParticula.push_back(padrao);
         }
     }

    }
}
//==============================================
//integra = metodo integrador simpletico de yoshida para n particulas
void ambiente::integra()
{
    double t=0.01;  //passo de tempo
    for (int s=2; s < 5; s++){
        for (int i = 0; i < vecParticula.size(); ++i){
            double x, y, px, py;


            x = this->vecParticula[i].get_loc().x;
            y = this->vecParticula[i].get_loc().y;
            px = this->vecParticula[i].get_moment().px;
            py = this->vecParticula[i].get_moment().py;

            x = x + t*c[s]*px;
            y = y + t*c[s]*py;

            this->vecParticula[i].set_Posx(x);
            this->vecParticula[i].set_Posy(y);
        }

        this->force();

        for (int i = 0; i < vecParticula.size(); ++i){
            double px, py;

            px = this->vecParticula[i].get_moment().px;
            py = this->vecParticula[i].get_moment().py;

            px = px - t*d[s]*fx[i];
            py = py - t*d[s]*fy[i];

            this->vecParticula[i].set_Momentx(px);
            this->vecParticula[i].set_Momenty(py);
        }
    }

    for (int i = 0; i < vecParticula.size(); ++i){
        double x, y, px, py;

        x = this->vecParticula[i].get_loc().x;
        y = this->vecParticula[i].get_loc().y;
        px = this->vecParticula[i].get_moment().px;
        py = this->vecParticula[i].get_moment().py;

        x = x + t*c[5]*px;
        y = y + t*c[5]*py;

        this->vecParticula[i].set_Posx(x);
        this->vecParticula[i].set_Posy(y);
    }
}
//==============================================
// distancia entre as particulas i e j
double ambiente::distancia(int i, int j)
{
    dx = this->vecParticula[i].get_loc().x -
            this->vecParticula[j].get_loc().x;
    dy = this->vecParticula[i].get_loc().y -
            this->vecParticula[j].get_loc().y;
    return sqrt(dx*dx + dy*dy);
}
//==============================================
// distancia entre o ponto P(x,y) a particila j
double ambiente::distancia(double x, double y, int j)
{
    dx = x - this->vecParticula[j].get_loc().x;
    dy = y - this->vecParticula[j].get_loc().y;
    return sqrt(dx*dx + dy*dy);
}
//==============================================
//calcula a forca exercida sobe cada particula
void ambiente::force()
{

    double d;
    double ax, ay;

    //zera a forca exercida sob cada particula
    for (int i = 0; i < vecParticula.size(); ++i){
        fx[i] = 0.0;
        fy[i] = 0.0;
    }

    for (int i = 0; i < vecParticula.size() - 1; ++i){
        for (int j = (i + 1); j < vecParticula.size(); ++j){
            d = this->distancia(i,j);

            ax = C_alpha*( -( 12.0 / pow(d,13.0)) + (alpha / pow(d,(alpha+1.0))) ) * dx/d;
            ay = C_alpha*( -( 12.0 / pow(d,13.0)) + (alpha / pow(d,(alpha+1.0))) ) * dy/d;

            fx[i] = fx[i] + ax;
            fy[i] = fy[i] + ay;

            fx[j] = fx[j] -ax;    // Terceira Lei de Newton
            fy[j] = fy[j] -ay;
        }
    }
    //Forca devido o potencial da parede
    for (int i = 0; i < vecParticula.size(); ++i){
        fx[i] = fx[i] - ((12.0 / pow(this->vecParticula[i].get_loc().x , 13.0)) + (-12.0 / pow((L - this->vecParticula[i].get_loc().x) , 13.0)) );
        fy[i] = fy[i] - ((12.0 / pow(this->vecParticula[i].get_loc().y , 13.0)) + (-12.0 / pow((L - this->vecParticula[i].get_loc().y) , 13.0)) );

    }
}
//==============================================
double ambiente::moment()
{
    double p,px, py;
    px = 0.0;
    py = 0.0;
    p = 0.0;


    for(int i=0; i < vecParticula.size(); ++i)
    {
       px = px + this->vecParticula[i].get_moment().px;
       py = py + this->vecParticula[i].get_moment().py;

    }


    p = sqrt( pow(px,2.0) + pow(py,2.0) );
    return p;
}
//==============================================
void ambiente::anulaMomento()
{

    double px, py, kx, ky;
    px = 0.0;
    py = 0.0;
    kx = 0.0;
    ky = 0.0;



    for(int i=0; i < vecParticula.size(); ++i)  //calcula o momento total na direcao x e y
    {
       px = px + this->vecParticula[i].get_moment().px;
       py = py + this->vecParticula[i].get_moment().py;

    }

    kx = px/vecParticula.size();  //momento por particula, ele devera ser subtraido do momento de cada particula
    ky = py/vecParticula.size();

    for(int i=0; i < vecParticula.size(); ++i)
    {
       px = this->vecParticula[i].get_moment().px - kx;
       py = this->vecParticula[i].get_moment().py - ky;

       this->vecParticula[i].set_Momentx(px);
       this->vecParticula[i].set_Momenty(py);

    }

}
//==============================================
double ambiente::energiaK()
{
    double K,Kx, Ky, K_total;
    K_total = 0.0;
   /*  for(int i=0; i < vecParticula.size(); ++i)
    {
        K.push_back(0);
    }*/

    for(int i=0; i < vecParticula.size(); ++i)
    {
       Kx = this->vecParticula[i].get_moment().px;
       Ky = this->vecParticula[i].get_moment().py;

       K = sqrt( pow(Kx,2.0) + pow(Ky,2.0) );
       K = pow(K,2.0) / 2.0;
       K_total = K_total + K;
    }

    K_total = K_total / vecParticula.size();
    return K_total;
}
//==============================================
double ambiente::energiaU()
{

    double d;
    double u;

    //zera a forca exercida sob cada particula
    for (int i = 0; i < vecParticula.size(); ++i){
        U[i] = 0.0;
        double merda =i;
    }

    for (int i = 0; i < vecParticula.size() - 1; ++i){
        for (int j = (i + 1); j < vecParticula.size(); ++j){
            d = this->distancia(i,j);

            u = C_alpha*( (1.0 / pow(d,12.0)) - (1.0 / pow(d,alpha)) );

            U[i] = U[i] + u;


  //          U[j] = U[j] +u;

        }

    }
    // somando o potencial entre as particulas com o potencial das particulas e paredes
    for (int i = 0; i < vecParticula.size(); ++i){
        U[i] = U[i] + ((1.0 / pow(this->vecParticula[i].get_loc().x , 12.0)) + (1.0 / pow((L - this->vecParticula[i].get_loc().x) , 12.0)) );
        U[i] = U[i] + ((1.0 / pow(this->vecParticula[i].get_loc().y , 12.0)) + (1.0 / pow((L - this->vecParticula[i].get_loc().y) , 12.0)) );

    }

    double U_total;
    U_total = 0.0;
    for (int i = 0; i < vecParticula.size(); ++i){
        U_total = U_total + U[i];
    }
     U_total = ( U_total / vecParticula.size() ) - bo;
    return U_total;
}
//==============================================
double ambiente::energiaM()
{
    double energy;
    energy = this->energiaK() + this->energiaU();
    return energy;
}
