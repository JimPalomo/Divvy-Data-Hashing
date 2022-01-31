/*hash.cpp*/

//
// Jim Palomo
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Project 6: hashing for DIVVY bike data
// 

#include <iostream>
#include <string>
#include <cctype>  /*isdigit*/
#include <stdlib.h>	


#include "hash.h"

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
// isNumericChar
//
// Returns true if the given char is numeric (a digit), false
// if not.  If the char is null, false is returned since there 
// are no digit present.
//
bool isNumericChar(char c)
{
   if (c == '\0')
     return false;
    
	if (!isdigit(c))  // if c is not a digit, it cannot be numeric:
   		return false;

	return true;	// must be numeric
}


//
// string2int
// 
// Converts a string to an integer, unless string is empty, in
// which case 0 is returned.
// 
int string2int(string s)
{
	if (s == "")
		return 0;
	else
		return stoi(s);
}


//
// char2int
// 
// Converts a char to an integer, unless char is null, in
// which case 0 is returned.
// 
int char2int(char c)
{
	if (c == '\0')
		return 0;
	else {
		int number = c;
		return number;
	}
}


//
// HashStationID
// 
// Function that hashes the station ID (10k)
//
int HashStationID(string ID, int N) {

    // Validation check
    if (ID.length() < 1) {
        return -1;
	}

	int index = 0; 

	if (isNumeric(ID))	// make sure ID is numeric
		index = string2int(ID);	// convert ID into an int

	return index % N;	// 3 digit 0-999
}


//
// HashStationAbbrev
// 
// Function that hashes the station abbreviation (10k)
//
int HashStationAbbrev(string Abbrev, int N) {

    // Validation check
    if (Abbrev.length() < 1) {
        return -1;
	}
	
	int index = 0;
	int temp = 1;
	int temp2 = 1;

	for (unsigned int i = 0; i < Abbrev.length(); i++) {
		if (isNumericChar(Abbrev[i]))
			temp = char2int(Abbrev[i]) ^ (13 >> 3);	// 26-35 (xor & right shift)

		else if (!isNumericChar(Abbrev[i])) {
			for (unsigned int i = 0; i < Abbrev.length(); i++) 
				temp = Abbrev[i] - 'A';	// letter - ascii value of A --> 0-25
		} 
		
		else {	// must contain both #'s & letters
			if (isNumericChar(Abbrev[i]))
				temp2 = char2int(Abbrev[i]) ^ (23 << 5);	// 26-35 (xor & left shift)
			else
				temp2 = Abbrev[i] - 'A';			
		}

		index += (temp2 ^ temp);	// XOR 
	}

	return index % N;
}


//
// HashTripID
// 
// Function that hashes the trip ID (2.5M)
//
int HashTripID(string ID, int N) {
	
    // Validation check
    if (ID.length() < 3) 
        return -1;
	
	// make sure T & r exist at the correct location
    if (ID[0] != 'T' || ID[1] != 'r') 
		return -1;
	
	ID.erase(2,1);	// delete 1 (make more unique)
	ID.erase(1,1);	// delete r
	ID.erase(0,1);	// delete T

	if (!isNumeric(ID))	// check if ID contains #'s
		return -1;

	int index = string2int(ID);	// convert ID into an integer

	return index % N;
}


//
// HashBikeID
// 
// Function that hashes the bike ID (50k)
//
int HashBikeID(string ID, int N) {
	
    // Validation check
    if (ID.length() < 2) 
        return -1;
	
	// make sure B exists
    if (ID[0] != 'B') 
		return -1;
	
	ID.erase(0,1);	// delete B

	if (!isNumeric(ID)) // check if ID contains #'s
		return -1;	
	
	int index = string2int(ID);	// convert ID into an integer

	return index % N;
}