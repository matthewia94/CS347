/*******************************************************************************
* @file puzzle.cpp
* @author Matthew Anderson <mia2n4>
* @version 1.0
* @brief function implementation file for the wriggler puzzle class
*******************************************************************************/

#include "puzzle.h"

using namespace std;

bool Puzzle::isSolved()
{
  bool goal = false;
  if(m_board[m_height - 1][m_width - 1] == 'U' || 
     m_board[m_height - 1][m_width - 1] == 'L')
  {
    if(findNum(m_width - 1, m_height - 1) == 0)
      goal = true;
  }
  return goal;
}

int Puzzle::findNum(int headX, int headY)
{
  int x = headX, y = headY;
  //find x and y after the head
  nextPiece(x, y);
  
  //trace the body of the wriggler
  while(m_board[y][x] == 'v' || m_board[y][x] == '<' || m_board[y][x] == '>' || 
        m_board[y][x] == '^')
  {
    nextPiece(x, y);
  }

  return m_board[y][x] - 48;
}

void Puzzle::findTail(int headX, int headY, int& x, int& y)
{
  int tempX = headX, tempY = headY;
  //find x and y after the head
  nextPiece(tempX, tempY);
  x = tempX;
  y = tempY;
  
  //trace the body of the wriggler
  while(m_board[tempY][tempX] == 'v' || m_board[tempY][tempX] == '<' || 
        m_board[tempY][tempX] == '>' || m_board[tempY][tempX] == '^')
  {
    x = tempX;
    y = tempY;
    nextPiece(tempX, tempY);
  }
}

Puzzle::Puzzle()
{
  m_board = NULL;
  m_wrigglers = NULL;
  m_height = 0;
  m_width = 0;
  m_numWrigs = 0;
  m_parent = -1;
  m_move.m_wrigglerID = 0;
  m_move.m_side = 0;
  m_move.m_destinationX = 0;
  m_move.m_destinationY = 0;
}

Puzzle::Puzzle(int h, int w, int wrig)
{
  m_height = h;
  m_width = w;
  m_numWrigs = wrig;
  m_parent = -1;

  m_move.m_wrigglerID = 0;
  m_move.m_side = 0;
  m_move.m_destinationX = 0;
  m_move.m_destinationY = 0;
  
  m_wrigglers = new wriggler[wrig];
  m_board = new char*[h];
  for(int i = 0; i < h; i++)
    m_board[i] = new char[w];    
}

Puzzle::~Puzzle()
{
  for(int i = 0; i < m_height; i++)
    delete[] m_board[i];
  delete[] m_board;
  
  delete[] m_wrigglers;
}

Puzzle& Puzzle::operator=(const Puzzle& rhs)
{
  for(int i = 0; i < m_height; i++)
    delete[] m_board[i];
  delete[] m_board;
  delete[] m_wrigglers;
  
  m_height = rhs.m_height;
  m_width = rhs.m_width;
  m_numWrigs = rhs.m_numWrigs;
  m_parent = rhs.m_parent;
  
  m_move.m_wrigglerID = rhs.m_move.m_wrigglerID;
  m_move.m_side = rhs.m_move.m_side;
  m_move.m_destinationX = rhs.m_move.m_destinationX;
  m_move.m_destinationY = rhs.m_move.m_destinationY;
  
  m_wrigglers = new wriggler[m_numWrigs];
  m_board = new char*[m_height];
  for(int i = 0; i < m_height; i++)
    m_board[i] = new char[m_width];
    
  for(int i = 0; i < m_numWrigs; i++)
  {
    m_wrigglers[i].m_headX = rhs.m_wrigglers[i].m_headX;
    m_wrigglers[i].m_headY = rhs.m_wrigglers[i].m_headY;
  }
    
  for(int i = 0; i < m_height; i++)
  {
    for(int j = 0; j < m_width; j++)
      m_board[i][j] = rhs.m_board[i][j];
  }
  return *this;
}

Puzzle::Puzzle(const Puzzle& rhs)
{
  m_height = rhs.m_height;
  m_width = rhs.m_width;
  m_numWrigs = rhs.m_numWrigs;
  m_parent = rhs.m_parent;
  
  m_move.m_wrigglerID = rhs.m_move.m_wrigglerID;
  m_move.m_side = rhs.m_move.m_side;
  m_move.m_destinationX = rhs.m_move.m_destinationX;
  m_move.m_destinationY = rhs.m_move.m_destinationY;
  
  m_wrigglers = new wriggler[m_numWrigs];
  m_board = new char*[m_height];
  for(int i = 0; i < m_height; i++)
    m_board[i] = new char[m_width];
    
  for(int i = 0; i < m_numWrigs; i++)
  {
    m_wrigglers[i].m_headX = rhs.m_wrigglers[i].m_headX;
    m_wrigglers[i].m_headY = rhs.m_wrigglers[i].m_headY;  
  }
    
  for(int i = 0; i < m_height; i++)
  {
    for(int j = 0; j < m_width; j++)
      m_board[i][j] = rhs.m_board[i][j];
  }
}


