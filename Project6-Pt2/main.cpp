/*main.cpp*/

//
// Jose M. Aguilar (jaguil61)
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Project 6 Part 2
// 

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <utility> // pair<>
#include <vector>
#include <algorithm> // sort()


#include "hash.h"
#include "hashmap.h"
#include "util.h"

using namespace std;


//
// Stations
// 
struct Stations
{
	string ID;
	string Abbrev;
	string FullName;
	double Latitude;
	double Longitude;
	string Capacity; 
	string OnlineDate;
	
	Stations()
	{
		ID = "";
		Abbrev = "";
		FullName = "";
		Latitude = 0;
		Longitude = 0;
		Capacity = "";
		OnlineDate = "";
	}
};

//
// Trips
//
struct Trips
{
	string TripID;
	string StartTime;
	string BikeID;
	string Duration;
	string From;
	string To;
	string Identifies;
	string BirthYear;

	Trips()
	{
		TripID = "";
		StartTime = "";
		BikeID = "";
		Duration = "";
		From = "";
		To = "";
		Identifies = "";
		BirthYear = "";
	}
};

//
// prioratize (functor)
// 
// sorts data based on value or by key if values are the same
//
class prioritize
{
public:
	bool operator()(const pair<int, double>& p1, const pair <int, double>& p2) const
	{
		if (p1.second != p2.second)
			return p1.second < p2.second;

		else // p1 and p2 have the same value so sort by key
			return p1.first < p2.first; 
	}

};

//
// timeConversion
//
// given a string of seconds convert to hours | minutes | seconds
// updates string passed to be in ?? hours ?? minutes ?? seconds
//
void timeConversion(string& theDuration)
{
	int durationSec = stoi(theDuration);

	int hour, min, sec;

	hour = min = sec = 0;

	hour = durationSec / 3600; // 60 * 60 = 3600 sec in an hour
	durationSec %= 3600;
	min = durationSec / 60;
	durationSec %= 60;
	sec = durationSec;

	string strHour, strMin, strSec;

	strHour = to_string(hour);
	strMin = to_string(min);
	strSec = to_string(sec);

	if ((hour > 0) && (min > 0))
		theDuration = strHour + " hours, " + strMin + " minutes, " + strSec + " seconds";

	else if ((hour > 0) && (min == 0))
		theDuration = strHour + " hours, " + strSec + " seconds";

	else if ((hour == 0) && (min > 0))
		theDuration = strMin + " minutes, " + strSec + " seconds";

	else // hour == 0 && min == 0
		theDuration = strSec + " seconds";
}

//
// parseNearbyCoor
//
// parses the nearby coordinates for findNearby function
void parseNearbyCoor(string command, string& strLat, string& strLong, string& strDis)
{
	size_t found = command.find(" ");

	command.erase(0, found + 1);

	//cout << "|" << command << "|" << endl; // for debugging purposes

	found = command.find(" ");

	strLat = command.substr(0, found);

	command.erase(0, found + 1);

	found = command.find(" ");

	strLong = command.substr(0, found);

	command.erase(0, found + 1);

	strDis = command;
}

//
// loopHashTable
// 
// decides if the distance between two points in the hashtable are <= the distance given
//
void loopHashTable(string key, double latitude, double longitude, double distance, hashmap<string, Stations>& stationsByID, vector<pair<int, double>>& stats)
{
	Stations statInfo;

	bool succes = stationsByID.search(key, statInfo, statIDHash);

	if (!succes) // "do nothing"
	{
		//cout << "Not found" << endl; // for debugging purposes
	}

	else // found
	{
		if (distBetween2Points(latitude, longitude, statInfo.Latitude, statInfo.Longitude) <= distance)
		{
			int key = string2int(statInfo.ID); //convert to int for comparisons sake

			//cout << "Found Station: " << index << endl; // for debugging purposes

			stats.push_back(make_pair(key, distBetween2Points(latitude, longitude, statInfo.Latitude, statInfo.Longitude))); // push into vector to sort later (stationID, distance)
		}

		else // not nearby
			; // do nothing
	}

	return;
}

