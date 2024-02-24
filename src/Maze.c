#include "Maze.h"

void BlockToString(unsigned int block, char* str)
{
    unsigned int value = block;
    unsigned int binaryNum[32];

    for(int i = 0; i < 32; i++)
        binaryNum[i] = 0;
   
    int i = 0; 
    while (value > 0) 
    { 
        binaryNum[i] = value % 2; 
        value = value / 2; 
        i++;
    } 
    
    for(int i = 0; i < 32; i++)
        str[i] = '0' + binaryNum[32 - i - 1];
}

unsigned int StringToBlock(char* blockString)
{
    unsigned int value = 0;

    unsigned int i = 0, k = 1; 
    while (i < 32) 
    { 
        if(blockString[32 - i - 1] == '1')
            value += k;
        k *= 2; 
        i++;
    }

    return value;
}

char GetMazeCell(MazeData* mazeData, int xPos, int yPos)
{
    if(xPos < 0 || xPos >= mazeData->width || yPos < 0 || yPos >= mazeData->height) return '1';

    int inLinePos = yPos * mazeData->width + xPos;
    int block = inLinePos / 32;
    int inBlockPos = inLinePos - block * 32;

    char blockString[32];
    BlockToString(mazeData->Maze[block], blockString);

    char ToReturn = blockString[inBlockPos];
    
    return ToReturn;
}

void SetMazeCell(MazeData* mazeData, int xPos, int yPos, char value)
{
    if(xPos < 0 || xPos >= mazeData->width || yPos < 0 || yPos >= mazeData->height) return;

    int inLinePos = yPos * mazeData->width + xPos;
    int block = inLinePos / 32;
    int inBlockPos = inLinePos - block * 32;

    char blockString[32];
    BlockToString(mazeData->Maze[block], blockString);

    blockString[inBlockPos] = value;

    mazeData->Maze[block] = StringToBlock(blockString);
}

void SetMazeRect(FILE * file, MazeData* mazeData)
{
    char Head;
    bool widthSet = false;
    int x = 0, y = 0;

    while ((Head = fgetc(file)) != EOF)
    {
        if(Head == '\n')
        {
            if(!widthSet) 
            {
                mazeData->width = x;
                widthSet = true;
            }
            y++;
        }
        x++;
    }

    mazeData->height = y;
    
    rewind(file);
}

void ReadMaze(char* filename, MazeData* mazeData)
{
    FILE* file = fopen(filename, "r");

    SetMazeRect(file, mazeData);

    int blocks = (int)(mazeData->width * mazeData->height / 32 + 0.5);
    mazeData->Maze = malloc(blocks * sizeof(unsigned int));

    for(int i = 0; i < blocks; i++)
        mazeData->Maze[i] = 4294967295;
    

    int x = 0, y = 0;
    char Head;
    while ((Head = fgetc(file)) != EOF)
    {
        if(Head == '\n')
        {
            x = 0;
            y++;
            continue;
        }
        else if (Head == 'P')
        {
            mazeData->Start.x = x;
            mazeData->Start.y = y;
            SetMazeCell(mazeData, x, y, '0');
        }
        else if (Head == 'K')
        {
            mazeData->End.x = x;
            mazeData->End.y = y;
            SetMazeCell(mazeData, x, y, '0');
        }
        else if (Head == ' ')
        {
            SetMazeCell(mazeData, x, y, '0');
        }

        x++;
    }
    
    fclose(file);
}

void WriteMaze(MazeData* mazeData)
{
    printf("START: %d, %d\n", mazeData->Start.x, mazeData->Start.y);
    printf("END: %d, %d\n", mazeData->End.x, mazeData->End.y);

    FILE* f = fopen("binaryMaze.txt", "w");

    for (int y = 0; y < mazeData->height; y++)
    {
        for (int x = 0; x < mazeData->width; x++)
        {
            fprintf(f, "%c", GetMazeCell(mazeData, x, y));
        }
        fprintf(f, "\n");
    }
    
    fclose(f);
}