#include "Maze.h"
#include "RightHand.h"

int main(int argc, char** argv)
{   
    MazeData MazeData;
    ReadMaze(argv[1], &MazeData);

    RightHandSolve(&MazeData);
    //WriteMaze(&MazeData);
    
    free(MazeData.Maze);
}