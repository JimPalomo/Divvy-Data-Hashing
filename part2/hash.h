/*hash.h*/

//
// Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Lab Week 11: hashing of dates and lottery numbers (SOLUTION)
// 

#include <string>

using namespace std;

bool isNumeric(string s);
bool isNumericChar(char c);
int string2int(string s);
int char2int(char c);
int HashStationID(string ID, int N);
int HashStationAbbrev(string Abbrev, int N);
int HashTripID(string ID, int N);
int HashBikeID(string ID, int N);
