/*hash.cpp*/

//
// Jose M. Aguilar
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Project 6 Part 2
// 

#include <iostream>
#include <string>
#include <cctype>  /*isdigit*/
#include <regex>   /*regular expressions*/

#include "hash.h"

using namespace std;

//
// isNumeric
//
// Returns true if the given string is numeric (all digits), false
// if not.  If the string is empty, false is returned since there 
// are no digits present.
//
bool isNumeric(string s)
{
	//
	// A string is numeric if it contains 1 or more digits, so let's
	// use a regular expression to check that.
	//
	// we are using POSIX basic notation for regular expressions, see
	//   https://en.cppreference.com/w/cpp/regex/ecmascript
	// where [:d:] means digit, [[:d:]] means match a single digit.
	// The + means match the preceding sub-expression 1 or more times.
	//
	regex pattern("[[:d:]]+", regex::ECMAScript);

	smatch matchResults;  // provides more details on the match

	if (regex_match(s, matchResults, pattern))
		return true;
	else
		return false;
}

//
// string2int
// 
// Converts a string to an integer, unless string is empty, in
// which case 0 is returned.
// 
int string2int(string s)
{
	if (s == "")
		return 0;
	else
		return stoi(s);
}

//
// removesSpacesAnds
//
// removes all the spaces and &'s in a give string
//
string removeSpacesAnds(string key)
{
	key.erase(remove(key.begin(), key.end(), ' '), key.end()); // removes spaces if found
	key.erase(remove(key.begin(), key.end(), '&'), key.end()); // remove '&' if found

	return key;
}

//
// Hash function for station ID
//
// YYYY up to 10^4 combos
// 
int statIDHash(string statID, int N)
{
	if (!isNumeric(statID))
		return -1;

	else
		return string2int(statID) % N; // index
}

//
// Hash function for station abbrevation
//
// From lecture 31: general hashing, unordered_map
// Hash alg by Dan Bernstien
//
size_t statAbbrevHash(string statAbbrev, int N)
{
	if (isNumeric(statAbbrev))
		return -1;

	statAbbrev = removeSpacesAnds(statAbbrev);

	unsigned long long index = 5381;

	for (char c : statAbbrev)
		index = ((index << 5) + index) + c;

	return index % N;
}

//
// Hash function for trip ID's
//
// TrXXXXXXX(X) up to 10^8 possible combos
//
int tripIDHash(string tripID, int N)
{
	// remove "Tr"
	string Tr = tripID.substr(0, 2);
	tripID.erase(0, 2);

	if ((Tr != "Tr") || (!isNumeric(tripID))) // double check tripID is correct format
	{
		//cout << "** ERROR **" << Tr << " | " << tripID << endl; // for debugging purposes
		return -1;
	}

	else
	{
		//cout << "Index before %: " << string2int(tripID) << endl; // for debugging purposes
		return string2int(tripID) % N; // index is too big for Hash Table so % N
	}
}

//
// Hash function for bike ID's
//
// BXXX(X) up to 10^4
//
int bikeIDHash(string bikeID, int N)
{
	if ((bikeID[0] != 'B') || (!isNumeric(bikeID.substr(1)))) // double check bike ID is valid
	{
		//cout << "** ERROR **" << bikeID[0] << " | " << bikeID.substr(1) << endl; // for debugging purposes
		return -1;
	}

	else
	{
		bikeID.erase(0, 1); // erase B

		return string2int(bikeID) % N; // index
	}
}