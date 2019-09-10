/*
 *  COMP 15 Proj2 - Gerp
 *  Phase 2
 *
 *  stringProcessing.cpp
 *  (UTLN): sstolz01
 *
 *  implementation of stripNonAlphaNum function
 *  tested in testStringProcessing.cpp
 *  
 */

#include "stringProcessing.h"
#include <iostream>
#include <string>

using namespace std;


/* 
* stripNonAlphaNum
* Parameters: 
*		word: string to be stripped
* Effects: returns string given but without the
*			surrounding non-alphanumeric characters
*/
string stripNonAlphaNum(string word)
{
	int start = -1, end = -1;
	int len = word.length(); 

	for (int i = 0; i < len; i++)
	if (isalnum(word[i]))
	{
		start = i;
		break;
	}

	for (int i = (len - 1); i >= 0; i--)
	if (isalnum(word[i]))
	{
		end = i;
		break;
	}

	if (end > -1)
		return (word.substr(start, (end - start + 1)));
	else 
		return "";

}