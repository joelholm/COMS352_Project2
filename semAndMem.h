/**
* semAndMem.h
* This header file contains code to assist alloc.cpp and prov-rep.cpp in there tasks.
* The programs alloc and prov-rep share common coding problems which are solved with
* the functions getSem, mapFile, checkResourceUnits, and changeResourceUnits.
* This file also contains some data structures needed to interact with System V
* semaphores.
*
* @author Joel Holm
* @date   4/18/19
*/

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

//memory structure needed to interface with System V semaphores
union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
};

/**
* getSem
* This function goes through the necessary steps to initalize a semaphore.
* A semaphore is created and initalized to the value 1
*
* @return int - representing the id of the semaphore created
*/
int getSem(){
  key_t semKey = ftok(".",12);
  union semun arg;
  int semID = semget(semKey, 1, IPC_CREAT | 0660 );
  //set semaphore to 1
  arg.val = 1;
  semctl(semID,0,SETVAL,arg);
  return semID;
}

/**
* mapFile
* This function goes through the necessary steps to map the file res.txt to memory.
*
* @return char* - a pointer to a char array which represents the data being mapped to memory
*/
char* mapFile(){
  //Map the file
  struct stat buf;
  stat("res.txt",&buf);
  size_t fileSize = buf.st_size;
  int res = open("res.txt", O_RDWR, 0 );
  void *mmFile = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, res, 0);
  char *data = (char*)(mmFile);
  return data;
}

/**
* checkResourceUnits
* This function takes a pointer to memory mapped file - data - and finds the resource indicated
* by the parameter checkResource.  The parameter size is needed to prevent a memory leak.
* Upon finding the resource checkResource, this function returns the number of units currently
* provided by that resource.  If the resource does not exist a -1 will be returned.
*
* @param    char* data - the mapped data
* @param    int checkResource - the resource number
* @param    int size - the size of the data
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

/**
* changeResourceUnits
* This function takes a pointer to memory mapped file - data - and changes the resource indicated
* by the parameter checkResource to that of the parameter newUnits.  The parameter size is needed
* to prevent a memory leak. Upon finding the resource checkResource, this function changes the data
* of the respected resource.  Upon success the new unit assigned is returned.  Upon failure, this
* function returns -1.
*
* @param    char* data - the mapped data
* @param    int checkResource - the resource number
* @param    int size - the size of the data
* @param    int newUnits - the new unit value to be written
* @return   int - the number of this resource type which are available.
*               returns -1 if the resource doesn't exist.
*/
int changeResourceUnits(char* data, int size, int checkResource, int newUnits){
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