//
// statInputData
// 
// Given a filename inputs that data into the given hash table. For station file
// 
bool statInputData(int& numStations, string filename, hashmap<string, Stations>& stationsByID, hashmap<string, string>& stationsByAbbrev)
{
	ifstream  infile(filename);
	
	//cout << "Reading " << filename << endl;
	
	if (!infile.good())
	{
		cout << "**Error: unable to open '" << filename << "'..." << endl;
		return false;
	}
	
	// file is open, start processing:
	string line;
	
	getline(infile, line);  // input and discard first row --- header row
	
	int count = 0;
	
	while (getline(infile, line))
	{
		stringstream s(line);
		
		string theID, theAbbrev, theFullName, theLatitude, theLongitude, theCapacity, theOnlineDate;
		
		//read in values
		getline(s, theID, ',');  
		getline(s, theAbbrev, ',');   
		getline(s, theFullName, ',');
		getline(s, theLatitude, ',');
		getline(s, theLongitude, ',');
		getline(s, theCapacity, ',');
		getline(s, theOnlineDate, ',');
		
		//
		// store into hash table
		// 
		Stations statInfo;
		
		statInfo.ID = theID;
		statInfo.Abbrev = theAbbrev;
		statInfo.FullName = theFullName;
		statInfo.Latitude = stod(theLatitude);
		statInfo.Longitude = stod(theLongitude);
		statInfo.Capacity = theCapacity;
		statInfo.OnlineDate = theOnlineDate;

		stationsByID.insert(theID, statInfo, statIDHash);
		stationsByAbbrev.insert(theAbbrev, theID, statAbbrevHash);

		numStations++;
	}
	
	return true;  // we have data to be processed:
}

//
// tripInputData
// 
// Given a filename inputs that data into the given hash table. For trip file
// 
bool tripInputData(int& numTrips, int& numBikes, string filename, hashmap<string, Trips>& tripsByID, hashmap<string, int>& bikesByID)
{
	ifstream  infile(filename);

	//cout << "Reading " << filename << endl;

	if (!infile.good())
	{
		cout << "**Error: unable to open '" << filename << "'..." << endl;
		return false;
	}

	// file is open, start processing:
	string line;

	getline(infile, line);  // input and discard first row --- header row

	int count = 0;

	while (getline(infile, line))
	{
		stringstream s(line);

		string theID, theStartTime, theStopTime, theBikeID, theDuration, theFrom, theTo, theIdentifier, theBirthYear;

		//read in values
		getline(s, theID, ',');
		getline(s, theStartTime, ',');
		getline(s, theStopTime, ',');
		getline(s, theBikeID, ',');
		getline(s, theDuration, ',');
		getline(s, theFrom, ',');
		getline(s, theTo, ',');
		getline(s, theIdentifier, ',');
		getline(s, theBirthYear, ',');

		// do some math for the duration
		timeConversion(theDuration);

		//
		// store into hash table
		// 
		Trips tripInfo;

		tripInfo.TripID = theID;
		tripInfo.StartTime = theStartTime;
		tripInfo.BikeID = theBikeID;
		tripInfo.Duration = theDuration;
		tripInfo.From = theFrom;
		tripInfo.To = theTo;
		tripInfo.Identifies = theIdentifier;
		tripInfo.BirthYear = theBirthYear;

		tripsByID.insert(theID, tripInfo, tripIDHash);
		
		int bikeCounter = 1; // no bike can have 0 uses if it was used for a trip

		if (bikesByID.search(theBikeID, bikeCounter, bikeIDHash)) // found another bike with the same bike ID
			bikeCounter++;

		else // did not find another bike so bikeCounter = 1
			numBikes++; // count unique bike

		bikesByID.insert(theBikeID, bikeCounter, bikeIDHash);
		
		numTrips++;
	}

	return true;  // we have data to be processed:
}

