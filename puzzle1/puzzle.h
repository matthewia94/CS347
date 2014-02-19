/*******************************************************************************
* @file puzzle.h
* @author Matthew Anderson <mia2n4>
* @version 1.0
* @brief header file for the wriggler puzzle 
*******************************************************************************/

#ifndef PUZZLE_H
#define PUZZLE_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <queue>
#include <stack>
#include <ctime>
#include <cstdio>

using namespace std;

/*******************************************************************************
* @class Puzzle
* @brief A class which holds the information of the wriggler puzzle
*******************************************************************************/

/*******************************************************************************
* @fn findNum
* @brief finds the wriggler number of the wriggler with head at headX and headY
* @pre a non empty puzzle board with at least one wriggler with a head at headX 
       and headY
* @post returns the number of the wriggler
* @param headX the x index of the head of a wriggler
* @param headY the y index of the head of a wriggler
* @return returns the number of the wriggler with the head at headX and headY
*******************************************************************************/

/*******************************************************************************
* @fn nextPiece
* @brief finds the position of the next piece given a piece of a wriggler
* @pre a valid x and y where a piece besides the tail of the wriggler resides
* @post returns by reference the x and y of the next piece of the wriggler
* @param x the x coordinate of a piece of the wriggler
* @param y the y coordinate of a piece of the wriggler
* @return returns x and y coordinates of the next piece of the wriggler by 
          reference
*******************************************************************************/

/*******************************************************************************
* @fn convertHead
* @brief converts a head character to its corresponding body character
* @pre valid head character
* @post returns the body character of the corresponding head character
* @param dir the head character to be converted
* @return the corresponding body character
*******************************************************************************/

/*******************************************************************************
* @fn findTail
* @brief finds the coordinates of the body piece directly before the tail
* @pre valid coordinates of a head of a wriggler
* @post returns by reference the coordinates of the body piece right before the 
        tail
* @param headX x coordinate of the head of a wriggler
* @param headY y coordinate of the head of a wriggler
* @param x the reference variable which will hold the x coordinate of the tail
* @param y the reference variable which will hold the y coordinate of the tail
* @return returns by reference the coordinates of the piece directly before the
          tail
*******************************************************************************/

/*******************************************************************************
* @fn convertToHead
* @brief converts a body character to its corresponding head character
* @pre valid body character
* @post returns the body character which corresponds dir
* @return returns
*******************************************************************************/

/*******************************************************************************
* @fn Puzzle
* @brief default constructor for the puzzle class
* @post creates an empty board with 0 height and 0 width
*******************************************************************************/

/*******************************************************************************
* @fn Puzzle(int h, int w, int wrig)
* @brief constructor for the puzzle class which creates a board to specification
* @pre a positive number for h, w and wrig
* @post a puzzle which has height h, width w and wrig wriglers
* @param h a positive number which will be stored as the height of the board
* @param w a positive number which will be stored as the width of the board
* @param wrig a positive number which is the number of wriglers on the board
*******************************************************************************/

/*******************************************************************************
* @fn ~Puzzle
* @brief deconstructor for the Puzzle class which frees up dynamic memory
* @post all dynamically declared memory for puzzle is freed
*******************************************************************************/

/*******************************************************************************
* @fn operator=
* @brief sets the calling instance equal to rhs
* @pre a valid rhs and calling instance
* @post calling instance is identical to rhs
* @param rhs what to set the calling instance to
* @return returns lhs
*******************************************************************************/

/*******************************************************************************
* @fn copy constructor
* @brief the copy constructor for the puzzle class
* @pre a valid rhs
* @post creates a Puzzle instance which is a copy of class
*******************************************************************************/

/*******************************************************************************
* @fn operator<<
* @brief outputs the board to os
* @pre a valid puzzle class with width and height greater than 0
* @post outputs the current board to os
* @param os a stream which will hold the output of the board
* @param p puzzle to output to the stream
*******************************************************************************/

/*******************************************************************************
* @fn operator>>
* @brief inputs the board from a stream
* @pre a valid puzzle class with width and height equal to that found in the 
       file, a properly formatted file
* @post inputs from the stream is into m_board
* @param is a stream from which to read the board
* @param p puzzle to input into 
*******************************************************************************/

/*******************************************************************************
* @fn isSolved
* @brief checks to see if the head of the first wriggler is in the bottom right 
         corner
* @pre a non empty puzzle board
* @post returns whether or not the head of the first wriggler is in the bottom 
        right corner
*******************************************************************************/

/*******************************************************************************
* @fn isValid
* @brief checks to see if coordinates are valid to move to
* @pre a valid puzzle instance
* @post whether or not the x and y are valid to move to
* @param x x coordinate to move to
* @param y y coordinate to move to
* @return returns whether or not the x and y are a valid move
*******************************************************************************/

/*******************************************************************************
* @fn moveHead
* @brief move the wriggler by the head in the given direction
* @pre a valid wriggler with head at x, y
* @post the wriggler has been moved in the given direction
* @param dir an int where 0 is up, 1 is right, 2 is down and 3 is left
* @param x the x coordinate of the wriggler head
* @param y the y coordinate of the wriggler head
* @return whether or not the move was succesful
*******************************************************************************/

/*******************************************************************************
* @fn moveTail
* @brief move the wriggler by the tail in the given direction
* @pre a valid wriggler with tail at x, y
* @post the wriggler has been moved in the given direction
* @param dir an int where 0 is up, 1 is right, 2 is down and 3 is left
* @param x the x coordinate of the wriggler head
* @param y the y coordinate of the wriggler head
* @return whether or not the move was succesful
*******************************************************************************/

/*******************************************************************************
* @fn solveBFTS
* @brief solves the puzzle using BFTS
* @pre a valid puzzle to solve
* @post the puzzle is solved and a solution file is created
* @return returns a solved version of the puzzle
*******************************************************************************/

/*******************************************************************************
* @fn setHead
* @brief sets the heads coordinates to x and y
* @pre a valid x and y for the head
* @post heads coordinates are set to x and y
* @param x sets the heads x coordinate of wrig to x
* @param y sets the heads y coordinate of wrig to y
* @param wrig sets the number of the wriggler to be changed
*******************************************************************************/

class Puzzle
{
  private:
    struct wriggler
    {
      int m_headX;
      int m_headY;
    };
    struct move
    {
      int m_wrigglerID;
      int m_side;
      int m_destinationX;
      int m_destinationY;
    };
    char** m_board;
    int m_height;
    int m_width;
    int m_numWrigs;
    move m_move;
    wriggler* m_wrigglers;
    int m_parent;
    int findNum(int headX, int headY);
    void nextPiece(int& x, int& y);
    char convertHead(char dir);
    void findTail(int headX, int headY, int& x, int& y);
    char convertToHead(char dir);
    
  public:
    Puzzle();
    Puzzle(int h, int w, int wrig);
    ~Puzzle();
    Puzzle& operator=(const Puzzle& rhs);
    Puzzle(const Puzzle& rhs);
    friend ostream& operator<<(ostream& os, Puzzle& p);
    friend istream& operator>>(istream& is, Puzzle& p);
    bool isSolved();
    bool isValid(int x, int y);
    bool moveHead(int dir, int x, int y);
    bool moveTail(int dir, int x, int y);
    Puzzle solveBFTS();
    void setHead(int x, int y, int wrig);
        
};

#endif
