#include "structs.h"
#include "Maze.h"
#include "DeadEndFill.h"

int main(int argc, char** argv)
{   
    MazeData MazeData;
    ReadMaze(argv[1], &MazeData);


    DeadEndSolve(&MazeData);
    //WriteMaze(&MazeData);
    
    free(MazeData.Maze);
}