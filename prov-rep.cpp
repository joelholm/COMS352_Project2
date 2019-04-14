/**
* prov-rep.cpp
* @author Joel Holm
* @date   4/11/19
*/
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>  //might remove
#include <signal.h>

using namespace std;

void provideResourceLoop();
void reportResourceLoop();
int extractResource(string str);
int extractUnit(string str);

int main(){

  //Set up File mapping
  //TODO:



  sem_t outSem;
  //sem_init(outSem, 0, );    //idk doesn't work ...

  int pid = fork();

  if( pid == 0 ){
    //create child to report
    reportResourceLoop();

  } else if( pid > 0 ){
    //parent will provide resources
    provideResourceLoop();
  } else {
    cout << "Fork Failed" << endl;
  }

  // cout << "the end" << endl;
  sleep(1);

  return 0;
}

void provideResourceLoop(){
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

  }
}

void reportResourceLoop(){
  int i = 0;
  pid_t parentPid = getppid();
  while(true){

    //check if child dies
    cout << "child update" << endl;
    sleep(2);

    //check if parent proccess is alive

    int killResult = kill(parentPid,0);
    cout << "kill command result: " << killResult << endl;
    if( killResult == -1 ){
      cout << "Parent dead, sad, leaving now" << endl;
      break;
    }

    //sem wait

    // cout << "Here in pr, press enter to continue (q to exit)";
    // string str;
    // cin >> str;
    // cout << "You entererd: " << str;

    //sem signal


    // if( str.compare("q") == 0 ){
    //   break;
    // }
    if( i > 5){
      cout << "exiting by default" << endl;
      break;
    }
    i++;
  }

}
