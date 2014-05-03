#include "AI.h"
#include "util.h"

AI::AI(Connection* conn) : BaseAI(conn) {}

const char* AI::username()
{
  return "Mr. Anderson";
}

const char* AI::password()
{
  return "password";
}

//This function is run once, before your first turn.
void AI::init()
{
  srand(time(NULL));
}

//This function is called each time it is your turn.
//Return true to end your turn, return false to ask the server for updated information.
bool AI::run()
{
  vector<int> myPieces;
  vector<Action> posMoves;
  Action move;
  int board[8][8];
  bool enPassant[8];
  char promo;
  ifstream in("moveFile");
  string temp;
  
  // Print out the current board state
  cout<<"+---+---+---+---+---+---+---+---+"<<endl;
  for(int rank=8; rank>0; rank--)
  {
    cout<<"|";
    for(int file=1; file<=8; file++)
    {
      bool found = false;
      // Loops through all of the pieces
      for(int p=0; !found && p<pieces.size(); p++)
      {
        // determines if that piece is at the current rank and file
        if(pieces[p].rank() == rank && pieces[p].file() == file)
        {
          //add to the board
          board[rank - 1][file - 1] = p;
          
          found = true;
          // Checks if the piece is black
          if(pieces[p].owner() == 1)
          {
            cout<<"*";
          }
          else
          {
            cout<<" ";
          }
          // prints the piece's type
          cout<<(char)pieces[p].type()<<" ";
        }
      }
      if(!found)
      {
        //-1 on board is empty space
        board[rank - 1][file - 1] = -1;
        
        cout<<"   ";
      }
      cout<<"|";
    }
    cout<<endl<<"+---+---+---+---+---+---+---+---+"<<endl;
  }

  // Looks through information about the players
  for(size_t p=0; p<players.size(); p++)
  {
    cout<<players[p].playerName();
    // if playerID is 0, you're white, if its 1, you're black
    if(players[p].id() == playerID())
    {
      cout<<" (ME)";
    }
    cout<<" time remaining: "<<players[p].time()<<endl;
  }

  // if there has been a move, print the most recent move
  if(moves.size() > 0)
  {
    for(int i = 0; i < 8; i++)
    {
      enPassant[i] = false;
    }
    
    if(abs(moves.at(0).fromRank() - moves.at(0).toRank()) == 2 &&
       pieces.at(board[moves.at(0).toRank()-1][moves.at(0).toFile()-1]).type() == 'P')
    {
      enPassant[moves.at(0).toFile()] = true;
    }
  }
  
  if(moves.size() < 54)
  {
    for(int i = 0; i < moves.size(); i++)
    {
      getLine(in, temp);
    }
    in >> pieceNum;
    in >> pieceFile;
    in >> pieceRank;
  }
  else
  {
    move = iddlmm(4, board, playerID(), enPassant);
  }
  
  pieces[move.index].move(move.newFile, move.newRank, int(move.promotion));
  
  in.close();
  return true;
}

//This function is run once, after your last turn.
void AI::end(){}

void AI::findPieces(vector<int>& myP, int board[][8], int id)
{
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      if(board[i][j] != -1 && pieces.at(board[i][j]).owner() == id)
        myP.push_back(board[i][j]);
    }
  }
}

void AI::findActions(const int p, vector<Action>& a, int board[][8], int id, int level)
{
  bool enPassant[8];
  for(int i = 0; i < 8; i++)
  {
    enPassant[i] = false;
  }
  
  switch(pieces.at(p).type())
  {
    case 'P':
      pawnMoves(p, a, board, id, enPassant);
      break;
    case 'R':
      rookMoves(p, a, board, id);
      break;
    case 'B':
      bishMoves(p, a, board, id);
      break;
    case 'N':
      knightMoves(p, a, board, id);
      break;
    case 'Q':
      queenMoves(p, a, board, id);
      break;
    case 'K':
      kingMoves(p, a, board, id, level);
      break;
  }
}

void AI::findActions(const int p, vector<Action>& a, int board[][8], int id, int level, bool enPassant[])
{
  switch(pieces.at(p).type())
  {
    case 'P':
      pawnMoves(p, a, board, id, enPassant);
      break;
    case 'R':
      rookMoves(p, a, board, id);
      break;
    case 'B':
      bishMoves(p, a, board, id);
      break;
    case 'N':
      knightMoves(p, a, board, id);
      break;
    case 'Q':
      queenMoves(p, a, board, id);
      break;
    case 'K':
      kingMoves(p, a, board, id, level);
      break;
  }
}

