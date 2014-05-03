//Action class which will define a move

#ifndef ACTION_H
#define ACTION_H

using namespace std;

class Action
{
  public:
    int index;
    int newRank;
    int newFile;
    char promotion;
    bool takes;
    int heur;
};

#endif 

