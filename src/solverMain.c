#include <stdio.h>
#include <stdlib.h>
#include "Maze.h"

int main(int argc, char** argv)
{   
    MazeData MazeData;
    ReadMaze(argv[1], &MazeData);


    //WriteMaze(&MazeData);
    
    free(MazeData.Maze);
}