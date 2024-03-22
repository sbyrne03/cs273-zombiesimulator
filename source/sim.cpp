#include "citizen.h"
#include "sim.h"
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <ctime>
#include <algorithm>
using std::string;



Simulation::Simulation(string file_name, int dur) : f_name(file_name){ //Simulation Constructor
    populate(f_name);
    outPut();
    createDistricts();
    setDistAccess(districts);
    std::ofstream dataOutput("graphData.csv");
    dataOutput.open("graphData.csv");
    int counter = 0;
    if(dataOutput.is_open()){
        //std::cout<<"works" << std::endl;
        for(int i = 0; i < dur; i++){ //day cycle
            //std::cout << "day works" <<std::endl;
            for(int j = 0; j < 24; j++){ //hour cycle - 1 tic
            //std::cout << "hour works" <<std::endl;
                events = setQueue();
                //std::cout << events.size() << std::endl;
                while(events.size() > 0){
                    //std::cout << counter++ <<std::endl;
                    Event(events.front()+1);
                    events.pop();
                    //std::cout << "popped" << std::endl;
                }
                numTimeTicks++;
                dataOutput << numTimeTicks << "," << totalNumIgnorant << "," << totalNumAlarmed << ",";
                dataOutput << totalNumZombies << "," << totalNumDoctors <<","<< std::endl;
            }
            //File out data, numof Infected, Alarmed etc.
            //tick, ignorant, alarmed, zombies, doctors into csv
        }
    }
}
//this function will house the interaction between the person at the front of the queue
//and the people that are eventually selected to interact with this person
//it will also hold the interaction chance of moving between districts --- but not yet.
//it takes in the ID from the queue, searches our location list, finds which district, accesses the district
//accesses the map within the district and then locates the person.
 void Simulation::Event(int p_id){
    Citizen* person1 = findPerson(p_id);
    //std::cout << "find starts" <<std::endl;
    for(int i = 0; i < 10; i++){//will interact with 9-10 people
        //move causes out of range error
        //if(person1->move()){ //3% chance for true
        //    moveCitizen(matchDistrict(person1->getLocation()),person1); //moves person to accessible district
        //}
        Citizen* person2 = pickPerson(person1->getLocation()); //picks a person to interact with based on location
        //std::cout << "second find starts" << std::endl;
        int temp1 = person1->getPersonType(); //gets the type of person1
        int temp2 = person2->getPersonType(); //gets the type of person2
        person1->action(person2); // person 1 acts on person 2
        person2->action(person1); //person 2 acts back on person 1
        // if first person changes
        if(person1->getPersonType() != temp1){
            if(person1->getPersonType() == 1){ //checks current type and adds accordingly
                totalNumIgnorant++;
            }
            else if(person1->getPersonType() == 2){
                totalNumAlarmed++;
            }
            else if(person1->getPersonType() == 3){
                totalNumZombies++;
            }
            if(temp1 == 1){ //checks previous type and subtracts accordingly
                totalNumIgnorant--;
            }
            else if(temp1 == 2){
                totalNumAlarmed--;
            }
            else if(temp1 == 3){
                totalNumZombies--;
            }
            else if(temp1 == 4){
                totalNumDoctors--;
            }
        }
        // if second person changes
        if(person2->getPersonType() != temp2){
            if(person2->getPersonType() == 1){ //checks current type and adds accordingly
                totalNumIgnorant++;
            }
            else if(person2->getPersonType() == 2){
                totalNumAlarmed++;
            }
            else if(person2->getPersonType() == 3){
                totalNumZombies++;
            }
            if(temp1 == 1){ //checks previous type and subtracts accordingly
                totalNumIgnorant--;
            }
            else if(temp1 == 2){
                totalNumAlarmed--;
            }
            else if(temp1 == 3){
                totalNumZombies--;
            }
            else if(temp1 == 4){
                totalNumDoctors--;
            }
        }
     }
 }


