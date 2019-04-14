/**
* alloc.cpp
* @author Joel Holm
* @date   4/11/19
*/
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#define MAX_STRL 10

using namespace std;

void mapFile();
void allocateResourceLoop();

int main(){

  //mapFile();
  semget(99,2,0);


  allocateResourceLoop();


  /**
  * Thing To Do
  *

      HEY, GO TO CANVAS AND CHECK OUT THAT DEMO FILE THING, WILL PROBABLY HELP ALOT

  *   Makefile
  *   Semaphores
  *   Mutual Exclusion of printing to screen for prov-rep
  *   Github to pyrite
  */



  return 0;
}

void allocateResourceLoop(){
  //set up a loop asking if resources need to be added
  string numUnits,resourceNum;
  while(true){
    printf("Allocate Resources (Enter q to exit)\n");
    cout << "Resource type: ";
    cin >> resourceNum;
    if( resourceNum.compare("q") == 0 ){ break; }
    cout << "How many units: ";
    cin >> numUnits;
    if( numUnits.compare("q") == 0 ){ break; }

    cout << "Giving " << numUnits << " resources to resource " << resourceNum << "\n\n";

  }
}


void mapFile(){
  //Map the file
  struct stat buf;
  stat("res.txt",&buf);
  size_t fileSize = buf.st_size;
  int res = open("res.txt", O_RDWR, 0 );
  void *mmFile = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, res, 0);
  char *data;
  data = (char*)(mmFile);

  printf("FD: %d\tMMFile: %d\n",res,(mmFile != MAP_FAILED));

  printf("Pointer: %p\n", mmFile);

  data[4] = 'f';

  printf("String: %c\n",data[4]);

  msync(mmFile,fileSize,0);

  munmap(mmFile,fileSize);
  close(res);
}



//semop man page
