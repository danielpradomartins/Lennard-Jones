#include "particula.h"
#include <stdlib.h>
#include <math.h>


particula::particula(double L, double xi, double yi)
{
    pos_max = L;

    this->loc.x= xi;
    this->loc.y= yi;

    this->moment.px= ((double)rand()/RAND_MAX)*5.0 - 2.5;
    this->moment.py= ((double)rand()/RAND_MAX)*5.0 - 2.5;
}

void particula::set_Posx(double x)
{
    this->loc.x = x;

    // Cond Cont
//    if(this->loc.x>=pos_max -0.5 && this->moment.px>=0.0 ) this->moment.px= -this->moment.px;
//    if(this->loc.x<0.5 && this->moment.px<=0.0 ) this->moment.px= -this->moment.px;
}

void particula::set_Posy(double y)
{
    this->loc.y = y;

    // Cond Cont
//    if(this->loc.y>=pos_max -0.5 && this->moment.py>0.0 ) this->moment.py= -this->moment.py;
//    if(this->loc.y<=0.5 && this->moment.py<0.0 ) this->moment.py= -this->moment.py;
}

void particula::set_Momentx(double px)
{
    this->moment.px = px;
}

void particula::set_Momenty(double py)
{
    this->moment.py = py;
}
