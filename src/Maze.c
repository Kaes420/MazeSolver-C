#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

char GetMazeCell(unsigned int* Maze, int xPos, int yPos)
{
    int inLinePos = yPos * 1024 + xPos;
    int block = inLinePos / 32;
    int inBlockPos = inLinePos - block * 32;

    char blockString[32];
    BlockToString(Maze[block], blockString);

    char ToReturn = blockString[inBlockPos];
    
    return ToReturn;
}

void SetMazeCell(unsigned int* Maze, int xPos, int yPos, char value)
{
    int inLinePos = yPos * 1024 + xPos;
    int block = inLinePos / 32;
    int inBlockPos = inLinePos - block * 32;

    char blockString[32];
    BlockToString(Maze[block], blockString);

    blockString[inBlockPos] = value;

    Maze[block] = StringToBlock(blockString);
}

unsigned int* ReadMaze(char* filename)
{
    unsigned int* Maze = malloc(32768 * sizeof(unsigned int));

    for(int i = 0; i < 32768; i++)
        Maze[i] = 4294967295;

    FILE* f = fopen(filename, "r");

    int x = 0, y = 0;
    char Head;
    while ((Head = fgetc(f)) != EOF)
    {
        if(Head == '\n')
        {
            x = 0;
            y++;
            continue;
        }
        else if (Head == ' ' || Head == 'P' || Head == 'K')
        {
            SetMazeCell(Maze, x, y, '0');
        }

        x++;
    }
    
    fclose(f);
    
    return Maze;
}

void WriteMaze(unsigned int* Maze)
{
    FILE* f = fopen("binaryMaze.txt", "w");

    for (int y = 0; y < 1024; y++)
    {
        for (int x = 0; x < 1024; x++)
        {
            fprintf(f, "%c", GetMazeCell(Maze, x, y));
        }
        fprintf(f, "\n");
    }
    
    fclose(f);
}