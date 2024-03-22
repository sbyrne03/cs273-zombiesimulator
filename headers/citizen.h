/*
This class will be the base class for all the different kinds of citizens
such as ignorant, alarmed, infected, etc.
*/
#ifndef CITIZEN_H
#define CITIZEN_H

#include <string>
#include <iostream>
//#include <map>
using std::string;

static int totalID = 0;
class Citizen
{
private:
    string name;
    int age;
    int idNum;
    string location;
    int loc; //1-burbs, 2- downtown, 3- medical, 4- college
    int personType; // 1 - Ignorant, 2 - Alarmed, 3 - Zombie, 4 - Doctor
    int infection_rate; //determined by getInfectionRate function which bases of age
    int survival_rate; //determined by class
    
public:
    //static int totalID = 0; //static int, remains after the function is called.
    Citizen() : name("Unknown"), age(20), idNum(0) {} // Default constructor
    Citizen(string n, int a, int ID = ++totalID) : name(n), age(a), idNum(ID) {} //argumented constructor (name, age, ID)
    string getName(); //returns name
    void setName(string name_);
    string getLocation(); //returns location
    void setLocation(string l); //updates citizen location
    int getAge();   //returns age
    int getID();    //return id
    int getPersonType(); // 1 - Ignorant, 2 - Alarmed, 3 - Zombie, 4 - Doctor
    bool move(); //if it returns true the person will move before their next action, if false does not move
    //int getInfectionRate() = 0; //determined by class
    virtual void action(Citizen*&) = 0;
    int getInfectionRate();
    int getSurvivalRate();
    void setPersonType(int);  // 1 - Ignorant, 2 - Alarmed, 3 - Zombie, 4 - Doctor
};

// Ignorant class is child class of Citizen and does not have any special action
class Ignorant : public Citizen
{
    string name;
    int age;
    int idNum;
    string location;
    int infection_rate;
    int survival_rate;
public:
    Ignorant(string n, int a, int ID = ++totalID) : Citizen(n, a,ID) {
        name = n, age = a; idNum = ID;
        setPersonType(1);
        //infection_rate = InfectionR;
    }
    
    void action(Citizen*& person2){ //action for ignorant is nothing
        //std::cout << "Hello, I'm a citizen" << std::endl;
    }
    // somehow interact with a certain number of people?
    // based random id's within the area ig is in.
    // interaction with ig, results in nothing
    // interaction with alarmed = chance of becoming alarmed
    // interaction with infected = chance of becoming infected
    //-> influenced by age of both parties on surv rate of infection attack /// and rate of infection respectively
    //}
    //int getInfectionRate();
};

// Zombie class will be child of Citizen class and be able to infect other types of citizens
class Zombie : public Citizen
{
    string name;
    int age;
    int idNum;
    string location;
    //int infection_rate;
    //int survival_rate;
public:
    Zombie(string n, int a, int ID = ++totalID) : Citizen(n, a,ID) {
        name = n, age = a; idNum = ID;
        setPersonType(3);
    }
    void action(Citizen*& person2){ // action for zombie is infecting another person
        std::cout << "RAUUUGH" << std::endl;
        if(person2->getPersonType() == 4 || person2->getPersonType() == 2 || person2->getPersonType() == 1){
            // use infection rate and survival rate to see chances of infection
            int totalChance = person2->getInfectionRate() + person2->getSurvivalRate();
            if(rand()%100-1 <= totalChance){
                std::string name = person2->getName();
                int age = person2->getAge();
                int ID = person2->getID();
                delete person2;
                person2 = new Zombie(name, age, ID);
                person2->setLocation(this->getLocation());
            }
        }
    }
};

// Alarmed class is child class of citizen and will be able to alarm ignorant citizens
class Alarmed : public Citizen {
    string name;
    int age;
    int idNum;
    string location;
    int infection_rate;
    int survival_rate;
public:
    Alarmed(){
        name = "";
        age = 1;
    }
    Alarmed(string n, int a, int ID = ++totalID) : Citizen(n,a,ID){
        name = n; age = a; idNum = ID;
        setPersonType(2);
    }
    void action(Citizen*& person2){ //action for alarmed is alarming another citizen
        std::cout << "There's a zombie apocalypse!" << std::endl;
        if(person2->getPersonType() == 1){
            if(rand()%2 == 1){ // 50/50 chance of alarming someone
                //if(chance of alarming ignorant)
                //then alarm them
                std::string name = person2->getName();
                int age = person2->getAge();
                int ID = person2->getID();
                delete person2;
                person2 = new Alarmed(name, age, ID);
                person2->setLocation(this->getLocation());
                
            }
        }
    }
};  

// Doctor class is child class of Citizen and will be able to heal zombies
class Doctor : public Citizen {
    string name;
    int age; 
    int idNum;
    string location;
    int infection_rate;
    int survival_rate;
public:
    Doctor(string n, int a, int ID = ++totalID) : Citizen(n,a,ID){
        name = n; age = a; idNum = ID;
        setPersonType(4);
    }
    void action(Citizen*& person2){ //action for Doctor is healing zombies
        std::cout << "I can heal you." << std::endl;
        person2->getAge();
        if(person2->getPersonType() == 3){
            //if(10% chance of healing the zombie)
            //then heal zombie
            if(rand()%10 == 7){ //10% chance of healing
                std::string name = person2->getName();
                int age = person2->getAge();
                int ID = person2->getID();
                delete person2;
                person2 = new Alarmed(name, age, ID);
                person2->setLocation(this->getLocation());
            }
        }
    }   
};


#endif