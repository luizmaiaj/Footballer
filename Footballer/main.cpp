// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Mayor.h"

using namespace std;

int main(void)
{
	Mayor mayor(POPULATION); // instantiate mayor and allocate a population

	//mayor.loadPopulation();

	mayor.createPopulation();

	return(0);
}
