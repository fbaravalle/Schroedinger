#include "Potential.h"

Potential::Potential(Base base, std::string type, double k, double width, double height)
{
    this->base     = base; 
    this->k        = k;
    this->width    = width;
    this->height   = height;
    this->type     = type;

    // The following is hardcoded to work with a base having one continuous dimension. 
    this->v        = std::vector<double>(base.getContinuous().at(0).getCoords().size);

    if(type.compare("box potential") == 0 || type.compare("box") == 0 || type.compare("0") == 0)
        this->box_potential();

    else if(type.compare("harmonic oscillator") == 0 || type.compare("ho") == 0 ||  type.compare("1") == 0)
        this->ho_potential();

    else if(type.compare("finite well potential") == 0 || type.compare("well") == 0 || type.compare("2") == 0 )
        this->finite_well_potential();

    else {
        throw std::invalid_argument("Wrong potential type ("+type+") or initialization meaningless!");
    }
}

void Potential::ho_potential()
{
    for(std::vector<int>::size_type i = 0; i < x.size(); i++)
        this->v[i] = this->x[i] * this->x[i] * this->k;
}

void Potential::box_potential()
{
    std::fill(this->v.begin(), this->v.end(), 0.0);
}

void Potential::finite_well_potential()
{
    for(std::vector<int>::size_type i = 0; i < x.size(); i++)
        this->v[i] = (this->x[i] > -this->width/2.0 && this->x[i] < this->width/2.0) ? 0.0 : this->height;
}

std::vector<double> Potential::getValues()
{
    return this->v;
}
