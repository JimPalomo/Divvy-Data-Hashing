/*hashmap.h*/

//
// Jim Palomo
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #06: hashing
//
// References: original non-collision code written by Prof. Hummel (UIC)
// 
// hashmap implements a hash table assuming a non-perfect hash function;
// the class is designed to handle collisions. The class provides
// the underlying hash table along with insert() and search() functions;
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
#include <vector>
#include <cctype>     // distBetween2Points operations
#include <algorithm>  // sort

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
  // copy constructor: make an exact copy of the "other" hash table.
  //
  hashmap(const hashmap& other) 
  {
    
    N = other.N;  // update N's value with "other" N

    // allocate new hash table ("this")
    HT = new HashEntry[N];

    // copy over data from the "other" Hash Table to "this" Hash Table
    for (int i = 0; i < N; i++) {
      if (other.HT[i].ContainsData)
        // copy struct data: ContainsData, Key, Value for each index in the HT
        HT[i] = other.HT[i];  
    }
  }

  //
  // operator= : clears "this" hash table and then makes a copy of the "other"
  // tree. 
  //
  hashmap& operator=(const hashmap& other) 
  {

    delete[] HT;  // delete "this" hash table (clearing)    
    
    N = other.N;  // update N's value with "other" N (size of hash table)

    // allocate new hash table ("this")
    HT = new HashEntry[N];

    // copy over data from the "other" Hash Table to "this" Hash Table
    for (int i = 0; i < N; i++) {
      if (other.HT[i].ContainsData)
        // copy struct data: ContainsData, Key, Value for each index in the HT
        HT[i] = other.HT[i];  
    }
    
    // return "this" hash table
    return *this;
  }

  //
  // destructor
  //
  virtual ~hashmap()
  {
    delete[] this->HT;
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

  // getKeys
  // 
  // Get keys from the Hash Table and then return as vector
  //
  vector<KeyT> getKeys()
  {
    vector<KeyT> vectKey; // create a vector of keys

    // loop through hash table of size N (user input)
    for (int i = 0; i < N; i++) { // O(N)

      // push the key only if the index contains data
      if (HT[i].ContainsData) {
        vectKey.push_back(HT[i].Key);
      }

    }

    return vectKey;
  }

  // Insert no probing (insert only unique variables)
  bool insertUnique(KeyT key, ValueT& value, function<int(KeyT,int)> Hash)
  {
    int index = Hash(key, N);

    if (index < 0 || index >= N) { // invalid key or hash function:
      return false;
    }

    if (HT[index].ContainsData) { // if data exists then
        HT[index].Value.Usage++;  // keep track of bike use time
 
      return false;
    }

    
    // when an open spot is found insert values & update ContainsData
    HT[index].Key = key;
    HT[index].Value = value;
    HT[index].ContainsData = true;
    HT[index].Value.Usage++;  

    return true;
  }

  //
  // insert
  //
  // Inserts the given (key, value) pair into the hash table, overwriting
  // the previous value if already inserted.  If the insert was successful
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
  bool insert(KeyT key, ValueT value, function<int(KeyT,int)> Hash)
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
      return false;

    else {

        // while data exists in the index, traverse the hash table
        while (HT[index].ContainsData) {

          // check if user's key is found then break out of the loop
          if (HT[index].Key == key) 
            break;

          index++;  // increment through hash table

          // if at the end of the hash table then return to the top (probe through)
          if (index == N)
            index = 0;
      }
    }

    // when an open spot is found insert values & update ContainsData
    HT[index].Key = key;
    HT[index].Value = value;
    HT[index].ContainsData = true;  

    return true;
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
  // an integer in the the rangeHT[i] 0..N-1, inclusive, if the key is valid.
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
  bool search(KeyT key, ValueT& value, function<int(KeyT,int)> Hash) const
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
      return false;

    else {
      while (HT[index].ContainsData) {

        // check to see if the Key matches the key (user)
        if (HT[index].Key == key) {
          value = HT[index].Value;
          return true;
        }

        index++;  // increment through hash table

        // if the index reaches the end of the hash table then start over 
        // back at the beginning 
        if (index == N)
          index = 0;
      }
    }

      return false;
  }

};