bool AI::isOccupied(int rank, int file, int board[][8])
{
  if(rank > 7 || file > 7 || rank < 0 || file < 0)
    return true;
  return board[rank][file] != - 1;
}

bool AI::isEnemy(int rank, int file, int board[][8], int id)
{
  bool enemy = false;
  if(board[rank][file] != - 1 && rank < 8 && file < 8 && rank >= 0 && file >= 0)
    enemy = (pieces.at(board[rank][file]).owner() != id);
  return enemy;
}

void AI::pawnMoves(const int p, vector<Action>& a, int board[][8], int id, bool enPassant[])
{
  Action temp;
  const char promotions[4] = {'Q', 'R', 'B', 'N'};
  temp.index = p;
  temp.takes = false;
  int side = 1, rank, file;
  //set to default value to keep consistent
  temp.promotion = 'Q';
  
  if(pieces.at(p).owner() == 1)
    side = -1;
  
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      if(board[i][j] == p)
      {
        rank = i;
        file = j;
      }
    }
  }
  
  //move forward 2 if it hasn't moved
  if(!pieces.at(p).hasMoved() && !isOccupied(rank+(2*side), file, board) &&  
     !isOccupied(rank+(1*side), file, board))
  {
    temp.newFile = pieces.at(p).file();
    temp.newRank = pieces.at(p).rank() + (2*side);
    a.push_back(temp);
  }
  
  //move forward 1
  if(!isOccupied(rank + (1*side), file, board))
  {
    /*
    if(pieces.at(p).rank() + (1*side) == 8)
    {
      for(int i = 0; i < 4; i++)
      {
        temp.newFile = pieces.at(p).file();
        temp.newRank = pieces.at(p).rank() + (1*side);
        temp.promotion = promotions[i];
        a.push_back(temp);
      }
    }
    
    else
    {
    */
      temp.newFile = pieces.at(p).file();
      temp.newRank = pieces.at(p).rank() + (1*side);
      a.push_back(temp);
    //}
  }
  
  temp.takes = true;
  //take right
  if(isEnemy(rank + (1*side), file + 1, board, id))
  {
    /*
    //if promotion should happen this will push all posibilites
    if(pieces.at(p).rank() + (1*side) == 8 || pieces.at(p).rank() + (1*side) == 0)
    {
      temp.newFile = pieces.at(p).file() + 1;
      temp.newRank = pieces.at(p).rank() + (1*side);
      for(int i = 0; i < 4; i++)
      {
        temp.promotion = promotions[i];
        a.push_back(temp);
      }
    }
    else
    {*/
      temp.newFile = pieces.at(p).file() + 1;
      temp.newRank = pieces.at(p).rank() + (1*side);
      a.push_back(temp);
    //}
  }
  
  //take left
  if(isEnemy(rank + (1*side), file - 1, board, id))
  {
    /*
    //if promotion should happen this will push all posibilites
    if(pieces.at(p).rank() + (1*side) == 8 || pieces.at(p).rank() + (1*side) == 0)
    {
      temp.newFile = pieces.at(p).file() - 1;
      temp.newRank = pieces.at(p).rank() + (1*side);
      for(int i = 0; i < 4; i++)
      {
        temp.promotion = promotions[i];
        a.push_back(temp);
      }
    }
    else
    {*/
      temp.newFile = pieces.at(p).file() - 1;
      temp.newRank = pieces.at(p).rank() + (1*side);
      a.push_back(temp);
    //}
  }

  if(id == 0)
  {
    //en passant left
    if(rank == 4 && file -1 >= 0 && board[rank][file-1] != -1 &&
       pieces.at(board[rank][file - 1]).type() == 'P' && 
       pieces.at(board[rank][file - 1]).owner() != id && enPassant[file-1])
    {
      temp.newFile = pieces.at(p).file() - 1;
      temp.newRank = pieces.at(p).rank() + 1;
      a.push_back(temp);
    }
    
    //en passant right
    if(rank == 4 && file + 1 < 8 && board[rank][file+1] != -1 && 
       pieces.at(board[rank][file + 1]).type() == 'P' && 
       pieces.at(board[rank][file + 1]).owner() != id && enPassant[file+1])
    {
      temp.newFile = pieces.at(p).file() + 1;
      temp.newRank = pieces.at(p).rank() + 1;
      a.push_back(temp);
    }
  }
  else
  {
    //en passant left
    if(rank == 3 && file -1 >= 0 && board[rank][file-1] != -1 &&
       pieces.at(board[rank][file - 1]).type() == 'P' && 
       pieces.at(board[rank][file - 1]).owner() != id && enPassant[file-1])
    {
      temp.newFile = pieces.at(p).file() - 1;
      temp.newRank = pieces.at(p).rank() + 1;
      a.push_back(temp);
    }
    
    //en passant right
    if(rank == 3 && file + 1 < 8 && board[rank][file+1] != -1 && 
       pieces.at(board[rank][file + 1]).type() == 'P' && 
       pieces.at(board[rank][file + 1]).owner() != id && enPassant[file+1])
    {
      temp.newFile = pieces.at(p).file() + 1;
      temp.newRank = pieces.at(p).rank() + 1;
      a.push_back(temp);
    }
  }
}

