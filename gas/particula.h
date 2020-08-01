#ifndef PARTICULA_H
#define PARTICULA_H
#include <vector>
using namespace std;

// tipo estruturado para guardar a posicão e momento
struct pos
{
    double x;
    double y;
};
typedef pos Tpos;

struct momento
{
    double px;
    double py;
};
typedef momento Tmomento;

class particula
{
public:
    // construtor
    particula(double L, double xi, double yi);

    // localizacao da particula no ambiente
    Tpos get_loc(){return loc;}
    Tmomento get_moment(){return moment;}
    void rodaCiclo();

    void set_Posx(double x);
    void set_Posy(double y);
    void set_Momentx(double px);
    void set_Momenty(double py);

    double pos_max;

private:
    // atributos do particula
    Tpos loc;
    Tmomento moment;
};

#endif // PARTICULA_H
