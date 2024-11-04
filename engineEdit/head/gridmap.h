#if !defined(GRIDMAP_H)
#define GRIDMAP_H
#include <list>
#include <GBase.h>
class GActor;
struct gridmapNode
{
    gridmapNode *left = nullptr;
    gridmapNode *leftup = nullptr;
    gridmapNode *up = nullptr;
    gridmapNode *rightup = nullptr;
    gridmapNode *right = nullptr;
    gridmapNode *rightdown = nullptr;
    gridmapNode *down = nullptr;
    gridmapNode *leftdown = nullptr;
    std::list<GActor *> actors;
    FVector2 point;
    static float x;
    static float y;
    bool contain(const FVector3 &pos)
    {
        return pos.x >= point.x && pos.x < point.x + x && pos.y >= point.y && pos.y < point.y + y;
    }
};
class GridMap
{
public:
    FVector2 beginPoint;
    int row;
    int column;
    float height;
    float width;
    int getPositionIndex(const FVector3&pos){return 0;}
    bool isIndexValid(int a){return true;}
    GridMap()
    {
        int num = row * column;
        allNode = new gridmapNode[num];
        for (int i = 0; i < num; i++)
        {
            gridmapNode &an = allNode[i];
           
            int x = i / column;
            int y = i % column;
            if (x == 0||y==0||y==column-1||x==row-1)
            {
                continue;
            }            

            an.down =i+column<num-row? &allNode[i + column]:nullptr;//ok
            an.left =(i-1)%column!=0? &allNode[i - 1]:nullptr;//ok
            an.leftdown = i + column - 1<num-row?&allNode[i + column - 1]:nullptr;//ok
            an.leftup =i - column - 1>column-1? &allNode[i - column - 1]:nullptr;//ok
            an.right = (i + 1)%column!=column-1?&allNode[i + 1]:nullptr;//ok
            an.rightdown =i + column + 1<num-row? &allNode[i + column + 1]:nullptr;//ok
            an.rightup = i - column + 1>column-1?&allNode[i - column + 1]:nullptr;//ok
            an.up = i - column>column-1?&allNode[i - column]:nullptr;//ok
        }
    }
    gridmapNode *allNode;
};

#endif // GRIDMAP_H
