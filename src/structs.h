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

#endif