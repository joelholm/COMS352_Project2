#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstring>
#include <time.h>
#include <stdio.h>

using namespace std;

int main(){

  cout << "Enter File Paths for two documents" << endl;
  cout << "Lower Case File: ";
  string lPath;
  cin >> lPath;
  cout << "To Upper Case File: ";
  string uPath;
  cin >> uPath;

  char lPathc[lPath.size() + 1];
  strcpy(lPathc, lPath.c_str() );

  char uPathc[uPath.size() + 1];
  strcpy(uPathc, uPath.c_str() );


  //start time with open,read,write,close
  clock_t startTime = clock();

  //open both files
  int lfd = open(lPathc, O_RDONLY);
  int ufd = open(uPathc, O_WRONLY);

  //read lower case file
  struct stat lbuf;
  fstat(lfd,&lbuf);
  size_t lSize = lbuf.st_size;
  char lFileContent[lSize];
  read(lfd,lFileContent,lSize);

  //convert to upper case
  int i = 0;
  for( ; i < lSize; i++){
    lFileContent[i] = toupper(lFileContent[i]);
  }

  //write to upper case file
  write(ufd,lFileContent,lSize);

  close(lfd);
  close(ufd);

  clock_t endTime = clock();
  clock_t totalTime = endTime - startTime;
  cout << "open, read, write, and close Total Time: " << totalTime << endl;


  return 0;
}
