#include "Astar.h"

int GetListCount(AstarNodeElem* list)
{
    int count = 0;

    while (list != NULL && list->Node != NULL)
    {
        count++;

        if(list->Next == NULL) return count;
        list = list->Next; 
    }
    
    return count;    
}

void AddLast(AstarNodeElem** head_ref, AstarNode* new_data) 
{ 
    //create a new node
    AstarNodeElem *newNode = malloc(sizeof(AstarNodeElem));
    newNode->Node = new_data;
    newNode->Next = NULL;

    //if head is NULL, it is an empty list
    if(*head_ref == NULL)
         *head_ref = newNode;
    //Otherwise, find the last node and add the newNode
    else
    {
        AstarNodeElem *lastNode = *head_ref;

        //last node's next address will be NULL.
        while(lastNode->Next != NULL)
        {
            lastNode = lastNode->Next;
        }

        //add the newNode at the end of the linked list
        lastNode->Next = newNode;
    }
}

void delete(AstarNodeElem** head_ref, AstarNode* key) 
{ 
    // Store head node 
    struct AstarNodeElem *temp = *head_ref, *prev; 
  
    // If head node itself holds the key to be deleted 
    if (temp != NULL && temp->Node == key) { 
        *head_ref = temp->Next; // Changed head 
        free(temp); // free old head 
        return; 
    } 
  
    // Search for the key to be deleted, keep track of the 
    // previous node as we need to change 'prev->next' 
    while (temp != NULL && temp->Node != key) { 
        prev = temp; 
        temp = temp->Next; 
    } 
  
    // If key was not present in linked list 
    if (temp == NULL) 
        return; 
  
    // Unlink the node from linked list 
    prev->Next = temp->Next; 
  
    free(temp); // Free memory 
}

AstarNode* getNode(AstarNodeElem** head_ref, int index)
{
    AstarNodeElem* temp = *head_ref;
    int i = 0;
    while (i != index)
    {
        temp = temp->Next;
        i++;
    }
    
    return temp->Node;
}

bool PosInList(AstarNodeElem** head_ref, AstarNode key)
{
    bool flag = false;
    AstarNodeElem* temp = *head_ref;

    while (temp != NULL)
    {
        if(temp->Node->Pos.x == key.Pos.x && temp->Node->Pos.y == key.Pos.y) 
        {
            flag = true;
            break;
        }
        temp = temp->Next;
    }

    return flag;
}

void getNeighbours(AstarNode* neighbours, AstarNode* node, MazeData* mazeData)
{
    for(int i = 0; i < 4; i++)
    {
        int xDir = 0, yDir = 0;
        if(i == 0)      {xDir = 0; yDir = 1;}
        else if(i == 1) {xDir = 1; yDir = 0;}
        else if(i == 2) {xDir = 0; yDir =-1;}
        else if(i == 3) {xDir =-1; yDir = 0;}

        Vector2 Position;
        Position.x = node->Pos.x + xDir;
        Position.y = node->Pos.y + yDir;

        neighbours[i].Parent = node;
        neighbours[i].Pos = Position;
        if(Position.x < 0 || Position.x >= 1024 || Position.y < 0 || Position.y >= 1024)
        {
            neighbours[i].Walkable = false;
        }    
        else
        {
            neighbours[i].Walkable = GetMazeCell(mazeData->Maze, Position.x, Position.y) != '1';
        }

        neighbours[i].gCost += 1;

        int deltaX = mazeData->End.x - Position.x;
        int deltaY = mazeData->End.y - Position.y;
        neighbours[i].hCost = (int)sqrt(deltaX * deltaX + deltaY * deltaY);
    }
}

void AstarSolve(MazeData* mazeData)
{
    AstarNode StartNode;
    StartNode.Pos = mazeData->Start;
    StartNode.Walkable = true;

    AstarNodeElem* OpenSet   = NULL;
    AstarNodeElem* ClosedSet = NULL;
    AddLast(&OpenSet, &StartNode);

    printf("start\n");
    
    int openSetCount;
    while ((openSetCount = GetListCount(OpenSet)) > 0)
    {
        AstarNode* node = getNode(&OpenSet, 0);
        for (int i = 1; i < openSetCount; i++) 
        {
            AstarNode* openSetNode = getNode(&OpenSet, i);
            int openSetNodeFcost = openSetNode->gCost + openSetNode->hCost;
            int nodeFcost = node->gCost + node->hCost;

            if (openSetNodeFcost <= nodeFcost && openSetNode->hCost < node->hCost)
            {
                node = openSetNode;
            }   
        }

        delete(&OpenSet, node);
        AddLast(&ClosedSet, node);

        if(node->Pos.x == mazeData->End.x && node->Pos.y == mazeData->End.y)
        {
            printf("less go\n");
            break;
        }

        AstarNode Neighbours[4];
        getNeighbours(Neighbours, node, mazeData);
        for(int i = 0; i < 4; i++)
        {
            if (!Neighbours[i].Walkable || PosInList(&ClosedSet, Neighbours[i])) continue;

            int newCostToNeighbour = node->gCost + 1;
            if (newCostToNeighbour < Neighbours[i].gCost || !PosInList(&OpenSet, Neighbours[i])) 
            {
                Neighbours[i].Parent = node;
                Neighbours[i].gCost = newCostToNeighbour;

                int deltaX = mazeData->End.x - Neighbours[i].Pos.x;
                int deltaY = mazeData->End.y - Neighbours[i].Pos.y;
                Neighbours[i].hCost = (int)sqrt(deltaX * deltaX + deltaY * deltaY);

                if (!PosInList(&OpenSet, Neighbours[i]))
                {
                    AstarNode* new = malloc(sizeof(AstarNode));
                    new->Parent = Neighbours[i].Parent;
                    new->gCost = Neighbours[i].gCost;
                    new->hCost = Neighbours[i].hCost;
                    new->Pos = Neighbours[i].Pos;
                    new->Walkable = Neighbours[i].Walkable;
                    AddLast(&OpenSet, new);
                }   
            }
        }
    }
}