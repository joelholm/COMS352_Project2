/**
* prov-rep.cpp
* This file contains code for the prov-rep program.  The main function maps the memory of res.txt
* and then runs an infinite while loop which prompts the user to provide resources. There are two
* prompts for input.  The first asks if the user would like to provide resources (y/n).  Answering
* n will exit the progrom and answering y will reveal a second prompt. This prompt will ask the user
* for a resource and number of units to provide.  The format should be two integers with one space
* inbetween them (ex: 3 6)(this would provide 6 units from resource 3).
* This file also contains a child process which reports the status of the mapped memory.  The code
* for this reporting process is in the function reportResourceLoop.
*
* @author Joel Holm
* @date   4/18/19
*/

#include "semAndMem.h"
using namespace std;

void provideResourceLoop(int semID, char *data, int size);
void reportResourceLoop(int semID, char *data, int size);

/**
* main
* This is the main function for the alloc program.  Here, I initalize the semaphore
* and map the memory.  Then I split this process into a child and parent process.
* The parent process runs an infinite loop which prompts the user for input to
* provide resources.  THe child process reports on the status of the file res.txt.
*/
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

/**
* provideResourceLoop
* This function runs an infinite loop which prompts users for input to provide resources. Upon a request
* to provide resources, this function will enforce mutual exlusion with a semaphore by the id of semID.
*
* @param  semID - an int which represents the id of the semaphore used by alloc and prov-rep processes
*                 to ensure mutual exclusion.
* @param  data  - a pointer to a char array which represents the data mapped in my mmap in main.
* @param  size  - an int which represents the size of the data parameter.
*/
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
    if( prov.compare("y") != 0 ){
      continue;
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
        changeResourceUnits(data,size,resourceNum,newUnits);

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

/**
* This function runs an infinite loop which reports the status of the file res.txt every 10 seconds.
* This function enforces mutual exlusion of the mapped memory with a semaphore by the id of semID.
*
* @param  semID - an int which represents the id of the semaphore used by alloc and prov-rep processes
*                 to ensure mutual exclusion.
* @param  data  - a pointer to a char array which represents the data mapped in my mmap in main.
* @param  size  - an int which represents the size of the data parameter.
*/
void reportResourceLoop(int semID, char *data, int size){
  pid_t parentPid = getppid();
  struct sembuf ops;
  while(true){
    sleep(10);
    cout << endl << "========== Resource Update ==========" << endl;

    //wait
    ops = {0,-1,0};
    semop(semID,&ops,1);

    //report page size
    int pageSize = getpagesize();
    cout << "Page size: " << pageSize << endl;
    //report resource states
    cout << "Resources: " << endl;
    int i = 0;
    while( i < size ){
      cout << data[i];
      i++;
    }
    //pages in memory
    int vecSize = (size + pageSize - 1)/(pageSize);
    unsigned char vec[vecSize];
    int minCrr = mincore(data, size, vec);
    cout << "Status of pages in memory: " << endl;
    for( i = 0; i < vecSize; i++ ){
      if( vec[i] & 1 ){
        cout << "Page " << i << ": Resident" << endl;
      } else {
        cout << "Page " << i << ": Nonresident" << endl;
      }
    }

    //signal
    ops = {0,1,0};
    semop(semID,&ops,1);
  }
}
