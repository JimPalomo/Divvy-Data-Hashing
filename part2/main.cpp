/*main.cpp*/

//
// Jim Palomo
// U. of Illinois, Chicago
// CS 251: Spring 2020
// 
// Project 6: hashing DIVVY bike data
// 
// Given data for Stations & Trips (by the user), the program allows for
// analysis of the data. For example, the user can search for specific stations
// and display the ID, abbrevation, fullname, location, capacity, online date. 
// On the other hand, the user can find trips given the Trip ID and see the ID, 
// start time, bikeid, duration ,from station, to station, gender, birthyear.

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>		// std::pair
#include <algorithm>	// std::sort
#include <set>			// std::set

#include "hash.h"
#include "hashmap.h"
#include "util.h"		// std::distBetween2Points

using namespace std;

// struct for station data
struct StationData {
	string StationID;
	string Abbrev;
	string Fullname;
	string Latitude;
	string Longitude;
	string Capacity;
	string OnlineDate;

	StationData() {
	string StationID 	= "";
	string Abbrev 		= "";
	string Fullname 	= "";
	string Latitude 	= "";
	string Longitude 	= "";
	string Capacity 	= "";
	string OnlineDate 	= "";
	}
};

// struct for trip data
struct TripData {
	string TripID;
	string StartTime;
	string StopTime;
	string BikeID;
	string Duration;
	string From;
	string To;
	string Identifies;
	string BirthYear;

	TripData() {
		string TripID 		= "";
		string StartTime 	= "";
		string StopTime		= "";
		string BikeID		= "";
		string Duration		= "";
		string From			= "";
		string To			= "";
		string Identifies	= "";
		string BirthYear	= "";
	}
};

// struct for BikeData
struct BikeData {
	string BikeID;
	int Usage;
};

// Function Calls (function details below main)
bool inputDataStation(string filename, hashmap<string, StationData>& hmap, 
	int& numStations);

bool inputDataTrip(string filename, hashmap<string, TripData>& hmap, int& numTrips, 
	int& numBikes, hashmap<string, BikeData>& hmap2);

bool checkFiles(string& stationFile, string& tripFile, bool inputStation, 
	bool inputTrip);

void Duration(string totalSec);

void helpCommand();

bool sortByDistance(const pair<string, double>& a, const pair<string, double>& b);

bool sortByKeys(const string& a, const string& b);

void removeDuplicateVectPairs(vector<pair<string, double>>& vect);

void removeDuplicateVect(vector<string>& vect);

void nearbyCommand(string s, hashmap<string, StationData>& myMap, 
	vector<string>& storeStationID);

void printNearbyPoints(vector<string>& tripFromVect, vector<string>& tripToVect);

void printTripCount(vector<string>& tripFromVect, vector<string>& tripToVect, 
	vector<string>& storeTripID, hashmap<string, TripData>& tripMap);

bool checkTrip(hashmap<string, TripData>& tripMap, hashmap<string, StationData>& 
	stationMap, string& tripid, string& lat1, string& long1, string& fromID);

void updateFromVect(vector<string>& storeStationID, hashmap<string, StationData>& 
	stationMap, vector<string>& tripFromVect, string& lat1, string& long1, 
	string& dist);

void updateToVect(vector<string>& storeStationID, hashmap<string, StationData>& 
	stationMap, vector<string>& tripToVect, string& lat1, string& long1, 
	string& dist);

void similarCommand(string s, hashmap<string, TripData>& tripMap, 
	hashmap<string, StationData>& stationMap, vector<string>& storeStationID, 
	vector<string>& storeTripID);

void displayNumSet(int& numStations, int& numTrips, int& numBikes);

void stationIDCommand(string& userInput, hashmap<string, StationData>& stationMap);

void tripIDCommand(string& userInput, hashmap<string, TripData>& tripMap, 
	hashmap<string, StationData>& stationMap);

void bikeIDCommand(string& userInput, hashmap<string, BikeData>& bikeMap);

void stationAbbrevCommand(string& userInput, hashmap<string, StationData>& 
	stationMap);


