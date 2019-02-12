#include "Numerov.h"

Numerov::Numerov(Potential potential, int nbox) {
    this->potential      = &potential;
    this->nbox           = nbox;
    this->solutionEnergy = 0;
    this->probability    = std::vector<double>(nbox+10);
    this->wavefunction   = std::vector<double>(nbox+10);

    this->wavefunction.insert(this->wavefunction.begin(), 0);
    this->wavefunction.insert(this->wavefunction.begin()+1, 0.1);
    this->wavefunction.insert(this->wavefunction.begin()+2, 0.1);


}

/*! 
    Numerov Algorithm solves f''(x) + v(x)f(x) = 0,
    by considering
    \left( 1+ \frac{h^2}{12} v(x+h) \right) f(x+h) = 2 \left( 1 - \frac{5h^2}{12} v(x) \right) f(x) - \left( 1 + \frac{h^2}{12} v(x-h) \right) f(x-h).
    for the Shroedinger equation v(x) = V(x) - E, where V(x) is the potential and E the eigenenergy
*/
void Numerov::functionSolve(double energy) {
    double c, x;
    std::vector<double> pot = potential->getValues();
    pot.push_back(1.0);

    c = (2.0 * mass / hbar / hbar) * (dx * dx / 12.0);
    try{
        //Build Numerov f(x) solution from left. 
        for (int i = 2; i <= this->nbox; i++) {
            x = (-this->nbox / 2 + i) * dx;
            double &value = this->wavefunction[i];
            double &pot_1 = pot.at(i-1);
            double &pot_2 = pot.at(i-2);
            double &pot_a = pot.at(i);

            double &wave_1 = this->wavefunction.at(i-1);
            double &wave_2 = this->wavefunction.at(i-2);

            value = 2 * (1.0 - (5 * c) * (energy - pot_1 )) * wave_1 - (1.0 + (c) * (energy - pot_2)) * wave_2;
            value /= (1.0 + (c) * (energy - pot_a));  

            //std::cout << i << " " << this->wavefunction.at(i) << std::endl;
    }
    }catch (const std::out_of_range & ex)
    {
        std::cout << "out_of_range Exception Caught :: " << ex.what() << std::endl;
    }
}

/*! 
    \brief a solver of differential equation using Numerov algorithm and selecting non-trivial solutions.
    @param (*potential) is the pointer to the potential function, takes function of 1 variable as input
    @param wavefunction, takes array of @param nbox size as input (for preconditioning)
    and gives the output the normalizedwf solution

    solve_Numerov solves the pointed potential using the Numerov algorithm and
    renormalizing the output wavefunction to 1. To do this it must try the solutions
    for different energies. The natural solution to the second degree differential equation
    is the exponential. But my boundary conditions impose 0 at both beginning and end
    of the wavefunction, so you have to try until you find such solution by finding
    where the exponential solution changes sign.
*/

double Numerov::solve(double e_min, double e_max, double e_step) {
    double c, x, first_step, norm, energy = 0.0;
    int n, sign;

    // scan energies to find when the Numerov solution is = 0 at the right extreme of the box.
    for (n = 0; n < (e_max - e_min) / e_step; n++) {
        energy = e_min + n * e_step;
        this->functionSolve(energy);
        double &last_wavefunction_value = this->wavefunction[this->nbox];

        if ( fabs(last_wavefunction_value) < err ) {
            std::cout << "Solution found" << last_wavefunction_value << std::endl;
            this->solutionEnergy = energy;
            break;
        }

        if (n == 0)
            sign = (last_wavefunction_value > 0) ? 1 : -1;

        // when the sign changes, means that the solution for f[nbox]=0 is in in the middle, thus calls bisection rule.
        if (sign * last_wavefunction_value < 0) {
            std::cout << "Bisection " << last_wavefunction_value << std::endl;
            this->solutionEnergy = this->bisection(energy - e_step, energy + e_step);
            break;
        }
    }

    for (int i = 0; i <= nbox; i++) {
        double &value      = this->wavefunction[i];
        double &prob_value = this->probability[i];
        prob_value = value*value;
    }
    norm = trapezoidalRule(0.0, this->nbox, dx, this->probability);
    for (int i = 0; i <= nbox; i++) {
        double &value = this->wavefunction[i];
        value /= sqrt(norm);
    }

    this->printToFile();
    return this->solutionEnergy;
}

/*! Applies a bisection algorith to the numerov method to find
the energy that gives the non-trivial (non-exponential) solution
with the correct boundary conditions (@param wavefunction[0] == @param wavefunction[@param nbox] == 0)
*/
double Numerov::bisection(double e_min, double e_max) {
    double energy_middle, fx1, fb, fa;
    std::cout.precision(17);

    // The number of iterations that the bisection routine needs can be evaluated in advance
    int itmax = ceil(log2(e_max - e_min) - log2(err)) - 1;

    for (int i = 0; i < itmax; i++) {
        energy_middle = (e_max + e_min) / 2.0;

        this->functionSolve(energy_middle);
        fx1 = this->wavefunction.at(this->nbox);
        
        this->functionSolve(e_max);
        fb = this->wavefunction.at(this->nbox);

        if (std::abs(fx1) < err) {
            return energy_middle;
        }

        if (fb * fx1 < 0.) {
            e_min = energy_middle;
        } else {
            this->functionSolve(e_min);
            fa = this->wavefunction.at(this->nbox);

            if (fa * fx1 < 0.)
                e_max = energy_middle;
        }
    }

    std::cerr << "ERROR: Solution not found using the bisection method, " << this->wavefunction.at(this->nbox) << " > " << err << std::endl;
//    exit(9);
    return energy_middle;
}



void Numerov::printToFile() {
  std::ofstream myfile ("wavefunction.dat");
  if (myfile.is_open())
  {

    for(int i = 0; i < this->wavefunction.size(); i ++){
        myfile << i <<" " << this->wavefunction.at(i)<< std::endl ;
    }

    myfile.close();
  }
}

double Numerov::getSolutionEnergy() {
    return this->solutionEnergy;
}

std::vector<double> Numerov::getWavefunction() {
    return this->wavefunction;
}

std::vector<double> Numerov::getProbability() {
    return this->probability;
}