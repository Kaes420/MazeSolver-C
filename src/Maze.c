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
            if(x == mazeData->Start.x && y == mazeData->Start.y)
                fprintf(f, "P");
            else if(x == mazeData->End.x && y == mazeData->End.y)
                fprintf(f, "K");
            else
                fprintf(f, "%c", GetMazeCell(mazeData, x, y) == '0' ? ' ' : 'X');
        }
        fprintf(f, "\n");
    }
    
    fclose(f);
}

BinFileHeader binFileHeader;

void ReadBinMaze(char* filename, MazeData* mazeData)
{
    FILE *ptr = fopen(filename,"rb");

    fread(&binFileHeader.FileId ,sizeof(int),1,ptr);
    fread(&binFileHeader.Escape, sizeof(char), 1,ptr);
    fread(&binFileHeader.Columns, sizeof(short), 1,ptr);
    fread(&binFileHeader.Lines, sizeof(short), 1,ptr);
    fread(&binFileHeader.StartX, sizeof(short), 1,ptr);
    fread(&binFileHeader.StartY, sizeof(short), 1,ptr);
    fread(&binFileHeader.EndX, sizeof(short), 1,ptr);
    fread(&binFileHeader.EndY, sizeof(short), 1,ptr);
    fread(binFileHeader.Reversed, sizeof(binFileHeader.Reversed), 1,ptr);
    fread(&binFileHeader.Counter ,sizeof(int),1,ptr);
    fread(&binFileHeader.SolutionOffset ,sizeof(int),1,ptr);
    fread(&binFileHeader.Separator, sizeof(char), 1,ptr);
    fread(&binFileHeader.Wall, sizeof(char), 1,ptr);
    fread(&binFileHeader.Path, sizeof(char), 1,ptr);

    mazeData->width = binFileHeader.Columns;
    mazeData->height = binFileHeader.Lines;

    int blocks = (int)(mazeData->width * mazeData->height / 32 + 0.5);
    mazeData->Maze = malloc(blocks * sizeof(unsigned int));

    for(int i = 0; i < blocks; i++)
        mazeData->Maze[i] = 4294967295;

    mazeData->Start.x = binFileHeader.StartX - 1;
    mazeData->Start.y = binFileHeader.StartY - 1;
    SetMazeCell(mazeData, binFileHeader.StartX, binFileHeader.StartY, '0');
    mazeData->End.x = binFileHeader.EndX - 1;
    mazeData->End.y = binFileHeader.EndY - 1;
    SetMazeCell(mazeData, binFileHeader.EndX, binFileHeader.EndY, '0');

    int CellCount = 0;
    for(int i = 0; i < binFileHeader.Counter; i++)
    {
        unsigned char Separator, Code, Count;

        fread(&Separator, sizeof(char), 1, ptr);
        fread(&Code, sizeof(char), 1, ptr);
        fread(&Count, sizeof(char), 1, ptr);

        for(int a = 0; a < Count + 1; a++)
        {
            int x = CellCount % binFileHeader.Columns;
            int y = CellCount / binFileHeader.Columns;
            CellCount++;

            if(Code == binFileHeader.Path) SetMazeCell(mazeData, x, y, '0');
        }
    }

    fclose(ptr);
}

void WriteBinMaze(char* oldFilename)
{
    FILE* oldFile = fopen(oldFilename, "rb");
    FILE* newFile = fopen("SolvedMaze.bin", "wb");
    FILE* pathFile = fopen("path.txt", "r");

    char c = fgetc(oldFile); 
    while (c != EOF)
    { 
        fputc(c, newFile);
        c = fgetc(oldFile);
    }

    fseek(newFile, 33, SEEK_SET);
    int SolutionOffset = 3 * binFileHeader.Counter;
    fwrite(&SolutionOffset, sizeof(int), 1, newFile);

    fseek(newFile, SolutionOffset, SEEK_SET);
    
    fwrite(&binFileHeader.FileId ,sizeof(int),1,newFile);
    fseek(newFile, SolutionOffset + 8, SEEK_SET);

    unsigned char Code = '\0', Count = 0;
    
    int Steps = 0;
    char word[10];
    while (fscanf(pathFile, "%s", word) != EOF)
    {
        if(Code != '\0' && strcmp(word, "go") != 0)
        {
            fwrite(&Code, sizeof(char), 1, newFile);
            fwrite(&Count, sizeof(char), 1, newFile);
            Steps++;

            Code = '\0';
            Count = 0;
        }

        if(strcmp(word, "up") == 0)
            Code = 'N';
        else if(strcmp(word, "right") == 0)
            Code = 'E';
        else if(strcmp(word, "down") == 0)
            Code = 'S';
        else if(strcmp(word, "left") == 0)
            Code = 'W';
        else if(strcmp(word, "go") == 0)
            Count++;
    }

    fseek(newFile, SolutionOffset + 4, SEEK_SET);
    fwrite(&Steps, sizeof(int), 1, newFile);
    
    fclose(oldFile);
    fclose(newFile);
}