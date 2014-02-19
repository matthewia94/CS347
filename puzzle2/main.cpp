/*******************************************************************************
* @file main.cpp
* @author Matthew Anderson <mia2n4>
* @version 1.0
* @brief main file which will solve the puzzle
*******************************************************************************/

#include "puzzle.h"

using namespace std;

int main()
{
  ifstream in;
  string name;
  int height, width, wrigglers;
  Puzzle solved;
  
  cout << "Enter file name: " << endl;
  cin >> name;
  
  in.open(name.c_str());
  in >> width;
  in >> height;
  in >> wrigglers;
  
  Puzzle p(height, width, wrigglers);
  in >> p;
  p.solveIDDLTS();
  
  in.close();
  return 0;
}
