/**
* prov-rep.cpp
* @author Joel Holm
* @date   4/11/19
*/

#include "semAndMem.cpp"

using namespace std;

void provideResourceLoop(int semID, char *data);
void reportResourceLoop(int semID, char *data);

int main(){

  //Set up semaphore
  int semID = getSem();

  //Set up File mapping
  char* data = mapFile();

  struct sembuf ops;

  int pid = fork();

  if( pid == 0 ){
    //create child to report
    reportResourceLoop(semID, data);
  } else if( pid > 0 ){
    //parent will provide resources
    provideResourceLoop(semID, data);

    //kill child proccess
    ops = {0,-1,0};
    semop(semID,&ops,1);
    kill(pid, SIGKILL);
    ops = {0,1,0};
    semop(semID,&ops,1);
  } else {
    cout << "Fork Failed" << endl;
  }

  return 0;
}

void provideResourceLoop(int semdID, char *data){
  //set up a loop asking if resources need to be added
  string prov,units,resource;
  while(true){
    cout << "Provide Resources (y/n) ";
    cin >> prov;
    if( prov.compare("n") == 0 ){
      break;
    }
    cout << "Enter type and number of resources: ";
    cin >> resource;
    cin >> units;

    cout << "Giving " << units << " units to resource " << resource << "\n\n";

    //TODO: or implement dekkers algorithm
    //wait

    //check if changes are legal

    //make changes to mmaped data

    //sync back to disk

    //signal

  }
}

void reportResourceLoop(int semID, char *data){
  pid_t parentPid = getppid();
  while(true){
    //check if child dies
    cout << endl << "child update" << endl;

    //write all the stuff
    //TODO:

    sleep(2);
  }
}
