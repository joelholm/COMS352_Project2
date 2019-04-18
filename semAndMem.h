#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>

using namespace std;

#ifndef semAndMem
#define semAndMem

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
};

int getSem();
char* mapFile();

int getSem(){
  key_t semKey = ftok(".",12);
  union semun arg;
  int semID = semget(semKey, 1, IPC_CREAT | 0660 );
  //set semaphore to 1
  arg.val = 1;
  semctl(semID,0,SETVAL,arg);
  return semID;
}

char* mapFile(){
  //Map the file
  struct stat buf;
  stat("res.txt",&buf);
  size_t fileSize = buf.st_size;
  int res = open("res.txt", O_RDWR, 0 );
  void *mmFile = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, res, 0);
  char *data = (char*)(mmFile);


  return data;

  // msync(mmFile,fileSize,0);
  // munmap(mmFile,fileSize);
  // close(res);
}

//function which vertifies that a resource exists
/**
*
* @param    char* data - the mapped data
*           int checkResource - the resource number
*           int size - the size of the data
* @return   int - the number of this resource type which are available.
*               returns -1 if the resource doesn't exist.
*/
int checkResourceUnits(char* data, int size, int checkResource){
  int resource = -1, units = -1;

  //go through each resource line by scanning one character at a time
  int i = 0;
  while( i < size ){
    //if this character is an int and resource is not -1
    if( resource != -1 && isdigit(data[i]) ){
      //then set it to units
      units = data[i] - '0';
      //if this resource matches checkResource
      if( checkResource == resource ){
        return units;
      }
    }
    //if this character is an int and resource is -1
    if( resource == -1 && isdigit(data[i]) ){
      //then set it to the resource num
      resource = data[i] - '0';
    }
    //if this character is the return character
    if( data[i] == '\n' ){
      //then reset resource and units
      resource = -1;
      units = -1;
    }
    i++;
  }

  //couldn't find it return -1
  return -1;
}

int changeResouceUnits(char* data, int size, int checkResource, int newUnits){
  //validate that new units is 0-9
  if( newUnits < 0 || newUnits > 9 ){
    return -1;
  }
  int resource = -1, unitsLoc = -1;

  //go through each resource line by scanning one character at a time
  int i = 0;
  while( i < size ){
    //if this character is an int and resource is not -1
    if( resource != -1 && isdigit(data[i]) ){
      //then set it to units
      unitsLoc = i;
      //check if the resource matches
      if( resource == checkResource ){
        //change the units and return the units changed
        data[unitsLoc] = (char)('0' + newUnits);
        return newUnits;
      }
    }
    //if this character is an int and resource is -1
    if( resource == -1 && isdigit(data[i]) ){
      //assign resouce
      resource = data[i] - '0';
    }
    //if this character is the return character
    if( data[i] == '\n' ){
      //then reset resource and units
      resource = -1;
      unitsLoc = -1;
    }
    i++;
  }
  return -1;
}

#endif
