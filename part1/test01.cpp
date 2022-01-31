/*test01.cpp*/

//
// Unit tests for hash table
//

#include <iostream>
#include "hashmap.h"
#include "hash.h"
#include "catch.hpp"

using namespace std;

TEST_CASE("(1) initial test") {

  // int myHash(string key, int N);
 
  int N = 10000;
  hashmap<string, double>  hmap(N);  

  if (hmap.insert("deep-dish pizza", 19.99, Hash))
    cout << "inserted" << endl;
  else
    cout << "not inserted?!" << endl;



}