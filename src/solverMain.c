#include "structs.h"
#include "Maze.h"
#include "DeadEndFill.h"

int main(int argc, char** argv)
{   
    MazeData MazeData;
    ReadMaze(argv[1], &MazeData);


    RightHandSolve(&MazeData);
    //WriteMaze(&MazeData);
    
    free(MazeData.Maze);
}