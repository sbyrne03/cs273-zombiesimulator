//////////////////////////////////////////////////////////
// Group: Sophie Byrne, Lotanna Akukwe, Brodie Gibson
// Project: Final Project - Zombie Apocalypse
// Date: 12/ /22
// Sources: 
//////////////////////////////////////////////////////////

#include <map>
#include "citizen.h"
#include "sim.h"
#include <iostream>
#include <fstream>


int main(){
    
    std::cout << "Welcome to the Simulation! " << std::endl;
    std::string filename = "citizens.txt";
    
    std::cout<<"Enter How Many Days you would like to run: " << std::endl;
    int duration;
    std::cin >> duration;
    Simulation ZombieApocalypse(filename,duration);

}
