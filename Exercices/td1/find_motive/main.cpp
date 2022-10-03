#include <iostream>
#include <fstream>
#include "find_motive.hh"

int main(int argc, char *argv[])
{
  if(argc <= 2)
  {
    std::cerr << "Please input at least 2 arguments";
    exit(1);
  }

  std::string inFilePath = argv[1];
  std::string motive = argv[2];

  int motiveCount = find_motive(inFilePath, motive);

  std::cout << "The file " << inFilePath << " contains " << motiveCount << " words containing the motive " << motive;

  return 0;
}