ostream& operator<<(ostream& os, Puzzle& p)
{
  for(int i = 0; i < p.m_height; i++)
  {
    for(int j = 0; j < p.m_width; j++)
      os << p.m_board[i][j] << " ";
    os << endl;  
  }
  return os;
}

istream& operator>>(istream& is, Puzzle& p)
{
  for(int i = 0; i < p.m_height; i++)
  {
    for(int j = 0; j < p.m_width; j++)
    {
      is >> p.m_board[i][j];
    }
  }
  for(int i = 0; i < p.m_height; i++)
  {
    for(int j = 0; j < p.m_width; j++)
    {
      if(p.m_board[i][j] == 'U' || p.m_board[i][j] == 'D' || 
         p.m_board[i][j] == 'R' || p.m_board[i][j] == 'L')
      {
        p.m_wrigglers[p.findNum(j, i)].m_headX = j;
        p.m_wrigglers[p.findNum(j, i)].m_headY = i;
      }
    }
  }
}

bool Puzzle::isValid(int x, int y)
{
  bool valid = false;
  //check that x and y are in the correct range
  if(y < m_height && x < m_width && x >= 0 && y >= 0)
  {
    //check that x and y are a valid move
    if(m_board[y][x] == 'e')
      valid = true;
  } 
  return valid;
}

void Puzzle::nextPiece(int& x, int& y)
{
  switch (m_board[y][x])
  {
    case '^':
    case 'U':
      y -= 1;
      break;
    case 'v':
    case 'D':
      y += 1;
      break;
    case '<':
    case 'L':
      x -= 1;
      break;
    case '>':
    case 'R':
      x += 1;
      break; 
  }
}

char Puzzle::convertHead(char dir)
{
  char newDir;
  
  switch (dir)
  {
    case 'U':
      newDir = '^';
      break;
    case 'D':
      newDir = 'v';
      break;
    case 'L':
      newDir = '<';
      break;
    case 'R':
      newDir = '>';
  }
  
  return newDir;
}

char Puzzle::convertToHead(char dir)
{
  char newDir;
  
  switch (dir)
  {
    case 'v':
      newDir = 'D';
      break;
    case '^':
      newDir = 'U';
      break;
    case '>':
      newDir = 'R';
      break;
    case '<':
      newDir = 'L';
  }
  
  return newDir;
}

bool Puzzle::moveTail(int dir, int x, int y)
{
  bool success = false;
  int tailX, tailY, tempX = x, tempY = y;
  findTail(x, y, tailX, tailY);
  nextPiece(tailX, tailY);
  
  switch (dir)
  {
    case 0:
      //up
      if(isValid(tailX, tailY - 1))
      {
        m_board[tailY - 1][tailX] = m_board[tailY][tailX];
        m_board[tailY][tailX] = '^';
        nextPiece(tempX, tempY);
        m_board[y][x] = 'e';
        m_board[tempY][tempX] = convertToHead(m_board[tempY][tempX]);
        setHead(tempX, tempY, findNum(tempX, tempY));
        success = true;
      }
      break;
    case 1:
      //right
      if(isValid(tailX + 1, tailY))
      {
        m_board[tailY][tailX + 1] = m_board[tailY][tailX];
        m_board[tailY][tailX] = '>';
        nextPiece(tempX, tempY);
        m_board[y][x] = 'e';
        m_board[tempY][tempX] = convertToHead(m_board[tempY][tempX]);
        setHead(tempX, tempY, findNum(tempX, tempY));
        success = true;
      }
      break;
    case 2:
      //down
      if(isValid(tailX, tailY + 1))
      {
        m_board[tailY + 1][tailX] = m_board[tailY][tailX];
        m_board[tailY][tailX] = 'v';
        nextPiece(tempX, tempY);
        m_board[y][x] = 'e';
        m_board[tempY][tempX] = convertToHead(m_board[tempY][tempX]);
        setHead(tempX, tempY, findNum(tempX, tempY));
        success = true; 
      }
      break;
    case 3:
      //left
      if(isValid(tailX - 1, tailY))
      {
        m_board[tailY][tailX - 1] = m_board[tailY][tailX];
        m_board[tailY][tailX] = '<';
        nextPiece(tempX, tempY);
        m_board[y][x] = 'e';
        m_board[tempY][tempX] = convertToHead(m_board[tempY][tempX]);
        setHead(tempX, tempY, findNum(tempX, tempY));
        success = true; 
      }
      break;
  }
  return success;
}

