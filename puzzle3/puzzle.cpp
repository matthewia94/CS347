/*****************************k**************************************************
* @file puzzle.cpp
* @author Matthew Anderson <mia2n4>
* @version 1.0
* @brief function implementation file for the wriggler puzzle class
*******************************************************************************/

#include "puzzle.h"

using namespace std;

bool Puzzle::isSolved() const
{
  bool goal = false;
  if(m_board[m_height - 1][m_width - 1] == '0')
  {
    goal = true;
  }
  else if(m_board[m_height - 1][m_width - 1] == 'U' || 
     m_board[m_height - 1][m_width - 1] == 'L')
  {
    if(findNum(m_width - 1, m_height - 1) == 0)
      goal = true;
  }
  return goal;
}

int Puzzle::findNum(int headX, int headY) const
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

void Puzzle::findTail(int headX, int headY, int& x, int& y) const
{
  int tempX = headX, tempY = headY;
  
  //trace the body of the wriggler
  do
  {
    x = tempX;
    y = tempY;
    nextPiece(tempX, tempY);
  }while(m_board[tempY][tempX] == 'v' || m_board[tempY][tempX] == '<' || 
        m_board[tempY][tempX] == '>' || m_board[tempY][tempX] == '^');
}

Puzzle::Puzzle()
{
  m_board = NULL;
  m_wrigglers = NULL;
  m_height = 0;
  m_width = 0;
  m_numWrigs = 0;
  m_move.m_parent = -1;
  m_move.m_wrigglerID = 0;
  m_move.m_side = 0;
  m_move.m_destinationX = 0;
  m_move.m_destinationY = 0;
  m_move.m_depth = 0;
}

Puzzle::Puzzle(int h, int w, int wrig)
{
  m_height = h;
  m_width = w;
  m_numWrigs = wrig;
  
  m_move.m_parent = -1;
  m_move.m_wrigglerID = 0;
  m_move.m_side = 0;
  m_move.m_destinationX = 0;
  m_move.m_destinationY = 0;
  m_move.m_depth = 0;  
  
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
  
  m_move.m_parent = rhs.m_move.m_parent;
  m_move.m_wrigglerID = rhs.m_move.m_wrigglerID;
  m_move.m_side = rhs.m_move.m_side;
  m_move.m_destinationX = rhs.m_move.m_destinationX;
  m_move.m_destinationY = rhs.m_move.m_destinationY;
  m_move.m_depth = rhs.m_move.m_depth;
  
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
  
  m_move.m_parent = rhs.m_move.m_parent;
  m_move.m_wrigglerID = rhs.m_move.m_wrigglerID;
  m_move.m_side = rhs.m_move.m_side;
  m_move.m_destinationX = rhs.m_move.m_destinationX;
  m_move.m_destinationY = rhs.m_move.m_destinationY;
  m_move.m_depth = rhs.m_move.m_depth;
  
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


ostream& operator<<(ostream& os, const Puzzle& p)
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
  return is;
}

bool Puzzle::operator<(const Puzzle& rhs) const
{
  bool less = true;
  if(rhs.computeHeur() > computeHeur())
    less = false;
  return less;
}

bool Puzzle::operator==(const Puzzle& rhs) const
{
  for(int i = 0; i < m_height; i++)
  {
    for(int j = 0; j < m_width; j++)
    {
      if(m_board[i][j] != rhs.m_board[i][j])
        return false;
    }
  }
  return true;
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

void Puzzle::nextPiece(int& x, int& y) const
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

void Puzzle::setHead(int x, int y, int wrig)
{
  m_wrigglers[wrig].m_headX = x;
  m_wrigglers[wrig].m_headY = y;
}

bool Puzzle::solveDLTS(int limit, vector<move>& parents)
{
  stack<Puzzle> frontier;
  Puzzle tempEval, temp;
  frontier.push(*this);
  int tailX, tailY;
  bool goal = false;
  move m;
  
  //create tree and store possible moves in a tree
  while(!frontier.empty() && !frontier.top().isSolved())
  {
    parents.push_back(frontier.top().m_move);
    tempEval = frontier.top();
    frontier.pop();
    if(tempEval.m_move.m_depth < limit)
    {
      for(int i = 0; i < m_numWrigs; i++)
      {
        //test each direction
        for(int j = 0; j < 4; j++)
        {
          temp = tempEval;
          if(temp.moveHead(j, temp.m_wrigglers[i].m_headX, 
             temp.m_wrigglers[i].m_headY))
          {
            temp.m_move.m_wrigglerID = i;
            temp.m_move.m_side = 0;
            temp.m_move.m_destinationX = temp.m_wrigglers[i].m_headX;
            temp.m_move.m_destinationY = temp.m_wrigglers[i].m_headY;
            temp.m_move.m_depth++;
            temp.m_move.m_parent = parents.size() - 1;
            frontier.push(temp);
          }
          temp = tempEval;
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
            temp.m_move.m_depth++;
            temp.m_move.m_parent = parents.size() - 1;
            frontier.push(temp);
          }
        }
      }
    }
  }
  
  if(!frontier.empty() && frontier.top().isSolved())
  {
    goal = true;
    m.m_depth = frontier.top().m_move.m_depth;
    m.m_wrigglerID = frontier.top().m_move.m_wrigglerID;
    m.m_side = frontier.top().m_move.m_side;
    m.m_destinationX = frontier.top().m_move.m_destinationX;
    m.m_destinationY = frontier.top().m_move.m_destinationY;
    m.m_parent = frontier.top().m_move.m_parent;    
    parents.push_back(m);
    *this = frontier.top();
  }
  
  return goal;
}

