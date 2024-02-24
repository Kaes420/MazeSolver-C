#ifndef _MAZE_H
#define _MAZE_H

#include "structs.h"

void BlockToString(unsigned int block, char* str);

unsigned int StringToBlock(char* blockString);

void ReadMaze(char* filename, MazeData* mazeData);

void WriteMaze(MazeData* mazeData);

char GetMazeCell(MazeData* mazeData, int xPos, int yPos);

void SetMazeCell(MazeData* mazeData, int xPos, int yPos, char value);

#endif