Citizen* Simulation::findPerson(int id){
    string s = peopleLocations.at(id); //returns location of person
    //std::cout << s << std::endl;
    District d = matchDistrict(s); //finds their district
    //std::cout << d.getName() <<std::endl;
    std::map<int, Citizen*> map = d.getCitizens();
    std::map<int, Citizen*>::iterator it;
    it = map.find(id); //moves iterator to the citizen
    Citizen* person1 = it->second;
    return person1;
}

void Simulation::outPut(){
    std::ofstream outfile;
    outfile.open("citOut.txt");
    // allCitizens2.insert(allCitizens2.begin(), populate("citizens.txt").begin(), populate("citizens.txt").end());
    for(int i = 0; i < allCitizens.size(); i++){
        outfile << allCitizens[i]->getName() << " " << allCitizens[i]-> getAge() << std::endl;
    }
}

void Simulation::createDistricts(){
    std::map<int,Citizen*> tempMap;
    string names[4]{"Burbs","Downtown","Medical","College"}; //0 Burbs, 1 Downtown, 2 Medical, 3 College
    
    for(int i = 0; i <4; i++){ //loop to subdivide totalpeople into district vectors to be made into maps
        tempMap = make_maps(subDivide(allCitizens,i));
        //std::cout << tempMap.size() << std::endl;
        District temp {names[i], tempMap.size()+1, tempMap};
        districts.push_back(temp);
    }
}
//takes in a vector of all people, and subdivides them into maps made for each district.
//the person being assigned is also told what district they are going into (string location)
std::vector<Citizen*> Simulation::subDivide( std::vector<Citizen*> All_people, int district_Num){
    std::vector<Citizen*> sub_list;
    if(district_Num == 0){ //district 0 gets people with id's between 0 and 399  "Burbs"
        for(int i = 0; i < 400; i++){ //0-399
            All_people.at(i)->setLocation("Burbs"); //tells person their location
            updatePeopleLocations(All_people.at(i)->getID(),All_people.at(i)->getLocation()); //tells sim where person is
            sub_list.push_back(All_people.at(i));
        }
    } else if (district_Num == 1){ //district 1 gets people with id's between 400 and 899  "Downtown"
        for(int i = 400; i < 900; i++){
            All_people.at(i)->setLocation("Downtown");  //tells person their location
            updatePeopleLocations(All_people.at(i)->getID(),All_people.at(i)->getLocation()); //tells sim where person is
            sub_list.push_back(All_people.at(i));
        }
    } else if (district_Num == 2){ //district 2 gets people with id's between 900 and 1599  "Medical"
        for(int i = 900; i < 1600; i++){
            All_people.at(i)->setLocation("Medical");  //tells person their location
            updatePeopleLocations(All_people.at(i)->getID(),All_people.at(i)->getLocation()); //tells sim where person is
            sub_list.push_back(All_people.at(i));
        }
    } else if(district_Num == 3){//district 3 gets people with id's between 1600 and 1999  "College"
        for(int i = 1600; i < 2000; i++){
            All_people.at(i)->setLocation("College");  //tells person their location
            updatePeopleLocations(All_people.at(i)->getID(),All_people.at(i)->getLocation()); //tells sim where person is
            sub_list.push_back(All_people.at(i));
        }
    }
    return sub_list;
}

void Simulation::populate(string file_name){
    std::ifstream filepath;
    filepath.open(file_name);
    string name;// holds name of person
    int age; //holds age of person
    for(int i = 0; i < 2000; i++){
        filepath >> name; //reads in name from file
        filepath >> age; //reads in age from file
        if(i>900 && i<910){
            Citizen* temp2 = new Doctor(name,age); //makes a citizen* and points it to a new ignorant
            allCitizens.push_back(temp2); // add citiZen to member variable vector
        }
        else if(i>911 && i<915){
            Citizen* temp2 = new Alarmed(name,age); //makes a citizen* and points it to a new ignorant
            allCitizens.push_back(temp2); // add citiZen to member variable vector
        }
        else{
            Citizen* temp2 = new Ignorant(name,age); //makes a citizen* and points it to a new ignorant
            allCitizens.push_back(temp2); // add citiZen to member variable vector
        }
        //we may need to delete temp2 here to prevent a data leak, but im not sure
    }
    //std::cout<< "populate done" << std::endl;
    return;
}

