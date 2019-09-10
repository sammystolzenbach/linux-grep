/*
 *  COMP 15 Proj2 - Gerp
 *  Phase 3
 *
 *  Gerp.h
 *  (UTLN): sstolz01
 *
 *  Header file for Gerp class
 *  
 */

#ifndef GERP_H
#define GERP_H

#include "WordTable.h"
#include "FSTree.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Gerp
{

public:

	Gerp();
	Gerp(string);
	~Gerp();

	void run();

	void wordSearch(string, char, string);

private:

	//private attributes
	struct File
	{
		int startLine;
		int endLine;
		string filename;

		File(string name, int index)
		{
			startLine = index;
			endLine = -1;
			filename = name;
		}
	};

	string dirName;

	vector <File> files;
	vector <string> lines;

	WordTable table;

	void populateFiles();

	void getNames(DirNode *root, string rootName);

	void readFiles();

	void hashFiles();

	int getFile(int index);

	void printLines(vector <int> &vec);

	void createIndex();
};

#endif