void Puzzle::solveIDDLTS()
{
  int depth = 0;
  vector<move> parents;
  stack<move> path;
  clock_t start;
  double duration;
  move temp;
  ofstream out;
  int steps = 0;
  
  start = clock();
  while(!solveDLTS(depth, parents))
  {
    depth++;
    parents.clear();
  }
  duration = (clock() - start) / (double) CLOCKS_PER_SEC;
  
  temp = parents.back();
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
    out << path.top().m_wrigglerID << " " << path.top().m_side << " " 
        << path.top().m_destinationX << " " << path.top().m_destinationY 
        << endl;
    path.pop();
  }
  out << *this << duration << endl;
  out << steps << endl;
  out.close();
}

int Puzzle::computeHeur() const
{
  int heurHead, heurTail, heur, tailX, tailY;
  
  heurHead = (m_height - m_wrigglers[0].m_headY) + 
             (m_width - m_wrigglers[0].m_headX);
  heur = heurHead;           
             
  findTail(m_wrigglers[0].m_headX, m_wrigglers[0].m_headY, tailX, tailY);
  nextPiece(tailX, tailY);
  heurTail = (m_height - tailY) + (m_width - tailX);
  
  if(heurTail < heurHead)
    heur = heurTail;
  
  return heur;
}

bool Puzzle::inVisited(vector<Puzzle>& visited)
{
  for(int i = 0; i < visited.size(); i++)
  {
    if(visited.at(i) == *this)
      return true;
  }
  return false;
}

void Puzzle::solveGBFGS()
{
  priority_queue<Puzzle> frontier;
  vector<Puzzle> visited;
  stack<Puzzle> path;
  Puzzle tempEval, temp;
  frontier.push(*this);
  int steps, tailX, tailY;
  ofstream out;
  clock_t start;
  double duration;
  
  //create tree and store possible moves in a tree
  start = clock();
  while(!frontier.empty() && !frontier.top().isSolved())
  {
    visited.push_back(frontier.top());
    tempEval = frontier.top();
    frontier.pop();
    for(int i = 0; i < m_numWrigs; i++)
    {
      //test each direction
      for(int j = 0; j < 4; j++)
      {
        temp = tempEval;
        if(temp.moveHead(j, temp.m_wrigglers[i].m_headX, 
           temp.m_wrigglers[i].m_headY))
        {
          if(!temp.inVisited(visited))
          {
            temp.m_move.m_wrigglerID = i;
            temp.m_move.m_side = 0;
            temp.m_move.m_destinationX = temp.m_wrigglers[i].m_headX;
            temp.m_move.m_destinationY = temp.m_wrigglers[i].m_headY;
            temp.m_move.m_depth++;
            temp.m_move.m_parent = visited.size() - 1;
            frontier.push(temp);
          }
        }
        temp = tempEval;
        if(temp.moveTail(j, temp.m_wrigglers[i].m_headX, 
           temp.m_wrigglers[i].m_headY))
        {
          if(!temp.inVisited(visited))
          {
            temp.m_move.m_wrigglerID = i;
            temp.m_move.m_side = 1;
            temp.findTail(temp.m_wrigglers[i].m_headX, 
                          temp.m_wrigglers[i].m_headY, tailX, tailY);
            temp.nextPiece(tailX, tailY);
            temp.m_move.m_destinationX = tailX;
            temp.m_move.m_destinationY = tailY;
            temp.m_move.m_depth++;
            temp.m_move.m_parent = visited.size() - 1;
            frontier.push(temp);
          }
        }
      }
    }
  }
  duration = (clock() - start) / (double) CLOCKS_PER_SEC;
  
  temp = frontier.top();
  path.push(temp);
  while(temp.m_move.m_parent != -1)
  {
    temp = visited.at(temp.m_move.m_parent);
    path.push(temp);
  }
  
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
  out << frontier.top() << duration << endl;
  out << steps << endl;
  out.close();
  
  return;
}
    