void AI::rookMoves(const int p, vector<Action>& a, int board[][8], int id) 
{
  Action temp;
  int offset = 1;
  int rank, file;
  bool hitPiece = false;
  
  //promotion won't ever happen but required for the move function
  temp.index = p;
  temp.promotion = 'Q';
  
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      if(board[i][j] == p)
      {
        rank = i;
        file = j;
      }
    }
  }
  
  //move up
  while(!hitPiece && rank + offset < 8 && (board[rank + offset][file] == - 1 ||
        isEnemy(rank + offset, file, board, id)))
  {
    if(isOccupied(rank + offset, file, board))
      hitPiece = true;
      
    temp.takes = isEnemy(rank + offset, file, board, id);
    temp.newRank = pieces.at(p).rank() + offset;
    temp.newFile = pieces.at(p).file();
    a.push_back(temp);
    offset++;
  }

  //move down
  offset = 1;
  hitPiece = false;
  while(!hitPiece && rank - offset >= 0 && (board[rank - offset][file] == - 1 || 
        isEnemy(rank - offset, file, board, id)))
  {
    if(isOccupied(rank - offset, file, board))
      hitPiece = true;
      
    temp.takes = isEnemy(rank - offset, file, board, id);
    temp.newRank = pieces.at(p).rank() - offset;
    temp.newFile = pieces.at(p).file();
    a.push_back(temp);
    offset++;
  }  
  
  //move right
  offset = 1;
  hitPiece = false;
  while(!hitPiece && file + offset < 8 && (board[rank][file + offset] == - 1 || 
        isEnemy(rank, file + offset, board, id)))
  {
    if(isOccupied(rank, file + offset, board))
      hitPiece = true;
    
    temp.takes = isEnemy(rank, file + offset, board, id);
    temp.newRank = pieces.at(p).rank();
    temp.newFile = pieces.at(p).file() + offset;
    a.push_back(temp);
    offset++;
  }  
  
  //move left
  offset = 1;
  hitPiece = false;
  while(!hitPiece && file - offset >= 0 && (board[rank][file - offset] == - 1 || 
        isEnemy(rank, file - offset, board, id)))
  {
    if(isOccupied(rank, file - offset, board))
      hitPiece = true;

    temp.takes = isEnemy(rank, file - offset, board, id);
    temp.newRank = pieces.at(p).rank();
    temp.newFile = pieces.at(p).file() - offset;
    a.push_back(temp);
    offset++;
  }  
}

