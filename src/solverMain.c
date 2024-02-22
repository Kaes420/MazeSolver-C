#include <stdio.h>
#include <stdlib.h>
#include "Maze.h"

int main(int argc, char** argv)
{   
    unsigned int* Maze = ReadMaze(argv[1]);

    WriteMaze(Maze);
    
    free(Maze);
}