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

void NextStep(MazeData* MazeData, FILE* file, Vector2* currentPos, Vector2* dir, bool Fill)
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
    
    char rightWallChar = rightWall.x > 0 && rightWall.y > 0 ? GetMazeCell(MazeData->Maze, rightWall.x, rightWall.y) : '1';
    char FrontWallChar = FrontWall.x > 0 && FrontWall.y > 0 ? GetMazeCell(MazeData->Maze, FrontWall.x, FrontWall.y) : '1';

    char rightWallAfterChar = rightWallAfter.x > 0 && rightWallAfter.y > 0 ? GetMazeCell(MazeData->Maze, rightWallAfter.x, rightWallAfter.y) : '1';

    Vector2 PosBefore; PosBefore.x = currentPos->x; PosBefore.y = currentPos->y;
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

    if(currentPos->x == MazeData->End.x && currentPos->y == MazeData->End.y)
    {
        return;
    }        

    //Fill Dead End

    Vector2 up    = PosBefore; up.y    += 1;
    Vector2 right = PosBefore; right.x += 1;
    Vector2 down = PosBefore;  down.y  -= 1;
    Vector2 left = PosBefore;  left.x  -= 1;

    bool upCheck    = ((up.x > 0 && up.y > 0)       ? GetMazeCell(MazeData->Maze, up.x, up.y)       : '1') == '1';
    bool rightCheck = ((right.x > 0 && right.y > 0) ? GetMazeCell(MazeData->Maze, right.x, right.y) : '1') == '1';
    bool downCheck  = ((down.x > 0 && down.y > 0)   ? GetMazeCell(MazeData->Maze, down.x, down.y)   : '1') == '1';
    bool leftCheck  = ((left.x > 0 && left.y > 0)   ? GetMazeCell(MazeData->Maze, left.x, left.y)   : '1') == '1';

    int finalCheck = 0;
    if(upCheck    && !(up.x    == dir->x && up.y    == dir->y)) finalCheck++;
    if(rightCheck && !(right.x == dir->x && right.y == dir->y)) finalCheck++;
    if(downCheck  && !(down.x  == dir->x && down.y  == dir->y)) finalCheck++;
    if(leftCheck  && !(left.x  == dir->x && left.y  == dir->y)) finalCheck++;

    if(finalCheck == 3 && go && PosBefore.x > 0 && PosBefore.x <= 1023 && PosBefore.y > 0 && PosBefore.y <= 1023)
    {
        SetMazeCell(MazeData->Maze, PosBefore.x, PosBefore.y, '1');
    }
}

void RightHandSolve(MazeData* MazeData)
{
    FILE* file = fopen("path.txt", "w");

    Vector2 CurrentPos = MazeData->Start;
    Vector2 Dir; Dir.x = 0; Dir.y = 1;

    bool Fill = true;
    while (CurrentPos.x != MazeData->End.x || CurrentPos.y != MazeData->End.y)
    {
        NextStep(MazeData, file, &CurrentPos, &Dir, Fill);

        if(Fill && (CurrentPos.x == MazeData->End.x && CurrentPos.y == MazeData->End.y))
        {
            Fill = false;
            CurrentPos = MazeData->Start;
            Dir.x = 0; Dir.y = 1;
        }
    }

    fclose(file);
}