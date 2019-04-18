/**
* alloc.cpp
* This file contains code for the allocation program.  The main function maps the memory of res.txt
* and then runs an infinite while loop which prompts the user to allocate resources.  There are two
* prompts for input.  The first asks if the user would like to allocate resources (y/n).  Answering
* n will exit the progrom and answering y will reveal a second prompt. This prompt will ask the user
* for a resource and number of units to allocate.  The format should be two integers with one space
* inbetween them (ex: 3 6)(this would allocate 6 units from resource 3).
* @author Joel Holm
* @date   4/18/19
*/

#include "semAndMem.h"
using namespace std;

void allocateResourceLoop(int semID, char *data, int size);

/**
* main
* This is the main function for the alloc program.  Here, I initalize the semaphore
* and map the memory.  Then I enter into the resource allocation loop with the
* allocateResourceLoop function.
*/
int main(){
  //create semaphore
  int semID = getSem();

  //map data
  struct stat buf;
  stat("res.txt",&buf);
  size_t fileSize = buf.st_size;
  char* data = mapFile();
  allocateResourceLoop(semID, data, fileSize);

  //unmap
  munmap(data,fileSize);

  return 0;
}

/**
* allocateResourceLoop
* This function runs an infinite loop which prompts users for input to allocate resources. Upon a request
* to allocate resources, this function will enforce mutual exlusion with a semaphore by the id of semID.
*
* @param  semID - an int which represents the id of the semaphore used by alloc and prov-rep processes
*                 to ensure mutual exclusion.
* @param  data  - a pointer to a char array which represents the data mapped in my mmap in main.
* @param  size  - an int which represents the size of the data parameter.
*/
void allocateResourceLoop(int semID, char *data, int size){
  //set up a loop asking if resources need to be added
  string numUnits, alloc;
  string units, resource;
  struct sembuf ops;
  while(true){
    cout << "Allocate Resources (y/n) ";
    cin >> alloc;
    if( alloc.compare("n") == 0 ){
      break;
    }
    if( alloc.compare("y") != 0 ){
      continue;
    }
    cout << "Enter type and number of resources: ";
    cin >> resource;
    cin >> units;
    int resourceNum = stoi(resource,nullptr,10), unitsNum = stoi(units,nullptr,10);

    //mutual exclusion
    //wait
    ops = {0,-1,0};
    semop(semID,&ops,1);

    //check if changes are legal
    int availableUnits = checkResourceUnits(data, size, resourceNum);
    if( availableUnits != -1 ){
      int newUnits = availableUnits - unitsNum;
      if( newUnits >= 0 ){
        //make changes to mmaped data
        changeResourceUnits(data,size,resourceNum,newUnits);
        //sync back to disk
        msync((void*)data,size,0);

        cout << "Allocating " << unitsNum << " units from resource " << resourceNum << "\n\n";
      } else {
        cout << "Not enough units exist of resource " << resourceNum << endl;
      }
    } else {
      cout << "No such resource, no changes made" << endl;
    }

    //signal
    ops = {0,1,0};
    semop(semID,&ops,1);

  }//end of while
}
