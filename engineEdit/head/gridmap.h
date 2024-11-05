#if !defined(GRIDMAP_H)
#define GRIDMAP_H
#include <GBase.h>
#include <list>

template<class T>
struct gridmapNode {
    gridmapNode *left = nullptr;
    gridmapNode *leftup = nullptr;
    gridmapNode *up = nullptr;
    gridmapNode *rightup = nullptr;
    gridmapNode *right = nullptr;
    gridmapNode *rightdown = nullptr;
    gridmapNode *down = nullptr;
    gridmapNode *leftdown = nullptr;
    std::list<T *> actors;
    FVector2 point;
    static float width;
    static float height;
    bool contain(const FVector3 &pos) {
        return pos.x >= point.x && pos.x < point.x + width && pos.y >= point.y &&
               pos.y < point.y + height;
    }
};
template<class T>
class GridMap {
  public:
    FVector2 beginPoint;
    int row;
    int column;
    float height;
    float width;
    float edgeLeft;
    float edgeRight;
    float edgeUp;
    float edgeDown;
    gridmapNode<T> *allNode;
    int getPositionIndex(const FVector3 &pos) {
        if(pos.x<edgeLeft||pos.x>=edgeRight||pos.y<edgeUp||pos.y>=edgeDown)
            return -1;
        int column_ = (pos.x - beginPoint.x) / width;
        int row_ = (pos.y - beginPoint.y) / height;
        return row_*column+column_;
    }
    GridMap(FVector2 beginPoint_, int row_, int column_, float height_,
            float width_)
        : beginPoint(beginPoint_), row(row_), column(column_), height(height_),
          width(width_) {
        gridmapNode::width = width;
        gridmapNode::height = height;
        edgeUp = beginPoint.y+height;
        edgeDown =beginPoint.y+height*(row-1) ;
        edgeLeft =beginPoint.x+width ;
        edgeRight =beginPoint.x+width*(column-1) ;
        int num = row * column;
        allNode = new gridmapNode[num];
        for (int i = 0; i < num; i++) {
            gridmapNode &an = allNode[i];

            int x = i / column;
            int y = i % column;
            if (x == 0 || y == 0 || y == column - 1 || x == row - 1) {
                continue;
            }

            an.down =
                i + column < num - row ? &allNode[i + column] : nullptr; // ok
            an.left = (i - 1) % column != 0 ? &allNode[i - 1] : nullptr; // ok
            an.leftdown =
                i + column - 1 < num - row && (i + column - 1) % column != 0
                    ? &allNode[i + column - 1]
                    : nullptr;
            an.leftup =
                i - column - 1 && (i - column - 1) % column != 0 > column - 1
                    ? &allNode[i - column - 1]
                    : nullptr;
            an.right = (i + 1) % column != column - 1 ? &allNode[i + 1]
                                                      : nullptr; // ok
            an.rightdown = i + column + 1 < num - row &&
                                   (i + column + 1) % column != column - 1
                               ? &allNode[i + column + 1]
                               : nullptr;
            an.rightup = i - column + 1 > column - 1 &&
                                 (i - column + 1) % column != column - 1
                             ? &allNode[i - column + 1]
                             : nullptr;
            an.up =
                i - column > column - 1 ? &allNode[i - column] : nullptr; // ok
        }
    }
    ~GridMap() { delete[] allNode; }
    
};

#endif // GRIDMAP_H
