#include "DeadEndFill.h"

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
    if(Dir.x == 0 && Dir.y == 1)      strcpy(str, "up\n");
    else if(Dir.x == 1 && Dir.y == 0) strcpy(str, "right\n");
    else if(Dir.x == 0 && Dir.y ==-1) strcpy(str, "down\n");
    else if(Dir.x ==-1 && Dir.y == 0) strcpy(str, "left\n");
}

void NextStep(MazeData* mazeData, FILE* file, Vector2* currentPos, Vector2* dir, bool Fill)
{
    Vector2 rightDir = TurnRight(*dir);
    Vector2 FrontWall;
    Vector2 rightWall;
    Vector2 rightWallAfter;
    
    FrontWall.x = currentPos->x + dir->x; 
    FrontWall.y = currentPos->y + dir->y;
    rightWall.x = currentPos->x + rightDir.x; 
    rightWall.y = currentPos->y + rightDir.y;

    rightWallAfter.x = currentPos->x + rightDir.x - dir->x;
    rightWallAfter.y = currentPos->y + rightDir.y - dir->y;
    
    char rightWallChar = GetMazeCell(mazeData, rightWall.x, rightWall.y);
    char FrontWallChar = GetMazeCell(mazeData, FrontWall.x, FrontWall.y);
    char rightWallAfterChar = GetMazeCell(mazeData, rightWallAfter.x, rightWallAfter.y);

    Vector2 PosBefore = *currentPos;
    bool go = false;

    if(rightWallChar == '0' && rightWallAfterChar == '1')
    {
        *dir = rightDir;
        *currentPos = rightWall;
        char dirStr[8];
        DirToString(dirStr, *dir);
        if(!Fill) fprintf(file, "%s", dirStr);
        if(!Fill) fprintf(file, "go\n");
        go = true;
    }
    else if(FrontWallChar == '1')
    {
        *dir = TurnLeft(*dir);
        char dirStr[8];
        DirToString(dirStr, *dir);
        if(!Fill) fprintf(file, "%s", dirStr);
    }
    else if(FrontWallChar == '0')
    { 
        *currentPos = FrontWall;
        if(!Fill) fprintf(file, "go\n");
        go = true;
    }

    if(currentPos->x == mazeData->End.x && currentPos->y == mazeData->End.y)
    {
        return;
    }        

    //Fill Dead End

    Vector2 up    = PosBefore; up.y    += 1;
    Vector2 right = PosBefore; right.x += 1;
    Vector2 down = PosBefore;  down.y  -= 1;
    Vector2 left = PosBefore;  left.x  -= 1;

    bool upCheck    = GetMazeCell(mazeData, up.x, up.y) == '1';
    bool rightCheck = GetMazeCell(mazeData, right.x, right.y) == '1';
    bool downCheck  = GetMazeCell(mazeData, down.x, down.y) == '1';
    bool leftCheck  = GetMazeCell(mazeData, left.x, left.y) == '1';

    int finalCheck = 0;
    if(upCheck    && !(up.x    == dir->x && up.y    == dir->y)) finalCheck++;
    if(rightCheck && !(right.x == dir->x && right.y == dir->y)) finalCheck++;
    if(downCheck  && !(down.x  == dir->x && down.y  == dir->y)) finalCheck++;
    if(leftCheck  && !(left.x  == dir->x && left.y  == dir->y)) finalCheck++;

    if(finalCheck == 3 && go)
    {
        SetMazeCell(mazeData, PosBefore.x, PosBefore.y, '1');
    }
}

void DeadEndSolve(MazeData* mazeData)
{
    FILE* file = fopen("path.txt", "w");

    Vector2 CurrentPos = mazeData->Start;
    Vector2 Dir; Dir.x = 0; Dir.y = 1;

    bool Fill = true;
    while (CurrentPos.x != mazeData->End.x || CurrentPos.y != mazeData->End.y)
    {
        NextStep(mazeData, file, &CurrentPos, &Dir, Fill);

        if(Fill && (CurrentPos.x == mazeData->End.x && CurrentPos.y == mazeData->End.y))
        {
            Fill = false;
            CurrentPos = mazeData->Start;
            Dir.x = 0; Dir.y = 1;
        }
    }

    fclose(file);
}