/* << Jose M. Aguilar (jaguil61) >>
U. of Illinois, Chicago
CS 251: Spring 2020
Spam Filter Program
1.Load a spam list,
2.display the contents of a spam list 
3.check a single email address to see if it’s spam 
4.filter an email list and out put the resulting emails to a file */

#include <iostream>
#include <fstream>
#include <string>
#include "ourvector.h"
using namespace std;

void parseAgain(string command2, string& command3);
void parse(ourvector<string> spamList, string command0, string& command1, string& command2, string& command3);
void loadFile(ourvector<string>& spamList, string command2);
void displaySpam(ourvector<string>& spamList);
void emailParse(string& email, string& emailDomain, string& emailUsername);
bool binarySearch(ourvector<string>& spamList, string findMatch);
void checkEmail(ourvector<string>& spamList, string command2);
void openEmails(ourvector<string>& emailList, string fileName);
void outputWrite(ofstream& outFile, ourvector<string>& spamList, ourvector<string>& emailList, int& counter);
bool filterListCont(ourvector<string>& spamList, string emailLine, int& counter);
void filterList(ourvector<string> spamList, string command0, string command1, string command2, string command3);

//TO DO: Implement 4 functions (Load, display, check, filter)

/*Checks if there's another file after the first parsing (for the filter function)*/
void parseAgain(string command2, string& command3)
{
	size_t space;
	size_t afterSpace;

	space = command2.find(' ');

	afterSpace = space + 1;

	command3 = command2.substr(afterSpace);

	return;
}

/*This parses the command prompt*/
void parse(ourvector<string> spamList, string command0, string& command1, string& command2, string& command3)
{

	size_t space;
	size_t afterSpace;

	space = command0.find(' ');

	if (command0 == "display") //Ensures that the program will still run if the user inputs display without loading a spamlist first
	{
		command1 = "display";
		return;
	}

	else
	{
		afterSpace = space + 1;

		command1 = command0.substr(0, space);
		command2 = command0.substr(afterSpace);

		parseAgain(command2, command3); //Checks if there's a third part the command 
		//cout << "command 1: " << command1 << endl << "command 2: " << command2 << endl; //for debugging

		if (command1 == "#")
			return;

		else if (command1 == command2) //Makes sure there is a second part to the command if there needs to be (Ex: check command without an email after it)
		{
			cout << "**invalid command\n";
			cout << "\nEnter command or # to exit> ";
			getline(cin, command0);

			parse(spamList, command0, command1, command2, command3);
		}

		return;
	}
}

/*This will load a list of spam emails that will then be used to filter out the spam emails from the emails file*/
void loadFile(ourvector<string>& spamList, string command2)
{
	//TO DO: clear vector whenever this function is called
	spamList.clear();

	//TO DO: Open file and put everything into vector
	ifstream txtFile;
	string fileCon; //File contents

	txtFile.open(command2);

	if (!txtFile.is_open())
	{
		cout << "**Error, unable to open '" << command2 << "'\n";
		return;
	}

	else
	{
		cout << "Loading '" << command2 << "'\n";

		while (getline(txtFile, fileCon))
		{
			spamList.push_back(fileCon);
		}

		cout << "# of spam entries: " << spamList.size() << endl;
	}
	
	//cout << "File closing...\n"; //for debugging
	txtFile.close();

	return;
}

/*This will display the vector of spam emails*/
void displaySpam(ourvector<string>& spamList)
{
	//TO DO: create a for loop that prints the vector
	for (int i = 0; i < spamList.size(); i++)
	{
		cout << spamList[i] << endl;
	}

	return;
}

/*Parses an email*/
void emailParse(string& email, string& emailDomain, string& emailUsername)
{
	size_t at;
	size_t afterAt;

	at = email.find('@');
	afterAt = at + 1;

	emailUsername = email.substr(0, at);
	emailDomain = email.substr(afterAt);

	return;
}

/*Will perform a binary search to try a find a matching domain or username*/
bool binarySearch(ourvector<string>& spamList, string findMatch)
{
	int low = 0;
	int high = spamList.size() - 1;
	
	while (low <= high)
	{
		int mid = low + ((high - low) / 2);
		string midString = spamList[mid];

		if (findMatch == midString)
			return true; //Is spam

		else if (findMatch < midString)
			high = mid - 1;

		else
			low = mid + 1;
	}

	return false; //Not spam
}

/*This will check if a single email is spam*/
void checkEmail(ourvector<string>& spamList, string command2)
{
	string emailDomain;
	string emailUsername;
	string email = command2;
	string format; //The email with spam format including the *
	string format2; //The email in spam format

	emailParse(email, emailDomain, emailUsername);
	//cout << "Email username: " << emailUsername << endl; //For debugging
	//cout << "Email domain: " << emailDomain << endl; //For debugging	

	format = emailDomain + ":*";
	format2 = emailDomain + ":" + emailUsername;
	//cout << "Spam format 1: " << spamEmail << endl; //For debugging
	//cout << "Spam format 2:" << spamEmail2 << endl; //For debugging

	if (binarySearch(spamList, format) || binarySearch(spamList, format2))
	{
		cout << email << " is spam\n";
		return;
	}

	else
	{
		cout << email << " is not spam\n";
		return;
	}

	return;
}

