/*main.cpp*/

//
// <<Jose M. Aguilar>>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Lab Week 11: hashing of dates and lottery numbers
// 

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "hash.h"

using namespace std;

struct lotteryData
{
	string drawDate;
	string winningNums;
	string megaBall;
	string multiplier;
	
	lotteryData()
	{
		drawDate = "";
		winningNums = "";
		megaBall  = "";
		multiplier = "";
	}
};

bool inputData (string fileName, lotteryData* hashTable)
{
	fstream infile(fileName);
	
	//could not open file
	if (!infile.good())
	{
		cout << "** ERROR: Could not open '" << fileName <<"' **" << endl;
		return false;
	}
	
	//file was opened
	cout << "Reading " << fileName << endl;
	string line;
	int lineCount = 0; 
	
	getline(infile, line); //first line is the header (can be discarded)
	
	while(getline(infile, line))
	{
		stringstream token(line);
		
		string date, winning, mega, mult;
		
		getline(token, date, ',');
		getline(token, winning, ',');
		getline(token, mega, ',');
		getline(token, mult, ',');
		
		//cout << "The Date: " << date << " The Winning Numbers: " << winning << " The Mega Ball: " << mega << " The Multiplier: " << mult << endl; // for debugging purposes
		
		lotteryData& LD = hashTable[Hash(date)];
		
		LD.drawDate = date;
		LD.winningNums = winning;
		LD.megaBall = mega;
		LD.multiplier = mult;
		
		lineCount++; 
	} 
	
	if (lineCount == 0)
		return false;
	
	cout << "# data lines: " << lineCount << endl;
	cout << endl;
	return true;
}

int main()
{
	// TODO: Allocate space for hash table
	int SIZE = 37200;
	lotteryData* hashTable = new lotteryData[SIZE];
	
	string fileName = "mega_millions.csv";
		
	cout << "** Mega millions lottery data **" << endl;
	cout << endl;
	
	//input the data
	bool success = inputData(fileName, hashTable);
	
	if(!success)
	{
		cout << "** ERROR: No data inputed **" << endl;
		return 0;
	}
	
	string theDate;
	
	cout << "Enter a date 'mm/dd/yyyy' or # to exit> ";
	cin >> theDate;
	
	while (theDate != "#")
	{
		int index = Hash(theDate);
		
		if (index == -1)
		{
			cout << "Invalid date" << endl;
			cout << endl;
		}
		
		else if (hashTable[index].drawDate == "")
		{
			cout << "Sorry, no data for this date" << endl;
			cout << endl;
		}
		
		else
		{
			cout << "Winning numbers: " << hashTable[index].winningNums << endl;
			cout << "Mega ball: " << hashTable[index].megaBall << endl;
			cout << "Multiplier: " << hashTable[index].multiplier << endl;
			cout << endl;
		}
		
		cout << "Enter a date 'mm/dd/yyyy' or # to exit> ";
		cin >> theDate;
	}
	
	
	//
	// done!
	// 
	cout << endl;
	cout << "** Done **" << endl;
	
	return 0;
}
