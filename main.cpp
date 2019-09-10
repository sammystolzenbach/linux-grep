/*
 *  COMP 15 Proj2 - Gerp
 *  Phase 3
 *
 *  main.cpp
 *  (UTLN): sstolz01
 *
 *  Driver file for Gerp
 *  
 */

#include "Gerp.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int usageError();

int main(int argc, char *argv[])
{
	//if they didn't enter exactly 2 args
	if (argc != 2)
		usageError();

	string dir = argv[1];

	Gerp search(dir);

	search.run();
	
	return 0;
}

/* terminates program and prints usage error */
int usageError()
{
	cerr << "Usage:  gerp directory\n";
	cerr << "            where:  directory is a valid directory\n";
    exit(EXIT_FAILURE);
}