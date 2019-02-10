#include <iostream>
#include <string>
#include <BasisManager.h>
#include <Base.h>
#include <Potential.h>
#include <Schroedinger.h>


void main(int argc, char **argv) {

	BasisManager::Builder builder = BasisManager::Builder();
	BasisManager *manager = BasisManager::getInstance();
	Base base;
	std::vector<Base> basis;

	// Setup the properties of a new base, using the basis builder
	int dimension = 2;
	int start = 0;
	int end   = 1000;
	int mesh  = 2;
	for (int i = 0; i < dimension; i++) {
	    builder.addContinuous(start, end, mesh);
	}


	// Getting a base object, building the previous specified in the builder.
	base = builder.build(Base::basePreset::Cartesian, dimension);

	// Add the base in the basis list, using the basis manager
	manager->addBase(base);

	// Get a list of basis from the basis list (Not mandatory)
	basis = manager->getBasisList();

	
}
