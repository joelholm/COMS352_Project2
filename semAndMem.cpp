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
