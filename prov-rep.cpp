/**
* prov-rep.cpp
* @author Joel Holm
* @date   4/11/19
*/

#include "semAndMem.h"

using namespace std;

void provideResourceLoop(int semID, char *data, int size);
void reportResourceLoop(int semID, char *data, int size);

int main(){

  //Set up semaphore
  int semID = getSem();

  //Set up File mapping
  struct stat buf;
  stat("res.txt",&buf);
  size_t fileSize = buf.st_size;
  char* data = mapFile();

  struct sembuf ops;

  int pid = fork();

  if( pid == 0 ){
    //create child to report
    reportResourceLoop(semID, data, fileSize);
  } else if( pid > 0 ){
    //parent will provide resources
    provideResourceLoop(semID, data, fileSize);

    //kill child proccess
    ops = {0,-1,0};
    semop(semID,&ops,1);
    kill(pid, SIGKILL);
    ops = {0,1,0};
    semop(semID,&ops,1);

    //unmap
    munmap(data,fileSize);
  } else {
    cout << "Fork Failed" << endl;
  }

  return 0;
}

void provideResourceLoop(int semID, char *data, int size){
  //set up a loop asking if resources need to be added
  string prov,units,resource;
  struct sembuf ops;
  while(true){
    cout << "Provide Resources (y/n) ";
    cin >> prov;
    if( prov.compare("n") == 0 ){
      break;
    }
    cout << "Enter type and number of resources: ";
    cin >> resource;
    cin >> units;

    int resourceNum = stoi(resource,nullptr,10), unitsNum = stoi(units,nullptr,10);

    //wait
    ops = {0,-1,0};
    semop(semID,&ops,1);

    //check if changes are legal
    int availableUnits = checkResourceUnits(data, size, resourceNum);
    if( availableUnits != -1 ){
      int newUnits = availableUnits + unitsNum;
      if( newUnits < 10 ){
        //make changes to mmaped data
        changeResouceUnits(data,size,resourceNum,newUnits);

        //sync back to disk
        msync((void*)data,size,0);

        cout << "Providing " << unitsNum << " units to resource " << resourceNum << "\n\n";
      } else {
        cout << "Cannot provide to resource " << resourceNum << " because that would excede 9 units" << endl;
      }
    } else {
      cout << "No such resource, no changes made" << endl;
    }

    //signal
    ops = {0,1,0};
    semop(semID,&ops,1);
  }
}

void reportResourceLoop(int semID, char *data, int size){
  pid_t parentPid = getppid();
  while(true){
    //check if child dies
    cout << endl << "child update" << endl;

    //write all the stuff
    //TODO:

    sleep(10);
  }
}
