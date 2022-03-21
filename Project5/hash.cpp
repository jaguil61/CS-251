/*hash.cpp*/

//
// <<Jose M. Aguilar>>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Lab Week 11: hashing of dates and lottery numbers
// 

#include <iostream>
#include <string>
#include <cctype>  /*isdigit*/

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
   if (s.length() == 0)
     return false;
     
   for(char c : s)
   {
      if (!isdigit(c))  // if c is not a digit, it cannot be numeric:
        return false;
   }
   
   // if get here, all chars are digits, so it's numeric:
   return true;
}

//
// Hash
// 
// Hashes the given date "mm-dd-year" and returns
// a unique array index.  If the date falls within
// the range "01/01/2000" .. "12/31/2099", inclusive,
// a valid array index is returned.  If the date 
// falls outside this range, or the date is not 
// formatted mm/dd/yyyy, then -1 is returned. 
// 
// NOTE: the actual date is not validated, e.g. the
// function does not validate that 02/29/2020 is a
// leap year.  Validation is limited to checking that
// the format is mm/dd/yyyy, where 1<=mm<=12, 
// 1<=dd<=31, and 2000<=yy<=2099.
// 
// NOTE: the total # of combinations is 365.25 days 
// per year * 100 total years.  That's 36,525
// possible combinations.  We don't need to be that
// precise.  To simplify things, we'll assume 12
// months per year, 31 days per month, across 100
// years.  That's 12 * 31 * 100 = 37,200 possible
// combinations.  The Hash function is required to
// return a value in the range -1 .. 37199, 
// inclusive.  You can do better if you want, as 
// long as there are no collisions: different dates
// must yield different array indices.
//
int Hash(string theDate)
{
	string strDay, strMonth, strYear;
	int slashOne, slashTwo, day, month, year, bin, subBin, index;
	slashOne = slashTwo = day = month = year = bin = subBin = index = 0;

	// TODO: check length of string
	if (theDate.length() != 10)
	{
		//cout << "** NOT PROPER LENGTH **" << endl; // for debugging purposes
		return -1;
	}

	// TODO: find both "/" in proper location
	else
	{
		//cout << "** PROPER LENGTH **" << endl; // for debugging purposes
		slashOne = theDate.find("/");
		slashTwo = theDate.find("/", slashOne + 1);
	}

	if (slashOne != 2 || slashTwo != 5)
	{
		//cout << "** SLASHES NOT IN PROPER SPOT **" << endl; // for debugging purposes
		return -1;
	}

	// TODO: delete "/"s
	else
	{
		//cout << "** SLASHES IN PROPER SPOT **" << endl; // for debugging purposes
		theDate.erase(2, 1);
		theDate.erase(4, 1);
	}
	// TODO: call isNumeric
	if (!isNumeric(theDate))
	{
		//cout << "** NOT NUMERIC **" << endl; // for debugging purposes
		return -1;
	}

	// TODO: divide the string into month, day, year
	else
	{
		//cout << "** IS NUMERIC **" << endl; // for debugging purposes
		strMonth = theDate.substr(0, 2);
		strDay = theDate.substr(2, 2);
		strYear = theDate.substr(4);

		// TODO: convert to ints
		month = stoi(strMonth);
		//cout << "Month: " << month << endl; // for debugging purposes

		day = stoi(strDay);
		//cout << "Day: " << day << endl; // for debugging purposes

		year = stoi(strYear);
		//cout << "Year: " << year << endl; // for debugging purposes
	}

	// TODO: validate that each int is within range (1<=mm<=12, 1<=dd<=31, and 2000<=yy<=2099)
	
	if ((month < 1) || (month > 12))
	{
		//cout << "** NOT A VALID MONTH RANGE **" << endl; // for debugging purposes
		//cout << "Month: " << month << endl;
		return -1;
	}

	else if ((day < 1) || (day > 31))
	{
		//cout << "** NOT A VALID DAY RANGE **" << endl; // for debugging purposes
		//cout << "Day: " << day << endl;
		return -1;
	}

	else if ((year < 2000) || (year > 2099))
	{
		//cout << "** NOT A VALID YEAR RANGE **" << endl; // for debugging purposes
		//cout << "Year: " << year << endl;
		return -1;
	}

	// TODO: convert to index
	else
	{
		//cout << "** CONVERTING TO INDEX **" << endl; // for debugging purposes
		bin = (year - 2000) * 372;
		subBin = (month - 1) * 31;
		index = (day - 1) + subBin + bin;
	}

	return index;
}

