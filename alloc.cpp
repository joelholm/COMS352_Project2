/**
* alloc.cpp
* @author Joel Holm
* @date   4/11/19
*/
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_STRL 10

int main(){
  printf("Hello World!\n");

  //Map the file
  struct stat buf;
  stat("res.txt",&buf);
  size_t fileSize = buf.st_size;
  int res = open("res.txt", O_RDONLY, 0 );
  void *mmFile = mmap(NULL, fileSize, PROT_READ, MAP_PRIVATE, res, 0);
  char data[100];
  data = static_cast<char[100]>(mmFile);

  //printf("FD: %d\tMMFile: %d\n",res,(mmFile != MAP_FAILED));

  //printf("Pointer: %d\tNullThing: %d\n", mmFile, MAP_FAILED);

  //printf("String: %d\n",mmFile[0]);


  return 0;
}



//semop man page
