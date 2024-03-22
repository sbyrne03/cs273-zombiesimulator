#ifndef SIM_H
#define SIM_H

#include <string>
#include <vector>
#include <map>
#include <queue>
#include "citizen.h"

class Simulation
{
private:
    
    // District class will hold a multimap of citizens in it and will help move citizens to
    // and from the district
    class District
    {
    private:
        int numPeople;                          //Number of people
        std::string name;                       // name of district
        std::map<int, Citizen *> citizens; // key is the ID, and Citizen points to each person
    public:
        //Default Constructor
        //No parameters
        //Output: Gives default values to name of the distrinc and number of people
        District();                      
        
        /** Argumented constructors
            First parameter: name of the district
            Second parameter: number of people 
            Third parameter: a multimap of the citizens
            Output: Updates the name, number of people, and the map of citizens**/
        District(std::string n,  int nP, std::map<int, Citizen *> c); 

        /** Gets the number of people in the district
            No parameter
            Output: Returns the number of people in interger*/
        int getNumPeople();   

        /** Gets the name of this district
            No parameter
            Output: Returns the name of the district*/                            
        std::string getName();      
        
        /** Gets the people living in this district
            No parameter
            Output: Returns the people in this district*/
        std::map<int, Citizen*> getCitizens();

        /** Allows a citizen to move into this district
            First parameter: an iterator
            Second parameter: the id of the citizen
            Output: adds the person to the district */
        void addCitizen(Citizen* c, int id);

        /** Removes a citizen from the population of that district
            Parameter: An iterator
            Output: Deletes the citizen from the district*/
        void deleteCitizen(int id);

        /** prints out the citizens in the district
            No parameter*/
        void printCitizens();
    };

    ///////////basic members:
    string f_name; //name of file used for input of citizens.
    // total number of each type across whole simulation
    int totalNumZombies = 0;
    int totalNumDoctors = 10;
    int totalNumAlarmed = 1;
    int totalNumIgnorant = 1989;
    int numTimeTicks; //Keeps track of the number of time ticks the simulation runs
    ////////////data structures:
    std::queue<int> events;                                                  // holds the ID of the list of people to act;
    std::map<int,string> peopleLocations;                                    // holds an ID, location pair to find where people are                                          
    std::vector<District> districts;
    std::vector<Citizen*> allCitizens;
    std::map<string,std::vector<string>> dist_access; // string is the name of the district, district pointers are the districts that can be accessed/ moved to from this location
    ///////////functions:
    void createDistricts();
    void makeFirstInfected(); // turns 1 person into a zombie at random;
    std::map<string, std::vector<string>> setDistAccess(std::vector<District>);
    void populate(string);                       // takes in document, and creates a vector of people
    std::vector<Citizen *> subDivide(std::vector<Citizen *>, int); // takes in all people, and which district list is being formed
    std::map<int, Citizen *> make_maps(std::vector<Citizen *>);
    std::queue<int> setQueue();
    void Event(int);
    Citizen* findPerson(int);
public:
    Simulation(string file_name, int dur);
    void outPut();
    void moveCitizen(District d, Citizen*& c);
    Citizen *pickPerson(string); // returns a person to interact with for the citizen who's turn it is. string is current district
    void updatePeopleLocations(int, string);
    District& matchDistrict(string);
    int getNumZombies(); // returns number of zombie citizens in this district
    int getNumAlarmed(); // returns number of alarmed citizens in this district
    int getNumIgnorant(); // returns number of ignorant citizens in this district
    int getNumDoctors(); // returns number of doctors citizens in this district
    
};

#endif