void AI::bishMoves(const int p, vector<Action>& a, int board[][8], int id) 
{
  Action temp;
  int offset = 1, rank, file;
  bool hitPiece = false;
  
  //promotion won't ever happen but required for the move function
  temp.index = p;
  temp.promotion = 'Q';
  
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      if(board[i][j] == p)
      {
        rank = i;
        file = j;
      }
    }
  }
  
  //move diag up and right
  while(!hitPiece && (!isOccupied(rank + offset, file + offset, board) || 
        isEnemy(rank + offset, file + offset, board, id)))
  {
    hitPiece = isOccupied(rank + offset, file + offset, board);
    
    temp.takes = isEnemy(rank + offset, file + offset, board, id);
    temp.newRank = pieces.at(p).rank() + offset;
    temp.newFile = pieces.at(p).file() + offset;
    a.push_back(temp);
    offset++;
  }
  
  //move diag down and left
  offset = 1;
  hitPiece = false;
  while(!hitPiece && (!isOccupied(rank - offset, file - offset, board) || 
        isEnemy(rank - offset, file - offset, board, id)))
  {
    hitPiece = isOccupied(rank - offset, file - offset, board);
    
    temp.takes = isEnemy(rank - offset, file - offset, board, id);
    temp.newRank = pieces.at(p).rank() - offset;
    temp.newFile = pieces.at(p).file() - offset;
    a.push_back(temp);
    offset++;
  }  
  
  //move diag up and left
  offset = 1;
  hitPiece = false;
  while(!hitPiece && (!isOccupied(rank + offset, file - offset, board) || 
        isEnemy(rank + offset, file - offset, board, id)))
  {
    hitPiece = isOccupied(rank + offset, file - offset, board);
    
    temp.takes = isEnemy(rank + offset, file - offset, board, id);
    temp.newRank = pieces.at(p).rank() + offset;
    temp.newFile = pieces.at(p).file() - offset;
    a.push_back(temp);
    offset++;
  }  
  
  //move diag down and right
  offset = 1;
  hitPiece = false;
  while(!hitPiece && (!isOccupied(rank - offset, file + offset, board) ||
        isEnemy(rank - offset, file + offset, board, id)))
  {
    hitPiece = isOccupied(rank - offset, file + offset, board);
    
    temp.takes = isEnemy(rank - offset, file + offset, board, id);
    temp.newRank = pieces.at(p).rank() - offset;
    temp.newFile = pieces.at(p).file() + offset;
    a.push_back(temp);
    offset++;
  }  
}

void AI::knightMoves(const int p, vector<Action>& a, int board[][8], int id)
{
  Action temp;
  int rank, file;
  
  //promotion won't occur but required for the move function
  temp.index = p;
  temp.promotion = 'Q';
  
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      if(board[i][j] == p)
      {
        rank = i;
        file = j;
      }
    }
  }
  
  //up 2 right 1
  if(!isOccupied(rank+2, file+1, board) || isEnemy(rank+2, file+1, board, id))
  {
    temp.takes = isEnemy(rank + 2, file + 1, board, id);
    temp.newRank = pieces.at(p).rank() + 2;
    temp.newFile = pieces.at(p).file() + 1;
    a.push_back(temp);
  }
  
  //up 1 right 2
  if(!isOccupied(rank+1, file+2, board) || isEnemy(rank+1, file+2, board, id))
  {
    temp.takes = isEnemy(rank + 1, file + 2, board, id);
    temp.newRank = pieces.at(p).rank() + 1;
    temp.newFile = pieces.at(p).file() + 2;
    a.push_back(temp);
  }
  
  //down 1 right 2
  if(!isOccupied(rank-1, file+2, board) || isEnemy(rank-1, file+2, board, id))
  {
    temp.takes = isEnemy(rank - 1, file + 2, board, id);
    temp.newRank = pieces.at(p).rank() - 1;
    temp.newFile = pieces.at(p).file() + 2;
    a.push_back(temp);
  }
  
  //down 2 right 1
  if(!isOccupied(rank-2, file+1, board) || isEnemy(rank-2, file+1, board, id))
  {
    temp.takes = isEnemy(rank - 2, file + 1, board, id);
    temp.newRank = pieces.at(p).rank() - 2;
    temp.newFile = pieces.at(p).file() + 1;
    a.push_back(temp);
  }
  
  //down 2 left 1
  if(!isOccupied(rank-2, file-1, board) || isEnemy(rank-2, file-1, board, id))
  {
    temp.takes = isEnemy(rank - 2, file - 1, board, id);
    temp.newRank = pieces.at(p).rank() - 2;
    temp.newFile = pieces.at(p).file() - 1;
    a.push_back(temp);
  }
  
  //down 1 left 2
  if(!isOccupied(rank-1, file-2, board) || isEnemy(rank-1, file-2, board, id))
  {
    temp.takes = isEnemy(rank - 1, file - 2, board, id);
    temp.newRank = pieces.at(p).rank() - 1;
    temp.newFile = pieces.at(p).file() - 2;
    a.push_back(temp);
  }
  
  //up 1 left 2
  if(!isOccupied(rank+1, file-2, board) || isEnemy(rank+1, file-2, board, id))
  {
    temp.takes = isEnemy(rank + 1, file - 2, board, id);
    temp.newRank = pieces.at(p).rank() + 1;
    temp.newFile = pieces.at(p).file() - 2;
    a.push_back(temp);
  }
  
  //up 2 left 1
  if(!isOccupied(rank+2, file-1, board) || isEnemy(rank+2, file-1, board, id))
  {
    temp.takes = isEnemy(rank + 2, file - 1, board, id);
    temp.newRank = pieces.at(p).rank() + 2;
    temp.newFile = pieces.at(p).file() - 1;
    a.push_back(temp);
  }
}

