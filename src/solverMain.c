#include "structs.h"
#include "Maze.h"
#include "DeadEndFill.h"
#include "Astar.h"

int main(int argc, char** argv)
{   
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    MazeData MazeData;

    char subbuff[6];
    memcpy(subbuff, &argv[1][strlen(argv[1]) - 4], 4);
    subbuff[5] = '\0';

    if(strcmp(subbuff, ".bin") == 0)
        ReadBinMaze(argv[1], &MazeData);
    else    
        ReadMaze(argv[1], &MazeData);




    DeadEndSolve(&MazeData);
    //AstarSolve(&MazeData);
    //WriteMaze(&MazeData);

    if(strcmp(subbuff, ".bin") == 0)
        WriteBinMaze(argv[1]);
    
    free(MazeData.Maze);

    gettimeofday(&stop, NULL);

    printf("Czas: %0.3lf\n", ((double)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec)) / 1000);

} 