/*Opens the emails file*/
void openEmails(ourvector<string>& emailList, string fileName)
{
	emailList.clear();

	ifstream txtFile;
	string fileCon; //File contents

	txtFile.open(fileName);

	if (!txtFile.is_open())
	{
		cout << "**Error, unable to open '" << fileName << "'\n";
		return;
	}

	else
	{
		//cout << "Loading '" << fileName << "'\n"; //for debugging

		while (getline(txtFile, fileCon))
		{
			emailList.push_back(fileCon);
		}

		cout << "# emails processed: " << emailList.size() << endl;
	}

	//cout << "File closing...\n"; //for debugging
	txtFile.close();

	return;
}

/*Writes whatever emails were not spam to the outputfile*/
void outputWrite(ofstream& outFile, ourvector<string>& spamList, ourvector<string>& emailList, int& counter)
{
	string spam;

	for (int i = 0; i < emailList.size(); i++)
	{
		if (filterListCont(spamList, emailList[i], counter))
			outFile << emailList[i] << endl;

		else
			spam = emailList[i];
	}

	return;
}

/*Compares the contents of the email vector and spam vector. Will return the string that was marked as spam*/
bool filterListCont(ourvector<string>& spamList, string emailLine, int& counter)
{
	//These will be used to parse the line into email format
	string part1;
	string email;

	//This will be used to format the email into username and domain
	string username;
	string domain;
	string format; //The email with spam format including the *
	string format2; //The email in spam format
	size_t at;
	size_t afterAt;

	size_t space1 = emailLine.find(' ');

	part1 = emailLine.substr(space1 + 1);

	//cout << "part1:" << part1 << "||" << endl; //For debugging

	size_t space2 = part1.find(' ');

	email = part1.substr(0, space2);

	//cout << "part2:" << part2 << "||" << endl; //For debugging

	at = email.find('@');
	afterAt = at + 1;

	username = email.substr(0, at);
	domain = email.substr(afterAt);

	format = domain + ":*";
	format2 = domain + ":" + username;

	if (binarySearch(spamList, format) || binarySearch(spamList, format2)) //Was spam
		return false;

	else //Was not spam
	{
		counter++;
		return true;
	}

}

/*This will filter all emails and output them to a .txt file*/
void filterList(ourvector<string> spamList, string command0, string command1, string command2, string command3)
{
	//TO DO: Check to make sure that command2 and command3 aren't the same
	if (command2 == command3) //If the user didn't input a output file name
	{
		cout << "**invalid command" << endl;
		return;
	}
	
	else //A third input was found
	{
		size_t space;
		space = command2.find(' ');
		command2 = command2.substr(0, space);
	}

	if (command2 == command3) //If the user inputted a output file name the same as the file that's being read from
	{
		cout << "**invalid command" << endl;
		return;
	}

	//cout << "command 2: " << command2 << endl; //for debugging
	//cout << "command 3: " << command3 << endl; //for debugging

	//TO DO: Filter any email and send it to the output file
	ourvector<string> emailList;
	string fileName = command2;
	string outFileName = command3;
	int counter = 0;

	openEmails(emailList, fileName);

	if (emailList.size() == 0) //Makes sure that emailList vector was actually filled
		return;

	ofstream outFile(outFileName);
	
	if (!outFile.is_open())
	{
		cout << "**Error, unable to open '" << outFileName << "'\n";
		
		return;
	}

	else
	{
		outputWrite(outFile, spamList, emailList, counter);

		cout << "# non-spam emails: " << counter << endl;
	}

	outFile.close();

	return;
}

int main()
{
	//TO DO: Declare variables
	string command0;
	string command1;
	string command2;
	string command3;
	
	ourvector<string> spamList;

	//TO DO: Call Functions
	cout << "** Welcome to spam filtering app **\n";

	cout << "\nEnter command or # to exit> ";
	getline(cin, command0);
	
	parse(spamList, command0, command1, command2, command3);

	/*cout << "command 1: " << command1 << endl;
	cout << "command 2: " << command2 << endl; //for debugging
	*/

	while (command1 != "#")
	{

		if (command1 == "load")
		{
			loadFile(spamList, command2);
			//cout << "Loading file\n"; //For debugging
		}

		else if (command1 == "display")
		{
			displaySpam(spamList);
			//cout << "Displaying spam list\n"; //For debugging
		}

		else if (command1 == "check")
		{
			checkEmail(spamList, command2);
			//cout << "Checking email\n"; //For debugging
		}

		else if (command1 == "filter")
		{
			filterList(spamList, command0, command1, command2, command3);
			//cout << "filtering emails\n"; //For debugging
		}

		else
			cout << "**invalid command\n";
		
		cout << "\nEnter command or # to exit> ";
		getline(cin, command0);
	
		parse(spamList, command0, command1, command2, command3);
	}

	spamList.clear();
	spamList._stats();

	//cout << "\nExiting\n"; //For debugging
	system("pause");
	return 0;
}