void AI::queenMoves(const int p, vector<Action>& a, int board[][8], int id) 
{
  Action temp;
  int offset = 1, rank, file;
  
  //promotion won't ever happen but required for the move function
  temp.index = p;
  temp.promotion = 'Q';
  
  rank = pieces.at(p).rank() - 1;
  file = pieces.at(p).file() - 1;
  
  rookMoves(p, a, board, id);
  bishMoves(p, a, board, id);
}

void AI::kingMoves(const int p, vector<Action>& a, int board[][8], int id, int level)
{
  Action temp;
  int rank, file;
  bool castle = true;
  
  //promotion won't ever happen but required for the move function
  temp.index = p;
  temp.promotion = 'Q';
  
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      if(board[i][j] == p)
      {
        rank = i;
        file = j;
      }
    }
  }
 
  for(int i = -1; i < 2; i++)
  {
    for(int j = -1; j < 2; j++)
    {
      if(!(i == 0 && j == 0))
      {
        if(!isOccupied(rank + i, file + j, board) || 
          isEnemy(rank + i, file + j, board, id))
        {
          temp.takes = isEnemy(rank + i, file + j, board, id);
          temp.newRank = pieces.at(p).rank() + i;
          temp.newFile = pieces.at(p).file() + j;
          a.push_back(temp);
        }
      }
    }
  }
  
  temp.newRank = rank + 1;
  temp.newFile = file + 1;
  temp.takes = false;
  
  if(level == 0 && !inCheck(board, temp, id))
  {
    if(canCastleLeft(p, board, id))
    {
      temp.newRank = pieces.at(p).rank();
      temp.newFile = pieces.at(p).file() - 2;
      a.push_back(temp);
    }
    
    if(canCastleRight(p, board, id))
    {
      temp.newRank = pieces.at(p).rank();
      temp.newFile = pieces.at(p).file() + 2;
    }
  }
}

bool AI::inCheck(int board[][8], Action& act, int id)
{
  bool check = false;
  int temp, kingRank = 0, kingFile = 0, rank, file, newR, newF;
  vector<int> enemies;
  vector<Action> a;
  
  rank = pieces.at(act.index).rank() - 1;
  file = pieces.at(act.index).file() - 1;
  newR = act.newRank - 1;
  newF = act.newFile - 1;
  
  //move piece on board
  temp = board[newR][newF];
  board[newR][newF] = board[rank][file];
vector out of range/ range check

  board[rank][file] = - 1;
  
  findKing(kingRank, kingFile, board, id);
  
  //check for check
  findPieces(enemies, board, !id);

  for(int i = 0; i < enemies.size(); i++)
    findActions(enemies.at(i), a, board, !id, 1);
  
  for(int i = 0; i < a.size(); i++)
  {
    if(a.at(i).newRank == kingRank && a.at(i).newFile == kingFile)
    {
      check = true;
    }
  }
  
  //undo move of pieces
  board[rank][file] = board[newR][newF];
  board[newR][newF] = temp;
  
  return check;
}

void AI::findKing(int& r, int& f, int board[][8], int id)
{
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      if(board[i][j] != -1 && pieces.at(board[i][j]).owner() == id && 
         pieces.at(board[i][j]).type() == 'K')
      {
        r = i + 1;
        f = j + 1;
      }
    }
  }
}

