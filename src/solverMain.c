#include "structs.h"
#include "Maze.h"
#include "DeadEndFill.h"
#include "Astar.h"

int main(int argc, char** argv)
{   
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    MazeData MazeData;
    ReadMaze(argv[1], &MazeData);

    //DeadEndSolve(&MazeData);
    //AstarSolve(&MazeData);
    //WriteMaze(&MazeData);
    
    free(MazeData.Maze);

    gettimeofday(&stop, NULL);

    printf("Czas: %0.3lf", ((double)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec)) / 1000);
}