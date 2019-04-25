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



  //Now do it again but with fopen, fread, fwrite, and fclose
  clock_t fstartTime = clock();

  //open both files
  FILE *flfd = fopen(lPathc, "r");
  FILE *fufd = fopen(uPathc, "w");

  //read lower case file
  struct stat flbuf;
  fstat(fileno(flfd),&flbuf);
  size_t flSize = flbuf.st_size;
  char flFileContent[flSize];
  fread(flFileContent,1, flSize, flfd);


  //convert to upper case
  i = 0;
  for( ; i < flSize; i++){
    flFileContent[i] = toupper(flFileContent[i]);
  }

  //write to upper case file
  fwrite(flFileContent,flSize, 1, fufd);

  fclose(flfd);
  fclose(fufd);

  clock_t fendTime = clock();
  clock_t ftotalTime = fendTime - fstartTime;
  cout << "fopen, fread, fwrite, and fclose Total Time: " << ftotalTime << endl;




  return 0;
}