int main() {
    cout << "** DIVVY analysis program **" << endl << endl;

	hashmap<string, StationData> stationMap(10000);
	hashmap<string, TripData> tripMap(2500000);
	hashmap<string, BikeData> bikeMap(50000);

	// vectors to store station & trip ID's into a vector
	vector<string> storeStationID;
	vector<string> storeTripID;

	string stationFile, tripFile, userInput; // create variables

    cout << "Enter stations file> "; // obtain name of files
	getline(cin, stationFile);
	
	cout << "Enter trips file> ";
	getline(cin, tripFile);

	cout << endl;

	cout << "Reading " << stationFile 	<< endl; 	// read in files
	cout << "Reading " << tripFile 		<< endl;

	// create variables used for pass by reference
	int numStations, numBikes, numTrips;		
	numStations = numBikes = numTrips = 0;

	bool inputStation = inputDataStation(stationFile, stationMap, numStations);
	bool inputTrip = inputDataTrip(tripFile, tripMap, numTrips, numBikes, 
		bikeMap);

	bool check = checkFiles(stationFile, tripFile, inputStation, inputTrip);

	if(!check)
		return 0;

	// store stationIDs & TripIDs into a vector
	storeStationID = stationMap.getKeys();
	storeTripID = tripMap.getKeys();

	// add variables to couts
	displayNumSet(numStations, numTrips, numBikes);

	// input the user's command
	cout << "Please enter a command, help, or #> ";
	getline(cin, userInput);

	while (userInput != "#") {	

		// help command	
		if (userInput == "help") 
			helpCommand();
		
		// stationID command
		else if (((isNumeric(userInput)) && (userInput.substr(0,6) != "nearby")
			&& (userInput.substr(0,7) != "similar")))	// station data
			stationIDCommand(userInput, stationMap);

		// tripID command
		else if (userInput[0] == 'T' && userInput[1] == 'r' && 
			isNumeric(userInput.substr(2))) 
			tripIDCommand(userInput, tripMap, stationMap);
		
		// bikeID command
		else if (userInput[0] == 'B' && isNumeric(userInput.substr(1))) 	
			bikeIDCommand(userInput, bikeMap);

		// nearby command
		else if (userInput.substr(0,6) == "nearby")	
			nearbyCommand(userInput.substr(7), stationMap, storeStationID);
		
		// similar command
		else if (userInput.substr(0,7) == "similar") {
			if (isNumeric(userInput.substr(10,2)))
				similarCommand(userInput.substr(8), tripMap, stationMap, 
				storeStationID, storeTripID);
			else 
				cout << " no such trip" << endl << endl;
		}	

		else {	// station abbreviation command	
			stationAbbrevCommand(userInput, stationMap);
		}

		// get user's next input
		cout << "Please enter a command, help, or #> ";
		getline(cin, userInput);
	} 
	
}


//
// inputData
// 
// Given stations data, input all data into the given hash table
// 
bool inputDataStation(string filename, hashmap<string, StationData>& hmap, 
	int& numStations) {
	ifstream  infile(filename);	// create a file stream
	
	// check to make sure file has been read in
	if (!infile.good()) {
		cout << "**Error: unable to open '" << filename << "'..." << endl;
		return false;
	}
	
	// file is open, start processing:
	string line;
	
	getline(infile, line);  // input and discard first row (header)
		
	// Loop through the file and push information into the hash table
	while (getline(infile, line))
	{
		stringstream s(line);	// create a string stream
		
		string stationid, abbrev, fullname, latitude, longitude, 
		capacity, online_date;
		
		// use string stream & getline to store information into variables
		getline(s, stationid, ',');  	// first value => stationid
		getline(s, abbrev, ',');   		// second value => abbrev
		getline(s, fullname, ',');
		getline(s, latitude, ',');
		getline(s, longitude, ',');
		getline(s, capacity, ',');
		getline(s, online_date, ',');

		StationData sd;	// create an instance of the struct
		
		// update the station data information in the struct
		sd.StationID 	= stationid;
		sd.Abbrev 		= abbrev;
		sd.Fullname 	= fullname;
		sd.Latitude 	= latitude;
		sd.Longitude 	= longitude;
		sd.Capacity 	= capacity;
		sd.OnlineDate 	= online_date;

		// insert station ID & abbrev into the hash table
		hmap.insert(stationid, sd, HashStationID);
		hmap.insert(abbrev, sd, HashStationAbbrev);

		numStations++;	// count the number of stations inserted into the HT
	}
		
	return true;  // data processed
}


