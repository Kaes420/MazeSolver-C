#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>

typedef struct Vector2 
{
    int x, y;
} Vector2;

typedef struct MazeData 
{
    unsigned int* Maze;
    int width, height;
    Vector2 Start, End;
} MazeData;

typedef struct AstarNode 
{
    Vector2 Pos;
    struct AstarNode* Parent;
    bool Walkable;
    int gCost, hCost;
} AstarNode;

typedef struct AstarNodeElem
{
    AstarNode* Node;
    struct AstarNodeElem* Next;
} AstarNodeElem;

typedef struct BinFileHeader
{
    int FileId;
    char Escape;
    short Columns;
    short Lines;
    short StartX;
    short StartY;
    short EndX;
    short EndY;
    char Reversed[12];
    int Counter;
    int SolutionOffset;
    char Separator;
    char Wall;
    char Path;
} BinFileHeader;

#endif