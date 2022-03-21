/*hash.h*/

//
//
// Jose M. Aguilar
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Project 6 Part 2
// 

#include <string>

using namespace std;

bool isNumeric(string s);
int string2int(string s);
int statIDHash(string theDate, int N);
size_t statAbbrevHash(string statAbbrev, int N);
int tripIDHash(string tripID, int N);
int bikeIDHash(string tripID, int N);