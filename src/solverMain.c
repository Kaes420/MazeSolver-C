#include "Maze.h"
#include "RightHand.h"
#include "Astar.h"

int main(int argc, char** argv)
{   
    MazeData MazeData;
    ReadMaze(argv[1], &MazeData);

    AstarSolve(&MazeData);
    //RightHandSolve(&MazeData);
    //WriteMaze(&MazeData);
    
    free(MazeData.Maze);
}