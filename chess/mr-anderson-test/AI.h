#ifndef AI_H
#define AI_H

#include "BaseAI.h"
#include <iostream>
#include <cstdlib>
#include "action.h"

using namespace std;

const int BOARD_WEIGHT[8][8] = {{1, 1, 1, 1, 1, 1, 1, 1}, 
                               {1, 2, 2, 2, 2, 2, 2, 1},
                               {1, 2, 3, 3, 3, 3, 2, 1},
                               {1, 2, 3, 4, 4, 3, 2, 1},
                               {1, 2, 3, 4, 4, 3, 2, 1},
                               {1, 2, 3, 3, 3, 3, 2, 1},
                               {1, 2, 2, 2, 2, 2, 2, 1},
                               {1, 1, 1, 1, 1, 1, 1, 1}};
                               

///The class implementing gameplay logic.
class AI: public BaseAI
{
  public:
    AI(Connection* c);
    virtual const char* username();
    virtual const char* password();
    virtual void init();
    virtual bool run();
    virtual void end();
    
    //custom functions
    void findPieces(vector<int>& myP, int board[][8], int id);
    void findActions(const int p, vector<Action>& a, int board[][8], int id, int level, bool enPassant[]);
    void findActions(const int p, vector<Action>& a, int board[][8], int id, int level);
    void pawnMoves(const int p, vector<Action>& a, int board[][8], int id, bool enPassant[]);
    void rookMoves(const int p, vector<Action>& a, int board[][8], int id);
    void bishMoves(const int p, vector<Action>& a, int board[][8], int id);
    void knightMoves(const int p, vector<Action>& a, int board[][8], int id);
    void queenMoves(const int p, vector<Action>& a, int board[][8], int id);
    void kingMoves(const int p, vector<Action>& a, int board[][8], int id, int level);
    bool isOccupied(int rank, int file, int board[][8]);
    bool isEnemy(int rank, int file, int board[][8], int id);
    bool inCheck(int board[][8], Action& act, int id);
    void findKing(int& r, int& f, int board[][8], int id);
    bool canCastleLeft(const int p, int board[][8], int id);
    bool canCastleRight(const int p, int board[][8], int id);
    void outputMove(const Action a);
    char convertFile(int file);
    int computeHeur(int board[][8], int id);
    int pieceVal(char piece);
    int posVal(int rank, int file);
    Action iddlmm(int limit, int board[][8], int id, bool enPassant[]);
    Action dlmm(int limit, int board[][8], int id, bool enPassant[]);
    int max(int limit, int board[][8], int id, Action a); 
    int min(int limit, int board[][8], int id, Action a);
    Action findMaxMove(vector<Action>& a);
    Action findMinMove(vector<Action>& a);
    void getRankFile(int index, int board[][8], int& rank, int& file);
    
};

#endif
