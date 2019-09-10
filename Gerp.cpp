/*
 *  COMP 15 Proj2 - Gerp
 *  Phase 3
 *
 *  Gerp.cpp
 *  (UTLN): sstolz01
 *
 *  implementation file for Gerp class
 *  
 */

#include "Gerp.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "FSTree.h"
#include "stringProcessing.h"

using namespace std;


/* 
 * Constructor
 */
Gerp::Gerp()
{
	//nothing to do here
}

/* 
 * Paramaterized constructor
 * takes directory from main.cpp
 * and initializes dirName
 */
Gerp::Gerp(string directory)
{
	dirName = directory;
}

/* 
 *Destructor 
 */
Gerp::~Gerp()
{
	//nothing to do here yet
}

/* createIndex
 * takes no parameters, returns nothing
 * creates index using functions that
 * populate files vector, populate
 * lines vector, and hash the lines
 */
void Gerp::createIndex()
{
	populateFiles();
	readFiles();
	hashFiles();
}


/* run
 * takes no parameters, returns nothing
 * creates takes user's query in form of
 * a string, until end of input
 */
void Gerp::run()
{
	createIndex();

	while (!cin.eof())
	{
		string word = "";
		cout << "Query? ";
		char sen = 's';
		cin >> word;

		//set sensitivity
		if (word == "@i" || word == "@insensitive")
		{
			cin >> word;
			sen = 'i';
		}
		if (word == "@quit" || word == "@q")
			break;

		//strip word
		word = stripNonAlphaNum(word);
		string original = word;

		//find the insensitive version using tolower
		if (sen == 'i')
			for (size_t i = 0; i < word.length(); i++)
				word[i] = tolower(word[i]);

		if (word.length() > 0)
			wordSearch(word, sen, original);

		else if (cin.eof())
			break;

		//if it is an empty string
		else if (word.length() == 0)
		{
			cout << "\"\" Not Found."; 
			if (sen == 's')
				cout << " Try with @insensitive or @i.\n";
			else 
				cout << endl;
		}
	}
	cout << endl; cout << "Goodbye! Thank you and have a nice day.\n";
}

/* PopulateFiles
 * takes no parameters, returns nothing
 * creates a FSTree and uses getNames
 * to populate the files
 */
void Gerp::populateFiles()
{
	FSTree tree(dirName);
	getNames(tree.getRoot(), dirName);
}

/* getNames
 * Parameters: DirNode pointer
 * 			   string for root directory
 * recursively traverses FSTtree
 * and poplates files vecto
 */
void Gerp::getNames(DirNode *root, string rootName)
{
	//if there are subdirectorings, recurse to their files
	if (root->numSubDirs() > 0)
		for (int i = 0; i < root->numSubDirs(); i++)
		{
			//concatenate name of directories
			string DirName = rootName + "/" + root->getSubDir(i)->getName();
			getNames(root->getSubDir(i), DirName);
		}

	//get names of the files
	if (root->numFiles() > 0)
		for (int i = 0; i < root->numFiles(); i++)
		{
			//concatenate name of directories with file
			string fileName = (rootName + "/" + root->getFile(i));
			//put into sequence of file names
			File newFile(fileName, -1);
			files.push_back(newFile);
		}
}

/* readFiles()
 * takes no parameters, returns nothing
 * Opens each file in files vector and
 * adds its lines to the lines vector
 */
void Gerp::readFiles()
{
	ifstream infile;
	int index = 0;

	//iterate through files vector, store each
	//line as a string in lines vector
	for (size_t i = 0; i < files.size(); i++)
	{
		infile.open(files[i].filename);
		files[i].startLine = index;

		while (!infile.eof())
		{
			string temp;
			getline(infile, temp);
			lines.push_back(temp);
			index++;
		}

		files[i].endLine = (index - 1);
	
		infile.close();
	}
}

/* hashFiles()
 * takes no parameters, returns nothing
 * makes each line in the lines vector
 * into a stream and inserts its words
 * into WordTable hash table
 */
void Gerp::hashFiles()
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		//make line into string stream
		istringstream input;
		input.str(lines[i]);

		while (!input.eof())
		{
			string word;
			input >> word;
			word = stripNonAlphaNum(word);

			if (word.length() != 0)
			{
				//insert insensitive and sensitive case
				table.insert(word, 's', i);

				for (size_t i = 0; i < word.length(); i++)
					word[i] = tolower(word[i]);

				table.insert(word, 'i', i);
			}
		}
	}
}

/* wordSearch
 * Parameters: 
 *			string for word searching for
 *			char for case sensitivity
 *			string to keep the unparsed version
 * uses WordTable function findword() to check
 * for the word in the directory, and prints
 * outs its lines if it exists
 */
void Gerp::wordSearch(string word, char sen, string original)
{
	vector<int> *ptr = table.findWord(word, sen);

	if (ptr == NULL)
	{
		cout << original << " Not Found.";
		if (sen == 's')
			cout << " Try with @insensitive or @i.\n";
		else
			cout << endl;
	}
	else
		printLines(*ptr);
}

/* wordSearch
 * Parameters: 
 *			int vector by reference representing
 *			the lines a queried word is found on 
 * prints filename with path, the line number,
 * and the line the word is found in.
 */
void Gerp::printLines(vector <int> &vec)
{
	//FileNameWithPath:LineNum: Line
	for (size_t i = 0; i < vec.size(); i++)
	{
		int file = getFile(vec[i]);
		if (file == -1)
				cout << "ERROR: COULDNT FIND FILE.\n";
		else
		{
			cout << files[file].filename << ":" 
				 << (vec[i] - files[file].startLine + 1)
				 << ": " << lines[vec[i]];
			cout << endl;
		}
	}
}

/* getFile()
 * Parameters: 
 *			int representing index word is found on
 * Returns: 
 *			int representing the index of the file
 * 			it is contained in, used for printing
 */
int Gerp::getFile(int index)
{
	for (size_t i = 0; i < files.size(); i++)
	{
		if (index >= files[i].startLine)
			if (index <= files[i].endLine)
				return i;
	}
	return -1;
}