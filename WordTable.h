/*
 *  COMP 15 Proj2 - Gerp
 *  Phase 3
 *
 *  WordTable.h
 *  (UTLN): sstolz01
 *
 *  Header file for WordTable class
 *  
 */

#ifndef WORDTABLE_H
#define WORDTABLE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class WordTable
{

	public:

		WordTable();
		~WordTable();

		void insert(string key, char sen, int line);

		void print();

		int getNumItems();

		vector<int> *findWord(string data, char sen);

	private:


		struct WordNode
		{	
			string key;

			vector <int> insensitive;
			vector <int> sensitive;

			WordNode *next;

			//constructor used when inserting into hash table
			WordNode(string word, char sen, int line, WordNode *node)
			{
				key = word;
				if (sen == 's')
					sensitive.push_back(line);
				else
					insensitive.push_back(line);

				next = node;
			}

			//constructor used when copying nodes after expand
			WordNode(string word, vector<int> &sen, std::vector<int> &insen)
			{
				key = word;
				sensitive = sen;
				insensitive = insen;
			}

		};

		static const int INIT_SIZE = 269;
		int currentSize;
		int numItems;

		WordNode **wordTable;

		bool inserter(WordNode *, string, char, int);

		void expand();
		
		void deleteList(WordNode *node);

		int listLength(WordNode *node);

		void printWordNode (WordNode *node);

		void place(string word, vector<int> &sen, vector<int> &insen);

};

#endif

