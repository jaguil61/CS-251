#include <iostream>
#include <string>
#include "symtable.h"

using namespace std;

int main()
{
	symtable<string, string> table;
	symtable<int, int> intTable;

	table.enterScope("Global");

	//cout << "Number of scopes: " << table.numscopes() << endl;

	table.insert("x", "double");

	//cout << "Number of symbols in the scope: " << table.size() << endl;

	table.enterScope("L5");

	table.insert("x", "int");

	string symbol;
	
	if (table.lookup("i", symbol, symtable<string, string>::ScopeOption::ALL) == true)
	{
		cout << "Symbol: "<< symbol << endl;
	}

	else
	{
		cout << "Not found." << endl;
	}
	
	table.dump(std::cout, symtable<string, string>::ScopeOption::GLOBAL);

	table.exitScope();

	intTable.enterScope("GLOBAL");

	intTable.insert(6, 670);
	intTable.insert(8, 78);

	int Symbol;
	
	if (intTable.lookup(6, Symbol) == true)
	{
		cout << "Symbol: " << Symbol << endl;
	}

	else
	{
		cout << "Not found." << endl;
	}

	intTable.dump(cout);
	/*
	cout << "Number of scopes: " << table.numscopes() << endl;
	cout << "Number of symbols in the scope: " << table.size() << endl;
	*/

	intTable.curScope();

	cout << endl;
	system("pause");
	return 0;
}