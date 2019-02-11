#include <iostream>
#include <string>
#include <BasisManager.h>
#include <Base.h>
#include <Potential.h>
#include <Numerov.h>


int main(int argc, char **argv) {

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

    return 0;
}