std::map<int,Citizen*> Simulation::make_maps(std::vector<Citizen*> vect){
    std::map<int,Citizen*> map1;
    std::pair<int,Citizen*> p;
    for(int i = 0; i < vect.size()-1; i++){
        p.first = vect.at(i)->getID();
        p.second = vect.at(i);
        map1.insert(p);
    }
    return map1;
}

void Simulation::makeFirstInfected(){
    srand(time(NULL));//make someone a zombie
    int ZombId = rand()%(700-1)+900; //random id for citizen turned zombie
    District d = matchDistrict("Medical Hill");
    std::map<int, Citizen*> c = d.getCitizens(); //get citizens from medical hill
    Citizen* temp = c[ZombId]; //temp pointer to citizen
    delete c[ZombId]; //delete pointer to citizen
    c[ZombId] = new Zombie(temp->getName(), temp->getAge(), ZombId); //new pointer to Zombie object
    if(c[ZombId]->getPersonType() == 1){
        totalNumIgnorant--;
    }
    if(c[ZombId]->getPersonType() == 2){
        totalNumAlarmed--;
    }
    if(c[ZombId]->getPersonType() == 3){
        totalNumDoctors--;
    }
    delete temp; //release temp pointer
    //find person with ^ matching id, and turn into zombie
    totalNumZombies++;
}

Citizen* Simulation::pickPerson(std::string distName ){
    District d = matchDistrict(distName); //finds district corresponding to string
    std::map<int, Citizen*> c = d.getCitizens(); //grabs the map corresponding to the district
    int idTemp = 0;
    //srand(time(NULL));
    if(distName == "Burbs"){ //district 0 gets people with id's between 0 and 399  "Burbs"
        idTemp = rand()%399+1;
    } else if (distName == "Downtown"){ //district 1 gets people with id's between 400 and 899  "Downtown"
        idTemp = rand()%499+401;
    } else if (distName == "Medical"){ //district 2 gets people with id's between 900 and 1599  "Medical"
        idTemp = rand()%699+901;
    } else if(distName == "College"){//district 3 gets people with id's between 1600 and 1999  "College"
        idTemp = rand()%399+1601;
    }
    //std::cout << "pick person " << idTemp << std::endl;
    return c.at(idTemp);

    //int temp = rand()%c.size()-1; //picks a number between 0 and size of citizen multimap minus 1
    //std::map<int, Citizen*>::iterator it = c.begin(); //puts iterator at beginning of map
    //for(int i = 0;i < temp; i++){ //iterates through until it gets to the index
    //    ++it;
    //}
    //return  //returns the citizen from that index
}
//this function serves as a record of where people are, in order to make searching easier
void Simulation::updatePeopleLocations(int id,string loc){
    if(peopleLocations.find(id) != peopleLocations.end()){ //if it finds id before end of map
        peopleLocations.at(id) = loc; //update the location of said id
        return;
    }else{
        std::pair<int,string> p;
        p.first = id;
        p.second = loc;
        peopleLocations.insert(p); //insert data into map
        return;
    }
}

