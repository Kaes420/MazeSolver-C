#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct Vector2 
{
    int x, y;
} Vector2;

typedef struct MazeData 
{
    unsigned int* Maze;
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

#endif