bool AI::canCastleLeft(const int p, int board[][8], int id)
{
  int rank = pieces.at(p).rank() -1, file = pieces.at(p).file() -1;
  Action temp;
  bool castle;
  
  temp.promotion = 'Q';
  temp.index = p;
  
  castle = true;
  if(!pieces.at(p).hasMoved())
  {
    if(board[rank][file-4] != -1 && pieces.at(board[rank][file-4]).type() == 'R' 
       && pieces.at(board[rank][file-4]).owner() == id && 
       !pieces.at(board[rank][file-4]).hasMoved())
    {
      for(int i = 1; i <= 3; i++)
      { 
        if(isOccupied(rank, file - i, board))
        {
          castle = false;
        }
        else if(i < 3)
        {
          temp.newRank = pieces.at(p).rank();
          temp.newFile = pieces.at(p).file() - i;
          if(inCheck(board, temp, id))
            castle = false;
        }
      }
    }
    else
      castle = false;
  }
  else
  {
    castle = false;
  }
  return castle;
}

bool AI::canCastleRight(const int p, int board[][8], int id)
{
  int rank = pieces.at(p).rank() -1, file = pieces.at(p).file() -1;
  Action temp;
  bool castle;
  
  temp.promotion = 'Q';
  temp.index = p;
  
  castle = true;
  if(!pieces.at(p).hasMoved())
  {
    if(board[rank][file+3] != -1 && pieces.at(board[rank][file+3]).type() == 'R' 
       && pieces.at(board[rank][file+3]).owner() == id && 
       !pieces.at(board[rank][file+3]).hasMoved())
    {
      for(int i = 1; i <= 2; i++)
      { 
        if(isOccupied(rank, file + i, board))
        {
          castle = false;
        }
        else
        {        
          temp.newRank = pieces.at(p).rank();
          temp.newFile = pieces.at(p).file() + i;
          if(inCheck(board, temp, id))
            castle = false;
        }
      }
    }
    else
      castle = false;
  }
  else
  {
    castle = false;
  }
  return castle;
}

void AI::outputMove(const Action a)
{
  if(pieces.at(a.index).type() != 'P')
    cout << char(pieces.at(a.index).type());
  
  cout << convertFile(pieces.at(a.index).file()) << pieces.at(a.index).rank();
  if(a.takes)
    cout << "x";
  else
    cout << "-";
  cout << convertFile(a.newFile) << a.newRank;
  
  if(pieces.at(a.index).type() == 'P' && a.newRank == 8)
  {
    cout << a.promotion;
  }
  
  cout << endl;
}

char AI::convertFile(int file)
{
  return file + 96;
}

int AI::computeHeur(int board[][8], int id)
{
  int moveVal = 0;
  
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      //check if board space is occupied
      if(isOccupied(i,j, board) && pieces.at(board[i][j]).owner() == id)
      {
        //value based on piece type
        moveVal += pieceVal(pieces.at(board[i][j]).type()) * 10;
        
        //value base on piece position
        moveVal += posVal(i, j);
      }
      else if(isOccupied(i, j, board))
      {
        //value based on piece type
        moveVal -= pieceVal(pieces.at(board[i][j]).type()) * 10;
        
        //value based on piece position
        moveVal -= posVal(i, j);
      }
    }
  }
  
  return moveVal;
}

int AI::pieceVal(char piece)
{
  int val;
  
  switch (piece)
  {
    case 'K':
      val = 1000;
      break;
    case 'Q':
      val = 9;
      break;
    case 'R':
      val = 5;
      break;
    case 'B':
      val = 3;
      break;
    case 'N':
      val = 3;
      break;
    case 'P':
      val = 1;
      break;
  }
  
  return val;
}

int AI::posVal(int rank, int file)
{
  return BOARD_WEIGHT[rank][file];
}

Action AI::iddlmm(int limit, int board[][8], int id, bool enPassant[])
{
  Action temp;
  
  for(int i = 0; i < limit; i++)
  {
    temp = dlmm(limit, board, id, enPassant);
  }
  
  return temp;
}