Simulation::District& Simulation::matchDistrict(string distName){
    if(distName == "Burbs"){
        return districts.at(0);
    }else if (distName == "Downtown"){
        return districts.at(1);
    }else if (distName == "Medical"){
        return districts.at(2);
    }else if (distName == "College"){
        return districts.at(3);
    }else return districts.at(0);
}
//d represents district citizen is moving from, c represents the citizen themselves
void Simulation::moveCitizen(District d, Citizen*& c){
    std::vector<string> s = dist_access.at(c->getLocation()); //the vector of available locations
    string temp;
    if(c->getID()%2 == 0){
        temp = s.at(0);
    }else{
        temp = s.at(1);
    }
    //District newD = 
    matchDistrict(temp).addCitizen(c,c->getID()); //adds citizen to new map
    c->setLocation(temp);
    updatePeopleLocations(c->getID(), temp);
    d.deleteCitizen((c->getID())); //removes citizen from old map
    return;
}

std::map<string,std::vector<string>> Simulation::setDistAccess(std::vector<Simulation::District> d){
    std::map<string,std::vector<string>> access_map;
    std::vector<string> temp, temp2, temp3, temp4;
    temp.push_back("Downtown");
    temp.push_back("College");
    temp2.push_back("Burbs");
    temp2.push_back("Medical");
    temp3.push_back("Downtown");
    temp3.push_back("College");
    temp4.push_back("Burbs");
    temp4.push_back("Medical");
    std::pair<string, std::vector<string>> p;
    p.first = d.at(0).getName(); //Burbs
    p.second = temp;//accessible from burbs
    access_map.insert(p);
    p.first = d.at(1).getName(); //Downtown
    p.second = temp2;//accessible from downtown
    access_map.insert(p);
    p.first = d.at(2).getName(); //Medical
    p.second = temp3;//accessible from medical
    access_map.insert(p);
    p.first = d.at(3).getName(); //College
    p.second = temp4;//accessible from college
    access_map.insert(p);
    return access_map;
}

std::queue<int> Simulation::setQueue(){
    std::vector<int> v;
    std::queue<int> q;
    //for(int i = 0; i < 2000; i++){
    //    v.push_back(i);
    //}
    // std::random_shuffle(0,1999,&v);
    for(int i = 0; i < 2000; i++){
        q.push(i);
    }
    return q;
}

// should be called when a person is moving districts to update their location
// void Simulation::updatePeopleLocations(int ID, string new_location){
//     std::map<int,string>::iterator it;
//      it = peopleLocations.find(ID);
//     (*it).second= new_location;
//     return;
// }
////////////////////////////////////////District Implementation
//default constructor
Simulation::District::District(){
    name = "none";
    numPeople = 0;
}
// argumented constructor
Simulation::District::District(std::string n, int nP, std::map<int, Citizen*> c){
    name = n;
    numPeople = nP;
    citizens = c;
}
// return number of people in a district
int Simulation::District::getNumPeople(){
    return numPeople;
}
std::map<int, Citizen*> Simulation::District::getCitizens(){
    return citizens;
}  
// method to return num of zombies within a district
int Simulation::getNumZombies(){
    return totalNumZombies;
}
//method to get the num of alarmed within a district
int Simulation::getNumAlarmed(){
    return totalNumAlarmed;
}
//method to get the num of ignorant within a district
int Simulation::getNumIgnorant(){
    return totalNumIgnorant;
}
// method to get the num of doctors within a district
int Simulation::getNumDoctors(){
    return totalNumDoctors;
}
// NEEDS WORK Method to add a citizen to the multimap within the district
void Simulation::District::addCitizen(Citizen* c, int id){
    citizens.insert(std::make_pair(id, c));
    return;
}
// NEEDS WORK method to delete (move) a citizen from district class
void Simulation::District::deleteCitizen(int id){
    delete citizens[id];
    citizens.erase(id);
    return;
}
// Function to print the names of citizens in a district
void Simulation::District::printCitizens(){
    std::map<int, Citizen*>::iterator it; //iterator for multimap
    it = citizens.begin();
    while (it != citizens.end()){
        std::cout << it->second->getName();
        ++it;
    }
    return;
}

string Simulation::District::getName(){
    return name;
}