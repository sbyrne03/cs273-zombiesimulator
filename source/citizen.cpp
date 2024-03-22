#include "citizen.h"

#include <iostream>
#include <fstream>
#include <typeinfo>
using std::string;


int Citizen::getID(){
    return idNum;
}

int Citizen::getPersonType(){
    return personType;
}

std::string Citizen::getName(){
    return name;
}

void Citizen::setName(string name_){
    name = name_;
}

std::string Citizen::getLocation(){
    return location;
}

void Citizen::setLocation(string l){
    location = l;
}

int Citizen::getAge(){
    return age;
}

bool Citizen::move(){
    int chance  = rand()%100;
    if(chance > 97){
        return true;
    }else{
        return false;
    }
}
///chance of infecting/effecting others
int Citizen::getInfectionRate(){
    if((this->getAge() > 50)||(this->getAge()< 14)){
        //make lower chance of infection
        return -20;
    }else{
        //make higher chance of infection
        return 20;
    }
}

//chance of surviving/resisting a event
int Citizen::getSurvivalRate(){
    if(this->personType == 1){ //ignorant survival rate
        return 0;
    } else if(this->personType == 2){ //alarmed survival rate
        return 40;
    } else if(this->personType == 4){ //doctor survival rate
        return 35;
    } else if(this->personType == 3){ //zombie survival rate
        return 0;
        //against doctor
    }
}
// 1 - Ignorant, 2 - Alarmed, 3 - Zombie, 4 - Doctor
void Citizen::setPersonType(int t){
    personType = t;
}