//
// searchStations
//
// given a station ID or station abbrevation it will search for a station
//
void searchStations(string command, hashmap<string, Stations>& stationsByID, hashmap<string, string>& stationsByAbbrev)
{
	if (!isNumeric(command)) // command was not a numeric ID
	{
		string id;

		bool foundAbbrev = stationsByAbbrev.search(command, id, statAbbrevHash);

		if (!foundAbbrev) // could not find station by abbrevation
		{
			cout << "Station not found" << endl;

			return;
		}

		else // found Abbrev
			command = id;
	}


	else // do nothing just go on
		;

	//
	// we have a ID, let's look in hash table and
	// see if we have any data:
	// 
	Stations statInfo;

	bool foundID = stationsByID.search(command, statInfo, statIDHash);

	if (!foundID)
	{
		cout << "Station not found" << endl;

		return;
	}

	else
	{
		cout << "Station: " << endl;
		cout << " ID: " << statInfo.ID << endl;
		cout << " Abbrev: " << statInfo.Abbrev << endl;
		cout << " Fullname: " << statInfo.FullName << endl;
		cout << " Location: (" << statInfo.Latitude << ", " << statInfo.Longitude << ")" << endl;
		cout << " Capacity: " << statInfo.Capacity << endl;
		cout << " Online date: " << statInfo.OnlineDate << endl;

		return;
	}

}

//
// search tripID's
//
// given a tripID it will search for that trip info
//
void searchTripID(string command, hashmap<string, Trips>& tripsByID, hashmap<string, Stations>& stationsByID)
{
	// assume valid command was passed (TrXXXXXXX(X))
	Trips tripInfo;

	bool success = tripsByID.search(command, tripInfo, tripIDHash);

	if (!success)
	{
		cout << "Trip not Found" << endl;

		return;
	}

	else
	{
		Stations statInfoFrom;
		Stations statInfoTo;

		// assume station ID's are valid and will be found 
		stationsByID.search(tripInfo.From, statInfoFrom, statIDHash); // search for "From" station
		stationsByID.search(tripInfo.To, statInfoTo, statIDHash); // search for"To" station

		cout << "Trip: " << endl;
		cout << " ID: " << tripInfo.TripID << endl;
		cout << " Starttime: " << tripInfo.StartTime << endl;
		cout << " Bikeid: " << tripInfo.BikeID << endl;
		cout << " Duration: " << tripInfo.Duration << endl;
		cout << " From station: " << statInfoFrom.Abbrev << " (" << tripInfo.From << ")" << endl;
		cout << " To station: " << statInfoTo.Abbrev << " (" << tripInfo.To << ")" << endl;
		cout << " Rider identifies as: " << tripInfo.Identifies << endl;
		cout << " Birthyear: " << tripInfo.BirthYear << endl;

		return;
	}
}

//
// search bikeID's
//
// given a bikeID it will search for that bike info
//
void searchBikeID(string command, hashmap<string, int>& bikesByID)
{
	// assume valid command was passed (BXXX(X))
	int usage = 0;

	bool success = bikesByID.search(command, usage, bikeIDHash);

	if (!success)
	{
		cout << "Bike not found" << endl;

		return;
	}

	else
	{
		cout << "Bike: " << endl;
		cout << " ID: " << command << endl;
		cout << " Usage: " << usage << endl;

		return;
	}
}

// 
// findNearby
//
// "searches *all* stations for those that are <= thespecified distance, which is ingiven in miles"
//
void findNearby(string command, hashmap<string, Stations>& stationsByID)
{
	vector<pair<int, double>> stats;
	vector<string> keyV = stationsByID.keyVector(); // vector of all the keys

	string strLat, strLong, strDis;

	// parse command
	parseNearbyCoor(command, strLat, strLong, strDis);

	double latitude = stod(strLat);
	double longitude = stod(strLong);
	double distance = stod(strDis);

	//cout << "latitude: " << latitude << " | longitude: " << longitude << " | distance: " << distance << endl; // for debugging purposes

	// loop through hashtable and find nearby coordinates
	for (size_t i = 0; i < keyV.size(); i++)
	{
		//cout << "Keys: " << keyV[i] <<endl; // for debugging purposes
		loopHashTable(keyV[i], latitude, longitude, distance, stationsByID, stats);
	}

	sort(stats.begin(), stats.end(), prioritize()); // sorts data based on value or by key if values are the same

	cout << "Stations within " << strDis << " miles of (" << strLat << ", " << strLong << ")" << endl;

	if (stats.empty())
	{
		cout << " none found" << endl;

		return;
	}

	else
	{
		for (size_t i = 0; i < stats.size(); i++)
			cout << " station " << stats[i].first << ": " << stats[i].second << " miles" << endl;

		//stats.clear(); // give some memory back

		return;
	}
}

