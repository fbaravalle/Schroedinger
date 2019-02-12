#include <iostream>
#include <string>
#include <BasisManager.h>
#include <Base.h>
#include <Potential.h>
#include <Numerov.h>

void box_potential_example() {
	unsigned int nbox = 1000;
	double mesh       = dx;
	double k          = 0.0;
	double height     = 0.0;
	double width      = 0.0;
	double energy     = 0.0;
	std::vector<double> wavefunction;

	// Initialize the base using preset and parameters
	BasisManager::Builder baseBuilder;
	Base base = baseBuilder.build(Base::basePreset::Cartesian, 1, mesh, nbox);
	
	Potential::Builder potentialBuilder(base);
    Potential V = potentialBuilder.setType(Potential::PotentialType::BOX_POTENTIAL)
								  .setK(k)
								  .setHeight(height)
								  .setWidth(width)
								  .build();
    

    Numerov solver = Numerov(V, nbox);
    solver.solve(0.0, 2.0, 0.01);

    wavefunction  = solver.getWavefunction();
    energy        = solver.getSolutionEnergy();

	// Print everything 
	std::cout << solver;

	// Save to file wavefunction and probability
	solver.printToFile();
}

void finite_well_example() {
	unsigned int nbox = 1000;
	double mesh       = dx;
	double k          = 0.0;
	double height     = 5.0;
	double width      = 7.0;
	double energy     = 0.0;
	std::vector<double> wavefunction;

	// Initialize the base using a ContinuousInitializer
	ContinuousInitializer x_ini(mesh,nbox);
	BasisManager::Builder b;
	Base base = b.build(x_ini);
	
	Potential::Builder potentialBuilder(base);
    Potential V = potentialBuilder.setType(Potential::PotentialType::FINITE_WELL_POTENTIAL)
								  .setK(k)
								  .setHeight(height)
								  .setWidth(width)
								  .build();
    

    Numerov solver = Numerov(V, nbox);
    solver.solve(0.0, 2.0, 0.01);

    wavefunction  = solver.getWavefunction();
    energy        = solver.getSolutionEnergy();

	// Print everything 
	std::cout << solver;

	// Save to file wavefunction and probability
	solver.printToFile();
}

void harmonic_oscillator_example() {
	unsigned int nbox = 1000;
	double mesh       = dx;
	double k          = 1.0;
	double height     = 0.0;
	double width      = 0.0;
	double energy     = 0.0;
	std::vector<double> wavefunction;

	BasisManager::Builder baseBuilder;
	Base base = baseBuilder.build(Base::basePreset::Cartesian, 1, mesh, nbox);
	
	Potential::Builder potentialBuilder(base);
    Potential V = potentialBuilder.setType(Potential::PotentialType::HARMONIC_OSCILLATOR)
								  .setK(k)
								  .setHeight(height)
								  .setWidth(width)
								  .build();
    

    Numerov solver = Numerov(V, nbox);
    solver.solve(0.0, 2.0, 0.01);

    wavefunction  = solver.getWavefunction();
    energy        = solver.getSolutionEnergy();

	// Print everything 
	std::cout << solver;

	// Save to file wavefunction and probability
	solver.printToFile();
}

void custom_workflow() {
	BasisManager::Builder builder = BasisManager::Builder();
	BasisManager *manager = BasisManager::getInstance();
	Base base;
	std::vector<Base> basis;

	// Setup the properties of a new base, using the basis builder
	int dimension = 2;

    // Specify parameters of first dimension
	double start_1 = 0;
	double end_1   = 1000;
	double mesh_1  = 2;

    // Specify parameters of second dimension
    double start_2 = 0;
	double end_2   = 1000;
	double mesh_2  = 2;

    // Add the continuous representation to the base, using the basis builder
    builder.addContinuous(start_1, end_2, mesh_2);
    builder.addContinuous(start_2, end_2, mesh_2);

	// Getting a base object, building with the previous specified parameters in the basis builder.
	// (Specify that the base is Cartesian, passing also the dimension)
	base = builder.build(Base::basePreset::Cartesian, dimension);

	// Add the base in the basis list, using the basis manager
	manager->addBase(base);

	// Get a list of basis from the basis list (Not mandatory)
	basis = manager->getBasisList();

    // Print basis values for each dimension
	std::cout << base;
}

int main(int argc, char **argv) {
	int c = 0;

	std::cout << "Choose: " << std::endl;
	std::cout << "1) Harmonic oscillator (example)" << std::endl;
	std::cout << "2) Box (example)" << std::endl;
	std::cout << "3) Finite well1 (example)" << std::endl;
	std::cout << "4) Custom (step-by-step configuration)" << std::endl;
	std::cout << "\nInsert: ";
	std::cin >> c;

	switch(c){
		case 1:
			harmonic_oscillator_example();
			break;
		case 2:
			box_potential_example();
			break;
		case 3:
			finite_well_example();
			break;
		case 4:
			std::cout << "Not ready yet" << std::endl;
			break;
		default:
			std::cout << "Not a valid option" << std::endl;
			break;
	}

    return 0;
}