Action AI::dlmm(int limit, int board[][8], int id, bool enPassant[])
{
  Action temp;
  vector<Action> frontier;
  vector<int> myPieces;
  
  //find my pieces
  findPieces(myPieces, board, id);
  
  //generate actions
  for(int i = 0; i < myPieces.size(); i++)
  {
    findActions(myPieces.at(i), frontier, board, id, 1, enPassant);
  }
  
  //find value for each move in frontier
  for(int i = 0; i < frontier.size(); i++)
  {
    temp = frontier.at(i);
    frontier.at(i).heur = min(limit - 1, board, !id, temp);
  }
  
  //return the best move
  return findMaxMove(frontier);
}

int AI::max(int limit, int board[][8], int id, Action a)
{
  int value;
  int rank, file, newR, newF, previous;
  Action temp;
  vector<Action> frontier;
  vector<int> myPieces;
  bool enPassant[8];
  
  for(int i = 0; i < 8; i++)
    enPassant[i] = false;
    
  //make move
  getRankFile(a.index, board, rank, file);
  newR = a.newRank - 1;
  newF = a.newFile - 1;
    
  if(pieces.at(a.index).type() == 'P' && newR == rank + 2)
    enPassant[file] = true;
  
  //make move
  previous = board[newR][newF];
  board[newR][newF] = board[rank][file];
  board[rank][file] = -1;
  
  findPieces(myPieces, board, id);
  
  if(limit > 0 && myPieces.size() > 0)
  {
    
    //find all pieces for max player
    for(int i = 0; i < myPieces.size(); i++)
    {
      findActions(myPieces.at(i), frontier, board, id, 1, enPassant);
    }
    
    //find value for each move in frontier
    for(int i = 0; i < frontier.size(); i++)
    {
      temp = frontier.at(i);
      frontier.at(i).heur = min(limit - 1, board, !id, temp);
    }
    
    //undo move on board
    board[rank][file] = board[newR][newF];
    board[newR][newF] = previous;
    
    return findMaxMove(frontier).heur;
  }
  
  value = computeHeur(board, playerID());
  
  //undo move on board
  board[rank][file] = board[newR][newF];
  board[newR][newF] = previous;
  
  return value;
}

int AI::min(int limit, int board[][8], int id, Action a)
{ 
  int value;
  int rank, file, newR, newF, previous;
  Action temp;
  vector<Action> frontier;
  vector<int> myPieces;
  bool enPassant[8];
  
  for(int i = 0; i < 8; i++)
    enPassant[i] = false;
    
  //make move
  getRankFile(a.index, board, rank, file);
  newR = a.newRank - 1;
  newF = a.newFile - 1;
    
  if(pieces.at(a.index).type() == 'P' && newR == rank + 2)
    enPassant[file] = true;
  
  //make move
  previous = board[newR][newF];
  board[newR][newF] = board[rank][file];
  board[rank][file] = -1;
  
  findPieces(myPieces, board, id);
  
  if(limit > 0 && myPieces.size() > 0)
  { 
    
    //find all actions for min player
    for(int i = 0; i < myPieces.size(); i++)
    {
      findActions(myPieces.at(i), frontier, board, id, 1, enPassant);
    }
        
    //find value for each move in frontier
    for(int i = 0; i < frontier.size(); i++)
    {
      temp = frontier.at(i);
      frontier.at(i).heur = max(limit - 1, board, !id, temp);
    }

    //undo move on board
    board[rank][file] = board[newR][newF];
    board[newR][newF] = previous;
    
    return findMinMove(frontier).heur;
  }
  
  value = computeHeur(board, playerID());
  
  //undo move on board
  board[rank][file] = board[newR][newF];
  board[newR][newF] = previous;
  
  return value;
}

Action AI::findMaxMove(vector<Action>& a)
{
  int max = 0;

  for(int i = 1; i < a.size(); i++)
  {
    if(a.at(i).heur >= a.at(max).heur)
    {
      max = i;
    }
  }
  
  return a.at(max);
}

Action AI::findMinMove(vector<Action>& a)
{ 
  int min = 0;
  
  for(int i = 1; i < a.size(); i++)
  {
    if(a.at(i).heur < a.at(min).heur)
    {
      min = i;
    }
  }
  
  return a.at(min);
}

void AI::getRankFile(int index, int board[][8], int& rank, int& file)
{
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      if(board[i][j] == index)
      {
        rank = i;
        file = j;
        return;
      }
    }
  }
}












