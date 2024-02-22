typedef struct Vector2 
{
    int x, y;
} Vector2;

typedef struct MazeData 
{
    unsigned int* Maze;
    Vector2 Start, End;
} MazeData;

void BlockToString(unsigned int block, char* str);

unsigned int StringToBlock(char* blockString);

void ReadMaze(char* filename, MazeData* mazeData);

void WriteMaze(MazeData* mazeData);

char GetMazeCell(unsigned int* Maze, int xPos, int yPos);

void SetMazeCell(unsigned int* Maze, int xPos, int yPos, char value);