//
// inputData
// 
// Given trip data, input all data into the given hash table
// 
bool inputDataTrip(string filename, hashmap<string, TripData>& hmap, int& numTrips, 
	int& numBikes, hashmap<string, BikeData>& hmap2) {
	ifstream  infile(filename);	// create file stream

	// check to make sure file has been read in
	if (!infile.good()) {
		cout << "**Error: unable to open '" << filename << "'..." << endl;
		return false;
	}
	
	// file is open, start processing:
	string line;
	
	getline(infile, line);  // input and discard first row (header)

	// Loop through the file and push information into the hash table
	while (getline(infile, line))
	{
		stringstream s(line); // create a string stream
		
		string tripid, starttime, stoptime, bikeid, duration, from, to, 
		identifies, birthyear, bikeid2;
		
		// use string stream & getline to store information into variables
		getline(s, tripid, ',');  		// first value => tripid
		getline(s, starttime, ',');   	// second value => starttime
		getline(s, stoptime, ',');
		getline(s, bikeid, ',');
		getline(s, duration, ',');
		getline(s, from, ',');
		getline(s, to, ',');
		getline(s, identifies, ',');				
		getline(s, birthyear, ',');				
		
		TripData td; // create an instance of the struct

		// update the station data information in the struct
		td.TripID 		= tripid;
		td.StartTime 	= starttime;
		td.StopTime 	= stoptime;
		td.BikeID 		= bikeid;
		td.Duration 	= duration;
		td.From 		= from;
		td.To 			= to;
		td.Identifies 	= identifies;
		td.BirthYear 	= birthyear;

		// insert trip ID into the hash table
		hmap.insert(tripid, td, HashTripID);


		BikeData bd; // create an instance of the struct

		bd.Usage = 0; // reset usage to 0

		// insert bike ID into the hash table
		if (hmap2.insertUnique(bikeid, bd, HashBikeID))
			numBikes++; // count the number of bikes inserted into the HT
		
		numTrips++; // count the number of trips inserted into the HT
	}
		
	return true;  // data processed
}


//
// checkFiles
// 
// Checks to see if the files are found (return true), else return false
// 
bool checkFiles(string& stationFile, string& tripFile, bool inputStation, 
	bool inputTrip) {
	if (!inputStation) {		// check to see if files are read
		cout << "No data, file not found for: " << stationFile << endl;
		return false;
	}

	if (!inputTrip) {
		cout << "No data, file not found for: " << tripFile << endl;
		return false;
	}

	return true;
}


//
// Duration
// 
// Given seconds, convert into Hours, Minutes, Seconds
// 
void Duration(string totalSec) {
	int s = string2int(totalSec);	// convert seconds into an integer
	int m = s / 60;					// convert seconds into minutes
	int h = m / 60;					// convert seconds into hours
	s = s % 60;						// obtain seconds by mod operator

    if (h > 0) 	// if hours exist
	    cout << " Duration: " << h << " hours, " << m << " minutes, " << s << "seconds" << endl;
    
    else if (m > 0)	// if minutes & seconds exist
	    cout << " Duration: " << m << " minutes, " << s << " seconds" << endl;

    else 	// only seconds exist
	    cout << " Duration: " << s << " seconds" << endl;
}


//
// helpCommand
// 
// System output when the user enters help (command)
// 
void helpCommand() {
	cout << "Available commands: " << endl;
	cout << " Enter a station id (e.g. 341)" << endl;
	cout << " Enter a station abbreviation (e.g. Adler)" << endl;
	cout << " Enter a trip id (e.g. Tr10426561)" << endl;
	cout << " Enter a bike id (B5218)" << endl;
	cout << " Nearby stations (e.g. nearby 41.86, -87.62 0.5)" << endl;
	cout << " Similar trips (e.g. similar Tr10424639 0.3)" << endl;
	cout << endl;
}


//
// sortByDistance
// 
// Function to assist in sorting a pair of string & double. The function 
// is used when using std::sort as the third parameter. Sorts by least to 
// greatest distance.
//
bool sortByDistance(const pair<string, double>& a, const pair<string, double>& b) { 
    return (a.second < b.second); 
} 


//
// sortByKeys
//
// Function to assist in sorting strings. The function is used when using 
// std::sort as the third parameter. Sorts by least to greatest Keys.
// Keys are converted to integers for comparison.
//
bool sortByKeys(const string& a, const string& b) {
	return (string2int(a) < string2int(b));
}


//
// removeDuplicateVectPairs
//
// Function to remove duplicate pairs in a vector pair. Uses remove from #include
// algorithm library
//
void removeDuplicateVectPairs(vector<pair<string, double>>& vect) {
	auto end = vect.end();

	// iterate through vector and erase any duplicate keys, value pairs
	for (auto it = vect.begin(); it != end; ++it) {
		end = remove(it + 1, end, *it);	
	}

	vect.erase(end, vect.end());
}


