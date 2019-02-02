#include <iostream>
#include <string>
#include <BasisManager.h>
#include <Potential.h>
#include <Schroedinger.h>


void main(int argc, char **argv) {

	BasisManager::Builder builder = BasisManager::Builder();
	BasisManager *manager = BasisManager::getInstance();
	Base base;
	std::vector<Base> basis;

	// Setup the properties of a new base, using the basis builder
	builder.addContinuous(0.1, 1000);

	// Getting a base object, building the previous specified in the builder.
	base = builder.build();

	// Add the base in the basis list, using the basis manager
	manager->addBase(base);

	// Get a list of basis from the basis list
	basis = manager->getBasisList();

}
