/*hashmap.h*/

//
//
// Jose M. Aguilar
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Project 6 Part 1
//
// 
// The class provides the underlying hash table along with insert() and search() functions;
// the user of this class must provided a Hash(key, N) function that 
// hashes the given key and returns an integer in the range 0..N-1, 
// inclusive, if the key is valid.  If the key is invalid, the Hash() 
// function must return a negative value.
//
// The hash function is provided in the calls to insert and search.
// Pass the name of your hash function as the 3rd parameter.  Example:
//
//   int myHash(string key, int N)
//   { ... }
//
//   int N = 10000;
//   hashmap<string, double>  hmap(N);  
//
//   if (hmap.insert("deep-dish pizza", 19.99, myHash))
//     cout << "inserted" << endl;
//   else
//     cout << "not inserted?!" << endl;
//

#pragma once

#include <iostream>
#include <functional>

using namespace std;

template<typename KeyT, typename ValueT>
class hashmap
{
private:
    struct HashEntry
    {
        bool   ContainsData;  // false => empty, true => contains data
        KeyT   Key;           // (key, value)
        ValueT Value;

        HashEntry()
        {
            ContainsData = false;
        }
    };

    HashEntry* HT;  // hash table array of structs
    int        N;   // capacity of hash table (# of locations or array size)

    // probes the next index location for search function
    void _searchProbe(int& index, int& bucketsProbed) const
    {
        index++;
        bucketsProbed++;

        if (index == this->N) // wrap around
        {
            index = 0;
        }

        else // do nothing
            ;

        return;
    }

    // searches for the value
    bool _search(int index, KeyT key, ValueT& value) const
    {
        int bucketsProbed = 0;

        while (this->HT[index].ContainsData && bucketsProbed != N) // (bucketsProbed != N) searched whole HT and nothing found
        {
            if (this->HT[index].Key == key)
            {
                value = HT[index].Value;
                return true;
            }

            else
            {
                _searchProbe(index, bucketsProbed);
            }
        }

        // not found so return false
        return false;
    }

    // copies "other" HT into "this" HT
    void _copy(const hashmap<KeyT, ValueT>& other)
    {
        //cout << other.N << endl; // for debugging purposes

        for (int i = 0; i < other.N; i++)
        {
            this->HT[i].Key = other.HT[i].Key;
            this->HT[i].Value = other.HT[i].Value;
            this->HT[i].ContainsData = other.HT[i].ContainsData;
        }

        return;
    }

    // searches HT and returns the index of an open index location 
    int _indexLocation(int index, KeyT key)
    {
        while (this->HT[index].ContainsData)
        {
            if (this->HT[index].Key == key) // found a matching inside of the HT already
            {
                return index;
            }

            else // do nothing
                ;

            index++; // linear probes

            if (index == N) // wrap around
            {
                index = 0;
            }

            else // do nothing
                ;
        }

        //found an empty spot
        return index;
    }

public:
    //
    // constructor: N is the size of the underlying hash table
    //
    hashmap(int N)
    {
        this->N = N;
        this->HT = new HashEntry[N];
    }

    //
    // destructor
    //
    virtual ~hashmap()
    {
        delete[] this->HT;
    }

    //
    // copy constructor
    //
    hashmap(const hashmap& other)
    {
        // set defaults and allocate new HT
        this->N = other.N;
        this->HT = new HashEntry[other.N];

        //cout << this->N << endl; // for debugging purposes

        //call copy helper function
        _copy(other);
    }

    //
    // operator=
    //
    // Clears "this" tree and then makes a copy of the "other" tree.
    //
    hashmap& operator=(const hashmap& other)
    {
        // clear "this" HT
        delete[] this->HT;

        // set defaults and allocate new HT
        this->N = other.N;
        this->HT = new HashEntry[other.N];

        // call _copy() function to make copy
        _copy(other);

        return *this;
    }

    //
    // getN
    //
    // Returns the capacity of the hash table, i.e. the value N that
    // was passed to the constructor.
    //
    int getN() const
    {
        return this->N;
    }

    //
    // insert
    //
    // Inserts the given (key, value) pair into the hash table, adding into 
    // a new open spot if previous one is occupied. If the insert was successful
    // then true is returned, otherwise false is returned (meaning the key
    // was not hashed successfully, e.g. due to improper formatting).
    //
    // NOTE: the caller must provide a function Hash(key, N) that returns
    // an integer in the the range 0..N-1, inclusive, if the key is valid.
    // If the key is invalid, the hash function must return a negative
    // integer. The hash function is provided by simply calling insert
    // with the name of your hash function.  Example:
    //
    //   int myHash(string key, int N)
    //   { ... }
    //
    //   int N = 10000;
    //   hashmap<string, double>  hmap(N);  
    //
    //   if (hmap.insert("deep-dish pizza", 19.99, myHash))
    //     cout << "inserted" << endl;
    //   else
    //     cout << "not inserted?!" << endl;
    //
    bool insert(KeyT key, ValueT value, function<int(KeyT, int)> Hash)
    {
        //
        // Call the provided hash function with the key, and N, the size
        // of our hash table:
        //
        int index = Hash(key, N);

        //
        // insert:
        //
        if (index < 0 || index >= N)  // invalid key or hash function:
        {
            return false;
        }

        else  // valid array index, store into hash table:
        {
            index = _indexLocation(index, key);

            HT[index].Key = key;
            HT[index].Value = value;
            HT[index].ContainsData = true;  // array location now contains data

            return true;
        }
    }


    //
    // search
    //
    // Searches the hash table for the matching key, and if found then
    // (1) the value is returned via the reference parameter and (2) true
    // is returned.  If the key could not be found, or if the hash 
    // function returned an invalid value (e.g. due to an improperly 
    // formatted key), then false is returned.
    //
    // NOTE: the caller must provide a function Hash(key, N) that returns
    // an integer in the the range 0..N-1, inclusive, if the key is valid.
    // If the key is invalid, the hash function must return a negative
    // integer. The hash function is provided by simply calling search
    // with the name of your hash function.  Example:
    //
    //   int myHash(string key, int N)
    //   { ... }
    //
    //   int N = 10000;
    //   hashmap<string, double>  hmap(N);
    //   ...
    //   double price;
    //   if (hmap.search("deep-dish pizza", price, myHash))
    //     cout << "deep-dish piazza costs " << price << endl;
    //   else
    //     cout << "not found?!" << endl;
    //
    bool search(KeyT key, ValueT& value, function<int(KeyT, int)> Hash) const
    {
        //
        // Call the provided hash function with the key, and N, the size
        // of our hash table:
        //
        int index = Hash(key, N);

        //
        // search:
        //

        if (index < 0 || index >= N)  // invalid key or hash function:
        {
            return false;
        }

        else if (HT[index].ContainsData) // hash table contains data:
        {
            return _search(index, key, value); // _search updates value via reference parameter
        }

        else  // hash table is empty, no data:
        {
            return false;
        }
    }

};