//
// removeDuplicateVect
//
// Function to remove duplicate pairs in a vector pair. Uses remove from #include
// algorithm library
//
void removeDuplicateVect(vector<string>& vect) {
	auto end = vect.end();

	// iterate through vector and erase any duplicate keys
	for (auto it = vect.begin(); it != end; ++it) {
		end = remove(it + 1, end, *it);
	}

	vect.erase(end, vect.end());
}


//
// nearbyCommand
//
// Function that is used when the user requests for nearby data. Returns 
// nearby stations that are within a certain range.
//
void nearbyCommand(string s, hashmap<string, StationData>& myMap, 
	vector<string>& storeStationID) {
	stringstream mystream(s);	// create a string stream

	string lat1, long1, dist; 	// splice string and save information into variables

	getline(mystream, lat1, ' ');
	getline(mystream, long1, ' ');
	getline(mystream, dist, ' ');

	cout << "Stations within " << dist << " miles of (" << lat1 
		 << ", " << long1 << ")" << endl;

	// create a vector pair that stores station IDs & converted distances
	vector<pair<string, double>> distanceVect;

	StationData sd;	// create an instance of the struct

	// loop through the station IDs
	for (unsigned int i = 0; i < storeStationID.size(); i++) {
		// search StationData hashmap given the stored station IDs
		// allows to access station data HT
		myMap.search(storeStationID[i], sd, HashStationID);

		// given access to the station data, convert and obtain the distance
		// given lat1, long1, lat2, long2
		double dist2Points = distBetween2Points((double)atof(lat1.c_str()), 
			(double)atof(long1.c_str()), (double)atof(sd.Latitude.c_str()), 
			(double)atof(sd.Longitude.c_str()));

		// if the distance is within range then push into the vector pair
		if (dist2Points <= (double)atof(dist.c_str())) {
			distanceVect.push_back(make_pair(sd.StationID, dist2Points));	
		}
	}

	// sort distances using std::sort
	sort(distanceVect.begin(), distanceVect.end(), sortByDistance);

	// remove duplicates that occur in the vector
	removeDuplicateVectPairs(distanceVect);

	// dump information from the vector
	if (distanceVect.size() > 0) {
		for (auto& iter : distanceVect) {
			cout << " station " << iter.first << ": " << iter.second 
				 << " miles"<< endl;
		}
	}
	else
		cout << " none found" << endl;

	cout << endl;
}


//
// printNearbyPoints
//
// Function that prints out the nearby starting/ending points.
//
void printNearbyPoints(vector<string>& tripFromVect, vector<string>& tripToVect) {
	cout << " nearby starting points: ";	// display from (start) points
	for (unsigned int i = 0; i < tripFromVect.size(); i++) 
		cout << tripFromVect[i] << " ";
	cout << endl;
	
	cout << " nearby ending points: ";	// display to (end) points
	for (unsigned int i = 0; i < tripToVect.size(); i++)
		cout << tripToVect[i] << " ";
	cout << endl;
}

//
// printTripCount
//
// Function that prints out the trip count.
//
void printTripCount(vector<string>& tripFromVect, vector<string>& tripToVect, 
	vector<string>& storeTripID, hashmap<string, TripData>& tripMap) {

	cout << " trip count: ";

	// sets created and used for comparison
	set<string> tripFromSet;
	set<string> tripToSet;

	TripData td;	// instance of struct

	for (unsigned int i = 0; i < tripFromVect.size(); i++)	// insert vect into set
		tripFromSet.insert(tripFromVect[i]);

	for (unsigned int i = 0; i < tripToVect.size(); i++)
		tripToSet.insert(tripToVect[i]);	

	int numTrips = 0;

	for (auto it = storeTripID.begin(); it != storeTripID.end(); it++) {
		tripMap.search(*it, td, HashTripID);	// search for Hash Table & obtain access
		if (tripFromSet.count(td.From) && tripToSet.count(td.To)) { // insert unique pairs			
			numTrips++;	// iterate trip counter if exists
		}
	}
	cout << numTrips << endl << endl;

}


