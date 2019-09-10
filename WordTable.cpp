/*
 *  COMP 15 Proj2 - Gerp
 *  Phase 3
 *
 *  WordTable.cpp
 *  (UTLN): sstolz01
 *
 *  Implementation for WordTable class
 *  
 */
#include <iostream>
#include "WordTable.h"
#include <string>
#include <vector>
#include <stack>
#include <stdio.h>
#include <functional>

using namespace std;

const double LOAD_FACTOR = 0.7;

/* defaul constructor */
WordTable::WordTable()
{
	currentSize = INIT_SIZE;
	numItems = 0;

	wordTable = new WordNode *[currentSize];

	for (int i = 0; i < currentSize; i++)
		wordTable[i] = NULL;
}

/* destructor */
WordTable::~WordTable()
{
	for (int i = 0; i < currentSize; i++)
		deleteList(wordTable[i]);

	delete [] wordTable;
}

/* deleteList
 * Parameters: 
 *			pointer to WordNode
 * deletes list held in bucket of hashtable
 */
void WordTable::deleteList(WordNode *node)
{
	while (node != NULL)
    {
         WordNode *temp = node->next;
         delete node;
         node = temp;
    }
    node = NULL;
}

/* std hash function */
static size_t hasher(string key)
{
	return std::hash<string>{}(key);
}

/* insert
 * Parameters: 
 *			string key representing word
 *			char for case sensitivity
 *			int for line index word is from 
 * Inserts line number if word is already in table,
 * or makes new node if it does not exist yet
 */
void WordTable::insert(string key, char sen, int line)
{
	//check if load factor is exceeded
	if ((double)numItems/(double)currentSize > LOAD_FACTOR)
		expand();
		
	int i = hasher(key) % currentSize;

	bool exists = inserter(wordTable[i], key, sen, line);

	//if the word is not in the table, make new WordNode
	if (!exists)
	{
		WordNode *temp = wordTable[i];
		wordTable[i] = new WordNode(key, sen, line, temp);
		numItems++;
	}
}

/* inserter
 * Parameters: 
 *			pointer to word node
 *			string key representing word
 *			char for case sensitivity
 *			int for line index word is from 
 * recursively traverses list and and returns
 * false if word doesn't exist or inserts line number
 * into respective vector if it does exist
 */
bool WordTable::inserter(WordNode *node, string key, char sen, int line)
{
	if (node == NULL)
		return false;
	else if (node->key == key)
	{
		if (sen == 's')
		{
			//make sure the line inserted is not a duplicate
			size_t size = node->sensitive.size();
			if (size == 0 || node->sensitive[size-1] != line)
				node->sensitive.push_back(line);
		}
		else
		{
			//make sure line inserted is not a duplicate
			size_t size = node->insensitive.size();
			if (size == 0 || node->insensitive[size-1] != line)
				node->insensitive.push_back(line);
		}
		return true;
	}
	else
		return inserter(node->next, key, sen, line);
}

/* print
 * takes no parameters, returns nothing
 * prints the hash table
 */
void WordTable::print()
{
	cout << "printing Word Table:\n";
	cerr << "currentSize: " << currentSize << endl;

	cerr << "Number of items in table: " << numItems << endl;

	for (int i = 0; i < currentSize; i++)
	{
		WordNode *curr = wordTable[i];

		if (curr != NULL)
			cout << "|index "<< i << " |\n";
		while (curr != NULL)
		{
			printWordNode(curr);
			curr = curr->next;
		}
	}
}

/* printWordNode
 * takes node pointer as parameter
 * prints individual linked list in hash table
 */
void WordTable::printWordNode(WordNode *node)
{
	cout << "[key: " << node->key << endl;
	
	cout << "sensitive indexes: { ";
	for (size_t i = 0; i < node->sensitive.size(); i++)
		cout << node->sensitive[i] << " ";
	cout << "}";

	cout << "insensitive indexes { ";
	for (size_t i = 0; i < node->insensitive.size(); i++)
		cout << node->insensitive[i] << " ";
	cout << "}\n" << "]" << endl;
}

/* findWord
 * Parameters: 
 *			string data 
 *			char for sensitivity
 * Returns: 
 *		pointer to an int vector, or NULL
 * finds the line vector of a word if it exists
 * and returns it, if it does not exist in the table,
 * returns NULL
 */
vector<int> *WordTable::findWord(string data, char sen)
{
	int i = hasher(data) % currentSize;
	vector<int> *ptr = NULL;

	WordNode *curr = wordTable[i];
	while (curr != NULL)
	{
		if (curr->key == data)
		{
			if (sen == 's' && curr->sensitive.size() > 0)
				ptr = &curr->sensitive;
			else if (sen == 'i' && curr->insensitive.size() > 0)
				ptr = &curr->insensitive;
		}
		curr = curr->next;
	}
	return ptr;
}

/* expand()
 * Takes no parameters, returns nothing.
 * deletes current word table and creates
 * a new one of bigger by a factor of 3
 * rehashes all data to new table
 */
void WordTable::expand()
{
	//use stack to store data already in table
	//while the table is deleted and resized
	stack <string> keys;
	stack < vector <int> > iVec;
	stack < vector <int> > sVec;

	//store data and delete table
	for (int i = 0; i < currentSize; i++)
	{
		WordNode *curr = wordTable[i];

		while (curr != NULL)
		{
			WordNode *temp = curr->next;
			
			keys.push(curr->key);
			iVec.push(curr->insensitive);
			sVec.push(curr->sensitive);

         	delete curr;
         	curr = temp;
		}
	}
	delete [] wordTable;

	currentSize = (currentSize * 3);
	wordTable = new WordNode *[currentSize];

	for (int i = 0; i < currentSize; i++)
		wordTable[i] = NULL;

	//put back data into the new table
	while (!keys.empty())
	{
		place(keys.top(), sVec.top(), iVec.top());
		keys.pop();
		sVec.pop();
		iVec.pop();
	}
}

/* place
 * Parameters: 
 *			string word (key for hash table)
 *			int vector by reference
 *			int vector by reference
 * Helper function for expand, places data back into
 * new table of bigger size by rehashing key and
 * copying over value
 */
void WordTable::place(string word, vector<int> &sen, std::vector<int> &insen)
{
	int i = hasher(word) % currentSize;

	WordNode *temp = wordTable[i];
	wordTable[i] = new WordNode(word, sen, insen);
	wordTable[i]->next = temp;
}

/* getter for numItems, used in debugging */
int WordTable::getNumItems()
{
	return numItems;
}
