/*symtable.h*/

// 
// << Jose M. Aguilar (jaguil61) >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #03: symtable
//
// Symbol Table: a symbol table is a stack of scopes, typically used by a
// compiler to keep track of symbols in a program (functions, variables,
// types, etc.).  In most programming languages, you "enter scope" when you 
// see {, and "exit scope" when you see the corresponding }.  Example:
//
// int main()
// {              <-- enterScope()
//    int i;      <-- enter "i" into symbol table as type "int"
//    .
//    .
//    while (true)
//    {              <-- enterScope()
//       char i;     <-- enter "i" into symbol table as type "char"
//
// Notice there are two variables named "i", which is legal because
// they are in different scopes.
//

#pragma once

#include <iostream>
#include <deque>
#include <map>

using namespace std;

template<typename KeyT, typename SymbolT>
class symtable
{
public:
  //
  // A symbol table is a stack of scopes.  Every scope has a name, and 
  // we use a map to hold the symbols in that scope.  You can *add* to
  // this class, but you must use the Name and Symbols as given to store
  // the scope name, and scope symbols, respectively.
  //
  class Scope
  {
  public:
    string              Name;
    map<KeyT, SymbolT>  Symbols;

    // constructors:
    Scope()
    {
      // default empty string and an empty map:
    }

    Scope(string name)
    {
      this->Name = name;
      // empty map created by map's constructor:
    }
  };

private:
  // TODO: implementation details
  deque<Scope> Deck;
  
  int numSymbols = 0; //Total number of symbols
  int numScopes = 0; //Total number of scopes
 

public:
  enum class ScopeOption
  {
    ALL, 
    CURRENT,
    GLOBAL
  };

  //
  // default constructor:
  //
  // Creates a new, empty symbol table.  No scope is open.
  //
  symtable()
  {
    //
    // TODO: note that member variables will have their default constructor 
    // called automatically, so there may be nothing to do here.
    //
  }

  //
  // size
  //
  // Returns total # of symbols in the symbol table.
  //
  // Complexity: O(1)
  //
  int size() const
  {
    // TODO: 
    
    return numSymbols;
  }

  //
  // numscopes
  //
  // Returns the # of open scopes.
  //
  // Complexity: O(1)
  //
  int numscopes() const
  {
    // TODO: Return size of the deck
    
    return numScopes;
  }

  //
  // enterScope
  //
  // Enters a new, open scope in the symbol table, effectively "pushing" on
  // a new scope.  You must provide a name for the new scope, although
  // the name is currently used only for debugging purposes.
  //
  // NOTE: the first scope you enter is known as the GLOBAL scope, since this
  // is typically where GLOBAL symbols are stored.  
  //
  // Complexity: O(1)
  //
  void enterScope(string name)
  {
    // TODO: Add the new scope to the stack
    Deck.push_front(Scope(name));
    
    numScopes++;
  }

  //
  // exitScope
  //
  // Exits the current open scope, discarding all symbols in this scope.
  // This effectively "pops" the symbol table so that it returns to the 
  // previously open scope.  A runtime_error is thrown if no scope is 
  // currently open.  
  //
  // Complexity: O(1)
  //
  void exitScope()
  {
    // TODO: pop off the last scope
    if (numScopes == 0)
        throw runtime_error("symtable::exitScope: empty");

    int curSize = Deck.front().Symbols.size(); //Number of symbols in current scope

    Deck.pop_front();

    // TODO: Update both the numScopes and numSymbols variables
    numSymbols = numSymbols - curSize;
    numScopes--;
  }

  //
  // curScope
  //
  // Returns a copy of the current scope.  A runtime_error is thrown if
  // no scope is currently open.
  //
  // Complexity: O(N) where N is the # of symbols in the current scope
  //
  Scope curScope() const
  {
    // TODO: Throw a runtime_error if no scope is currently open
      if (numScopes == 0)
          throw runtime_error("symtable::curScope: empty");

      return Deck.front();
  }

  //
  // insert
  //
  // Inserts the (key, symbol) pair in the *current* scope.  If the key
  // already exists in the current scope, the associated symbol is replaced
  // by this new symbol.
  //
  // Complexity: O(lgN) where N is the # of symbols in current scope
  //
  void insert(KeyT key, SymbolT symbol)
  {
    if (numScopes == 0) //No scopes currently open
        return;

    // TODO: check if the key is already in the scope
    if (Deck.front().Symbols.count(key) != 0)
    {
        Deck.front().Symbols.at(key) = symbol; //If so set the old symbol = to the new symbol
    }

    else
    {
        // TODO: insert the key and symbol into the current scope if the symbol isn't already in the scope
        Deck.front().Symbols[key] = symbol; 
        numSymbols++; //Increment number of symbols because a new symbol was added
    }

  }