//
// checkTrip
//
// Function that checks to see if a trip From exists and returns true if it 
// does, else false.
//
bool checkTrip(hashmap<string, TripData>& tripMap, hashmap<string, StationData>& 
	stationMap, string& tripid, string& lat1, string& long1, string& fromID) {

	TripData td;	// create an instance of the struct
	StationData sd;	// create an instance of the struct

	bool foundTrip = tripMap.search(tripid, td, HashTripID);

	if (foundTrip) {	// if trip data is found then store From StationData, Lat/Long
		tripMap.search(tripid, td, HashTripID);
		fromID = td.From;

		stationMap.search(td.From, sd, HashStationID);

		lat1 = sd.Latitude;
		long1 = sd.Longitude;
	}

	else {	// no such trip exists
		cout << " no such trip" << endl << endl;
		return false;
	}

	return true;
}


//
// updateFromVect
//
// Function that updates the From vector and returns an updated version. Searches
// station IDs and pushes them into a trip From vector.
//
void updateFromVect(vector<string>& storeStationID, hashmap<string, StationData>& 
	stationMap, vector<string>& tripFromVect, string& lat1, string& long1, string& dist) {

	StationData sd;

	for (unsigned int i = 0; i < storeStationID.size(); i++) {	// pushing From station IDs into a vector 
		stationMap.search(storeStationID[i], sd, HashStationID);

		double dist2Points = distBetween2Points((double)atof(lat1.c_str()), 
			(double)atof(long1.c_str()), (double)atof(sd.Latitude.c_str()), 
			(double)atof(sd.Longitude.c_str()));

		if (dist2Points <= (double)atof(dist.c_str()))
			tripFromVect.push_back(sd.StationID);			
	}
}


//
// updateToVect
//
// Function that updates the To vector and returns an updated version. Searches
// station IDs and pushes them into a trip To vector.
//
void updateToVect(vector<string>& storeStationID, hashmap<string, StationData>& 
	stationMap, vector<string>& tripToVect, string& lat1, string& long1, 
	string& dist) {

	StationData sd;	// instance of the struct 
	
	for (unsigned int i = 0; i < storeStationID.size(); i++) { 	// pushing To station IDs into a vector 
		stationMap.search(storeStationID[i], sd, HashStationID);

		// convert distances given lat1, long1, lat2, long2
		double dist2Points = distBetween2Points((double)atof(lat1.c_str()), (double)atof(long1.c_str()), (double)atof(sd.Latitude.c_str()), (double)atof(sd.Longitude.c_str()));

		// push distances into trip To vector
		if (dist2Points <= (double)atof(dist.c_str()))
			tripToVect.push_back(sd.StationID);	
	}
}


//
// similarCommand
//
// Function that is used when the user requests for similar data. Returns 
// nearby stations that occur at start & end trips that are within a 
// certain range. Also returning the number of possible trips within that
// range.
//
void similarCommand(string s, hashmap<string, TripData>& tripMap, 
	hashmap<string, StationData>& stationMap, vector<string>& storeStationID, 
	vector<string>& storeTripID) {

	stringstream mystream(s);	// create string stream

	string tripid, dist, fromID, toID, lat1, long1; 		// splice string and save information into variables

	getline(mystream, tripid, ' ');
	getline(mystream, dist, ' ');

	cout << "Trips that follow a similar path (+/-" << dist << " miles) as " << tripid << endl;

	vector<string> tripFromVect;	// vector to hold From Station IDs (start)
	vector<string> tripToVect;		// vector to hold To Station IDs (end)

	TripData td;	// create an instance of the struct
	StationData sd;	// create an instance of the struct

	bool check = checkTrip(tripMap, stationMap, tripid, lat1, long1, fromID);

	if (!check) 
		return;

	updateFromVect(storeStationID, stationMap, tripFromVect, lat1, long1, dist);

	tripMap.search(tripid, td, HashTripID); 	// store StationData, Lat/Long for To Station ID
	toID = td.To;

	stationMap.search(td.To, sd, HashStationID);

	lat1 = sd.Latitude;
	long1 = sd.Longitude;

	updateToVect(storeStationID, stationMap, tripToVect, lat1, long1, dist);

	sort(tripToVect.begin(), tripToVect.end(), sortByKeys);		// sort vectors
	sort(tripFromVect.begin(), tripFromVect.end(), sortByKeys);

	removeDuplicateVect(tripFromVect);	// remove duplicates in vector
	removeDuplicateVect(tripToVect);

	// print nearby points (start/end)
	printNearbyPoints(tripFromVect, tripToVect);

	// print trip count
	printTripCount(tripFromVect, tripToVect, storeTripID, tripMap);
}


