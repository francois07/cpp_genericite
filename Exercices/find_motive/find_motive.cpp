#include "find_motive.hh"
#include <iostream>
#include <fstream>

int find_motive(std::string inFilePath, std::string motive)
{
  std::ifstream inFile;

  inFile.open(inFilePath);
  if(!inFile)
  {
    std::cerr << "The file " << inFilePath << " could not be opened.";
    exit(1);
  }

  inFile >> std::noskipws;

  int motiveCount = 0;
  bool motiveAppeared = false;
  int charToFind = 0;
  char c;

  while (inFile >> c)
  {
    if (c == motive[charToFind])
    {
      charToFind += 1;
    }
    else
    {
      charToFind = 0;
      if (c == ' ')
      {
        motiveAppeared = false;
      }
    }

    if (charToFind == (int)motive.length())
    {
      charToFind = 0;

      if (!motiveAppeared)
      {
        motiveCount += 1;
        motiveAppeared = true;
      }
    }
  }

  inFile.close();

  return motiveCount;
}