//
// find similar
//
// "Given a trip from station S to station D, this command searches and counts how many trips are similar to this one"
//
void findSimilar(const int SIZE1, string command, hashmap<string, Stations>& stationsByID, hashmap<string, Trips>& tripsByID)
{
	// parse command

	// find both stations that were used in the trip

	// find nearby stations for both of them

	return;
}

int main()
{
	cout << "** DIVVY analysis program **" << endl;
	cout << endl;

	//
	// Allocate hash table sizes
	//
	const int SIZE1 = 10000;
	const int SIZE2 = 2500000;
	const int SIZE3 = 50000;

	hashmap<string, Stations> stationsByID(SIZE1);  // SIZE: 10K
	hashmap<string, string> stationsByAbbrev(SIZE1); // SIZE: 10K
	hashmap<string, Trips> tripsByID(SIZE2); // SIZE: 2.5M
	hashmap<string, int> bikesByID(SIZE3); // SIZE: 50K | Bike usage

	int numStations = 0;
	int numTrips = 0;
	int numBikes = 0;

	string statFile;
	string tripFile;
	
	//
	// Ask for station/trip file names
	// 
	cout << "Enter stations file> ";
	getline(cin, statFile);
	
	bool statSuccess = statInputData(numStations, statFile, stationsByID, stationsByAbbrev);

	//
	// did we input anything?
	// 
	if (!statSuccess)
	{
		cout << "No data, file not found?" << endl;
		return 0;
	}

	cout << "Enter trips file> ";
	getline(cin, tripFile);
	cout << endl;

	bool tripSuccess = tripInputData(numTrips, numBikes, tripFile, tripsByID, bikesByID);

	//
	// did we input anything?
	// 
	if (!tripSuccess)
	{
		cout << "No data, file not found?" << endl;
		return 0;
	}

	cout << "Reading " << statFile << endl;
	cout << "Reading " << tripFile << endl;
	cout << endl;

	cout << "# of stations: " << numStations << endl;
	cout << "# of trips: " << numTrips << endl;
	cout << "# of bikes: " << numBikes << endl;

	//
	// allow the user to lookup winning numbers by date:
	// 
	string command;

	cout << endl;
	cout << "Please enter a command, help, or #> ";
	getline(cin, command);

	//cout << command << endl; // for debugging purposes

	//
	// user testing:
	//
	while (command != "#")
	{
		string Tr1 = command.substr(0, 2); // used to check if command is a tripID
		string nearby = command.substr(0, 6); // used to check if command is "nearby" command
		string similar = command.substr(0, 7); // used the check if command is a "similar" command

		if (command == "help")
		{
			cout << "Available Commands:\n" 
				<< " Enter a station id (e.g. 341)\n Enter a station abbreviation (e.g Adler)" << endl
				<< " Enter a trip id (e.g. Tr10426561)\n Enter a bike id (e.g. B5218)" << endl
				<< " Nearby stations (e.g nearby 41.86 -86.62 0.5)" << endl
				<< " Similar trips (e.g. similar Tr10424639 0.3)" << endl;;
		}

		else if (Tr1 == "Tr" && isdigit(command[2])) // tripID was entered
		{
			//cout << "entered tripID" << endl; // for debugging purposes
			searchTripID(command, tripsByID, stationsByID);
		}

		else if (command[0] == 'B' && isdigit(command[1])) // bikeID was entered
		{
			//cout << "entered bikeID" << endl; // for debugging purposes
			searchBikeID(command, bikesByID);
		}

		else if (nearby == "nearby") // "nearby" command was entered
		{
			//cout << "nearby command was entered" << endl; // for debugging purposes
			findNearby(command, stationsByID);
		}

		else if (similar == "similar") // "similar" command was entered
		{
			//cout << "similar command was entered" << endl; // for debugging purposes
		}

		else // station ID or station abbrevation
		{
			searchStations(command, stationsByID, stationsByAbbrev);
		}

		cout << endl;
		cout << "Please enter a command, help, or #> ";
		getline(cin, command);

		//cout << command << endl; // for debugging purposes
	}


	//
	// done!
	// 
	//cout << "** Done **" << endl;

	//system("pause");
	return 0;
}