//
// displayNumSet
//
// Output numbers for stations, trips, bikes
//
void displayNumSet(int& numStations, int& numTrips, int& numBikes) {
	cout << endl;
	cout << "# of stations: " 	<< numStations 	<< endl;
	cout << "# of trips: " 		<< numTrips 	<< endl;
	cout << "# of bikes: " 		<< numBikes 	<< endl;
	cout << endl;
}


//
// stationIDCommand
//
// Output necessary data when user inputs a station ID
//
void stationIDCommand(string& userInput, hashmap<string, StationData>& stationMap) {
	StationData sd;	// instance of struct

	// check to see if data is found
	bool found = stationMap.search(userInput, sd, HashStationID);	

	if (found) {
		cout << "Station:" 		 << endl;
		cout << " ID: " 		 << sd.StationID 	<< endl;
		cout << " Abbrev: " 	 << sd.Abbrev 		<< endl; 
		cout << " Fullname: " 	 << sd.Fullname		<< endl;
		cout << " Location: ("	 << (double)atof(sd.Latitude.c_str()) 	
			 << ", "  << (double)atof(sd.Longitude.c_str()) << ")" << endl;
		cout << " Capacity: "	 << sd.Capacity		<< endl;
		cout << " Online Date: " << sd.OnlineDate 	<< endl << endl;
	}
	else	// return with no data found
		cout << "Station not found" << endl << endl;
}


//
// tripIDCommand
//
// Output necessary data when user inputs a trip ID
//
void tripIDCommand(string& userInput, hashmap<string, TripData>& tripMap, 
	hashmap<string, StationData>& stationMap) {
	TripData td;	// instance of struct
	
	// check to see if data is found
	bool found3 = tripMap.search(userInput, td, HashTripID);

	if (found3) {
		StationData sd;
		cout << "Trip: " 				 << endl;
		cout << " ID: " 				 << td.TripID		<< endl;
		cout << " Starttime: " 			 << td.StartTime 	<< endl;
		cout << " Bikeid: " 	 		 << td.BikeID 		<< endl; 

		// convert duration from seconds to hours, minutes, seconds
		Duration(td.Duration);
		
		// search for trip From data
		tripMap.search(td.From, td, HashStationID);	
		stationMap.search(td.From, sd, HashStationID);

		cout << " From station: " 		 << sd.Abbrev 		
			 << " ("		 << td.From 	<< ")" 		<< endl;
		
		// search for trip To data
		tripMap.search(td.To, td, HashStationID);
		stationMap.search(td.To, sd, HashStationID);

		cout << " To station: "	 		 << sd.Abbrev 		
			 << " ("		 << td.To		<< ")"		
			 << endl;
		cout << " Rider identifies as: " << td.Identifies 	
			 << endl;
		cout << " Birthyear: "			 << td.BirthYear 	
		     << endl << endl;
	}
	else 	// return with no data found
		cout << "Trip not found" << endl << endl;
}


//
// bikeIDCommand
//
// Output necessary data when user inputs a bike ID
//
void bikeIDCommand(string& userInput, hashmap<string, BikeData>& bikeMap) {
	BikeData bd;	// instance of struct
	
	// check to see if data is found
	bool found4 = bikeMap.search(userInput, bd, HashBikeID);
	
	if (found4) {
		cout << "Bike: " 	<< endl;
		cout << " ID: " 	<< userInput 	<< endl;
		cout << " Usage: " 	<< bd.Usage 	<< endl << endl;
	}

	else	// return with no data found
		cout << "Bike not found" << endl << endl;
}


//
// stationAbbrevCommand
//
// Output necessary data when user inputs a station abbreviation
//
void stationAbbrevCommand(string& userInput, hashmap<string, StationData>& 
	stationMap) {
	StationData sd;	// instance of struct

	// check to see if data is found
	bool found2 = stationMap.search(userInput, sd, HashStationAbbrev);
	
	if (found2) {
		cout << "Station:" 		 << endl;
		cout << " ID: " 		 << sd.StationID 	<< endl;
		cout << " Abbrev: " 	 << sd.Abbrev 		<< endl; 
		cout << " Fullname: " 	 << sd.Fullname		<< endl;
		cout << " Location: ("	 << (double)atof(sd.Latitude.c_str()) 	
			 << ", "  << (double)atof(sd.Longitude.c_str()) << ")" << endl;
		cout << " Capacity: "	 << sd.Capacity		<< endl;
		cout << " Online Date: " << sd.OnlineDate 	<< endl << endl;
	}

	else 	// return with no data found
		cout << "Station not found" << endl << endl;	
}
