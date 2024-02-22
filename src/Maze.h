char* BlockToString(int block);

unsigned int StringToBlock(char* blockString);

unsigned int* ReadMaze(char* filename);

void WriteMaze(unsigned int* Maze);

char GetMazeCell(unsigned int* Maze, int xPos, int yPos);

void SetMazeCell(unsigned int* Maze, int xPos, int yPos, char value);