  //
  // lookup
  //
  // Just return if the symbol table is empty
  //
  // Searches the symbol table for the first (key, symbol) pair that 
  // matches the given key.  The search starts in the current scope, and 
  // proceeds "outward" to the GLOBAL scope.  If a matching (key, symbol)
// pair is found, true is returned along with a copy of the symbol (via 
// "symbol" reference parameter).  If not found, false is returned and
// the "symbol" parameter is left unchanged.
//
// NOTE: the search can be controlled by the "option" parameter.  By 
// default, the entire symbol table is searched as described above.
// However, the search can also be limited to just the current scope, 
// or just the GLOBAL scope, via the "option" parameter.
//
// Example:
//   symtable<string,string>  table;
//   string                   symbol;
//   bool                     found;
//   ...
//   found = table.lookup("i", 
//                        symbol, 
//                        symtable<string,string>::ScopeOption::CURRENT);
//
// Complexity: O(SlgN) where S is the # of scopes and N is the largest #
// of symbols in any one scope
//
bool lookup(KeyT key,
    SymbolT& symbol,
    ScopeOption option = ScopeOption::ALL) const
{
    // TODO: Return if the symbol table is empty
    if (numScopes == 0)
        return false;

    if (option == ScopeOption::ALL)
    {
        for (auto& x : Deck)
        {
            if (x.Symbols.count(key) != 0)
            {
                symbol = x.Symbols.at(key);

                return true;
            }
        }

        return false; // Broke out of loop so return false
    }

  
    else if (option == ScopeOption::CURRENT)
    {
        if (Deck.front().Symbols.count(key) != 0)
        {
            symbol = Deck.front().Symbols.at(key);

            return true;
        }
        
        else
            return false;
    }

    else // GLOBAL
    {
        if (Deck.back().Symbols.count(key) != 0)
        {
            symbol = Deck.back().Symbols.at(key);

            return true;
        }

        else
            return false;
    }


    return false;
}

//
// dump
// 
// Dumps the contents of the symbol table to the output stream, 
// starting with the current scope and working "outward" to the GLOBAL
// scope.  You can dump the entire symbol table (the default), or dump
// just the current scope or global scope; this is controlled by the 
// "option" parameter.
//
// Example:
//   symtable<string,string>  table;
//   ...
//   table.dump(std::cout, symtable<string,string>::ScopeOption::GLOBAL);
//
// Complexity: O(S*N) where S is the # of scopes and N is the largest #
// of symbols in any one scope
//
void dump(ostream& output, ScopeOption option = ScopeOption::ALL) const
{
    output << "**************************************************" << endl;

    if (option == ScopeOption::ALL)
    {
        output << "*************** SYMBOL TABLE (ALL) ***************" << endl;
        output << "** # of scopes: " << this->numscopes() << endl;
        output << "** # of symbols: " << this->size() << endl;
        for (auto& x : Deck)
        {
            output << "** " << x.Name << " **" << endl;
           
            for (auto& P : x.Symbols)
            {
                output << P.first << ": " << P.second << endl;
            }
   
        }
    }

    else if (option == ScopeOption::CURRENT)
    {
        output << "*************** SYMBOL TABLE (CUR) ***************" << endl;
        output << "** # of scopes: " << this->numscopes() << endl;
        output << "** # of symbols: " << this->size() << endl;
        output << "** " << Deck.front().Name << " **" << endl;

        for (auto& P : Deck.front().Symbols)
        {
            output << P.first << ": " << P.second << endl;
        }
    }

    else // global:
    {
        output << "*************** SYMBOL TABLE (GBL) ***************" << endl;
        output << "** # of scopes: " << this->numscopes() << endl;
        output << "** # of symbols: " << this->size() << endl;
        output << "** " << Deck.back().Name << " **" << endl;

        for (auto& P : Deck.back().Symbols)
        {
            output << P.first << ": " << P.second << endl;
        }
    }

    //
    // output format per scope:
    //
    // ** scopename **
    // key: symbol
    // key: symbol
    // ...
    //

    output << "**************************************************" << endl;
  }

};