bool Puzzle::moveHead(int dir, int x, int y)
{
  bool success = false;
  int tailX, tailY, tempX, tempY;
  switch (dir)
  {
    case 0:
      //up
      if(isValid(x, y - 1))
      {
        m_board[y - 1][x] = 'D';
        m_board[y][x] = convertHead(m_board[y][x]);
        setHead(x, y - 1, findNum(x, y - 1));
        findTail(x, y, tempX, tempY);
        tailX = tempX;
        tailY = tempY;
        nextPiece(tailX, tailY);
        m_board[tempY][tempX] = m_board[tailY][tailX];
        m_board[tailY][tailX] = 'e';
        success = true;
      }
      break;
    case 1:
      //right
      if(isValid(x + 1, y))
      {
        m_board[y][x + 1] = 'L';
        m_board[y][x] = convertHead(m_board[y][x]);
        setHead(x + 1, y, findNum(x + 1, y));
        findTail(x, y, tempX, tempY);
        tailX = tempX;
        tailY = tempY;
        nextPiece(tailX, tailY);
        m_board[tempY][tempX] = m_board[tailY][tailX];
        m_board[tailY][tailX] = 'e';
        success = true;      
      }
      break;
    case 2:
      //down
      if(isValid(x, y + 1))
      {
        m_board[y + 1][x] = 'U';
        m_board[y][x] = convertHead(m_board[y][x]);
        setHead(x, y + 1, findNum(x, y + 1));
        findTail(x, y, tempX, tempY);
        tailX = tempX;
        tailY = tempY;
        nextPiece(tailX, tailY);
        m_board[tempY][tempX] = m_board[tailY][tailX];
        m_board[tailY][tailX] = 'e';
        success = true;   
      }
      break;
    case 3:
      //left
      if(isValid(x - 1, y))
      {
        m_board[y][x - 1] = 'R';
        m_board[y][x] = convertHead(m_board[y][x]);
        setHead(x - 1, y, findNum(x - 1, y));
        findTail(x, y, tempX, tempY);
        tailX = tempX;
        tailY = tempY;
        nextPiece(tailX, tailY);
        m_board[tempY][tempX] = m_board[tailY][tailX];
        m_board[tailY][tailX] = 'e';
        success = true;   
      }
      break;
  }
  return success;
}

Puzzle Puzzle::solveBFTS()
{
  vector<Puzzle> parents;
  queue<Puzzle> frontier;
  stack<Puzzle> path;
  Puzzle temp;
  frontier.push(*this);
  int steps, tailX, tailY;
  ofstream out;
  clock_t start;
  double duration;
  
  //create tree and store possible moves in a tree
  start = clock();
  while(!frontier.front().isSolved())
  {
    parents.push_back(frontier.front());
    for(int i = 0; i < m_numWrigs; i++)
    {
      //test each direction
      for(int j = 0; j < 4; j++)
      {
        temp = frontier.front();
        if(temp.moveHead(j, temp.m_wrigglers[i].m_headX, 
           temp.m_wrigglers[i].m_headY))
        {
          temp.m_move.m_wrigglerID = i;
          temp.m_move.m_side = 0;
          temp.m_move.m_destinationX = temp.m_wrigglers[i].m_headX;
          temp.m_move.m_destinationY = temp.m_wrigglers[i].m_headY;
          temp.m_parent = parents.size() - 1;
          frontier.push(temp);
        }
        temp = frontier.front();
        if(temp.moveTail(j, temp.m_wrigglers[i].m_headX, 
           temp.m_wrigglers[i].m_headY))
        {
          temp.m_move.m_wrigglerID = i;
          temp.m_move.m_side = 1;
          temp.findTail(temp.m_wrigglers[i].m_headX, 
                        temp.m_wrigglers[i].m_headY, tailX, tailY);
          temp.nextPiece(tailX, tailY);
          temp.m_move.m_destinationX = tailX;
          temp.m_move.m_destinationY = tailY;
          temp.m_parent = parents.size() - 1;
          frontier.push(temp);
        }
      }
    }
    frontier.pop();
  }
  duration = (clock() - start) / (double) CLOCKS_PER_SEC;
  
  temp = frontier.front();
  path.push(temp);
  while(temp.m_parent != -1)
  {
    temp = parents.at(temp.m_parent);
    path.push(temp);
  }
  
  //output solution file
  out.open("solution.txt");
  path.pop();
  steps = path.size();
  for(int i = 0; i < steps; i++)
  {
    out << path.top().m_move.m_wrigglerID << " " << path.top().m_move.m_side 
        << " " << path.top().m_move.m_destinationX << " " 
        << path.top().m_move.m_destinationY << endl;
    path.pop();
  }
  out << frontier.front() << duration << endl;
  out << steps << endl;
  out.close();
  
  return frontier.front();
}

void Puzzle::setHead(int x, int y, int wrig)
{
  m_wrigglers[wrig].m_headX = x;
  m_wrigglers[wrig].m_headY = y;
}











