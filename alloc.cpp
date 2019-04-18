/**
* alloc.cpp
* @author Joel Holm
* @date   4/11/19
*/
#include "semAndMem.h"

using namespace std;

void allocateResourceLoop(int semID, char *data, int size);

int main(){

  //create semaphore
  int semID = getSem();

  //map data
  struct stat buf;
  stat("res.txt",&buf);
  size_t fileSize = buf.st_size;
  char* data = mapFile();


  allocateResourceLoop(semID, data, fileSize);


  /**
  * Thing To Do
  *
  *   OK, mutual exclusion should be down.  Just need to write some methods to change and retrieve memory.
  *
  *   Github to pyrite
  */



  return 0;
}

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
        changeResouceUnits(data,size,resourceNum,newUnits);

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
