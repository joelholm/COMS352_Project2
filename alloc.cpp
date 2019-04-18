/**
* alloc.cpp
* @author Joel Holm
* @date   4/11/19
*/
#include "semAndMem.cpp"

using namespace std;

void allocateResourceLoop(int semID, char *data);

int main(){

  //create semaphore
  int semID = getSem();

  //map data
  char* data = mapFile();

  allocateResourceLoop(semID, data);


  /**
  * Thing To Do
  *   Semaphores

    OK now, I think I've figured out semaphores. To implement just start each process (alloc and prov-rep) with
    the semaphore being initalized to 1.  This should work everytime unless it is called while a proccess
    is in the critical section.  Just implement it because it should be ok.  If you think of a better strategy
    then implement that.  ok thanks

  *   Mutual Exclusion of printing to screen for prov-rep
  *   Github to pyrite
  */



  return 0;
}

void allocateResourceLoop(int semID, char *data){
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

    cout << "Giving " << units << " units to resource " << resource << "\n\n";


    //mutual exclusion

    //wait
    ops = {0,-1,0};
    semop(semID,&ops,1);

    //check if changes are legal

    //make changes to mmaped data

    //sync back to disk

    //signal
    ops = {0,1,0};
    semop(semID,&ops,1);

    //maybe
    //just for a test, ask if a num should be changed

    //if yes ask for a num and then change it

  }
}
