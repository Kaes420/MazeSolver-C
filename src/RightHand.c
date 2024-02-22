#include "RightHand.h"

Vector2 TurnRight(Vector2 Dir)
{
    if(Dir.x == 0 && Dir.y == 1)      {Dir.x = 1; Dir.y = 0;}
    else if(Dir.x == 1 && Dir.y == 0) {Dir.x = 0; Dir.y =-1;}
    else if(Dir.x == 0 && Dir.y ==-1) {Dir.x =-1; Dir.y = 0;}
    else if(Dir.x ==-1 && Dir.y == 0) {Dir.x = 0; Dir.y = 1;}

    return Dir;
}

Vector2 TurnLeft(Vector2 Dir)
{
    if(Dir.x == 0 && Dir.y == 1)      {Dir.x =-1; Dir.y = 0;}
    else if(Dir.x ==-1 && Dir.y == 0) {Dir.x = 0; Dir.y =-1;}
    else if(Dir.x == 0 && Dir.y ==-1) {Dir.x = 1; Dir.y = 0;}
    else if(Dir.x == 1 && Dir.y == 0) {Dir.x = 0; Dir.y = 1;}

    return Dir;
}

void DirToString(char* str, Vector2 Dir)
{
    if(Dir.x == 0 && Dir.y == 1)      strcpy(str, "down\n");
    else if(Dir.x == 1 && Dir.y == 0) strcpy(str, "right\n");
    else if(Dir.x == 0 && Dir.y ==-1) strcpy(str, "up\n");
    else if(Dir.x ==-1 && Dir.y == 0) strcpy(str, "left\n");
}

void NextStep(MazeData* MazeData, FILE* file, Vector2* currentPos, Vector2* dir)
{
    Vector2 rightDir = TurnRight(*dir);
    Vector2 FrontWall;
    Vector2 rightWall;
    
    FrontWall.x = currentPos->x + dir->x; 
    FrontWall.y = currentPos->y + dir->y;
    rightWall.x = currentPos->x + rightDir.x; 
    rightWall.y = currentPos->y + rightDir.y;
    
    char rightWallChar = rightWall.x > 0 && rightWall.y > 0 ? GetMazeCell(MazeData->Maze, rightWall.x, rightWall.y) : '1';
    char FrontWallChar = FrontWall.x > 0 && FrontWall.y > 0 ? GetMazeCell(MazeData->Maze, FrontWall.x, FrontWall.y) : '1';

    if(rightWallChar == '0')
    {
        *dir = rightDir;
        *currentPos = rightWall;
        char dirStr[8];
        DirToString(dirStr, *dir);
        fprintf(file, "%s", dirStr);
        fprintf(file, "go\n");

    }
    else if(FrontWallChar == '1')
    {
        *dir = TurnLeft(*dir);
        char dirStr[8];
        DirToString(dirStr, *dir);
        fprintf(file, "%s", dirStr);
    }
    else if(FrontWallChar == '0')
    {
        *currentPos = FrontWall;
        fprintf(file, "go\n");
    }

    if(currentPos->x == MazeData->End.x && currentPos->y == MazeData->End.y) 
        return;
    
    NextStep(MazeData, file, currentPos, dir);
}

void RightHandSolve(MazeData* MazeData)
{
    FILE* file = fopen("path.txt", "w");

    Vector2 CurrentPos = MazeData->Start;
    Vector2 Dir; Dir.x = 0; Dir.y = 1;

    NextStep(MazeData, file, &CurrentPos, &Dir);

    fclose(file);
}