#if !defined(GRIDMAP_H)
#define GRIDMAP_H
#include <GBase.h>
#include <list>
#include <listsafe.h>
#include <set>
extern float gridmapNodeWidth;
extern float gridmapNodeHeight;
inline int releasedActorNum = 0;
template <class T> struct gridmapNode {
    gridmapNode *left = nullptr;
    gridmapNode *leftup = nullptr;
    gridmapNode *up = nullptr;
    gridmapNode *rightup = nullptr;
    gridmapNode *right = nullptr;
    gridmapNode *rightdown = nullptr;
    gridmapNode *down = nullptr;
    gridmapNode *leftdown = nullptr;
    ListSafe<T> actors;
    FVector2 point;
    bool contain(const FVector3 &pos) {
        return pos.x >= point.x && pos.x < point.x + gridmapNodeWidth &&
               pos.y >= point.y && pos.y < point.y + gridmapNodeHeight;
    }

    ~gridmapNode() {
        for (auto elem : actors) {
            delete elem;
            releasedActorNum++;
        }
    }
};

template <class T> class GridMap {
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
    std::vector<T> actorsAlive;
    int getPositionIndex(const FVector3 &pos) {
        if (pos.x < edgeLeft || pos.x >= edgeRight || pos.y < edgeUp ||
            pos.y >= edgeDown)
            return 0;
        int column_ = (pos.x - beginPoint.x) / width;
        int row_ = (pos.y - beginPoint.y) / height;
        return row_ * column + column_;
    }
    GridMap(FVector2 beginPoint_, int row_, int column_, float height_,
            float width_)
        : beginPoint(beginPoint_), row(row_), column(column_), height(height_),
          width(width_) {
        gridmapNodeWidth = width;
        gridmapNodeHeight = height;
        edgeUp = beginPoint.y + height;
        edgeDown = beginPoint.y + height * (row - 1);
        edgeLeft = beginPoint.x + width;
        edgeRight = beginPoint.x + width * (column - 1);
        int num = row * column;
        allNode = new gridmapNode<T>[num];
        for (int i = 0; i < num; i++) {
            gridmapNode<T> &an = allNode[i];

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
                i - column - 1 > column - 1 && (i - column - 1) % column != 0
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
    std::vector<T> badActors;
    int addActor(T a) {
        int id = getPositionIndex(a->getPosInWs());
        if (id) {
            allNode[id].actors.addActor(a);
            return id;
        } else {
            printf("actor is out edge,been deleted");
            badActors.push_back(a);
            return 0;
        }
    }
    void setActorsAlive(int centerId) {

        for (auto elem : badActors)
            delete elem;
        badActors.clear();
        gridmapNode<T> &gridNode = allNode[centerId];

        gridNode.actors.pollList([&](T a) { actorsAlive.push_back(a); });
        gridNode.left->actors.pollList([&](T a) { actorsAlive.push_back(a); });
        gridNode.right->actors.pollList([&](T a) { actorsAlive.push_back(a); });
        gridNode.up->actors.pollList([&](T a) { actorsAlive.push_back(a); });
        gridNode.down->actors.pollList([&](T a) { actorsAlive.push_back(a); });
        gridNode.leftup->actors.pollList(
            [&](T a) { actorsAlive.push_back(a); });
        gridNode.leftdown->actors.pollList(
            [&](T a) { actorsAlive.push_back(a); });
        gridNode.rightup->actors.pollList(
            [&](T a) { actorsAlive.push_back(a); });
        gridNode.rightdown->actors.pollList(
            [&](T a) { actorsAlive.push_back(a); });

        std::sort(actorsAlive.begin(), actorsAlive.end(), [](T a, T b) {
            if (a->getPosInWs().y != b->getPosInWs().y)
                return a->getPosInWs().y < b->getPosInWs().y;
            return a->getPosInWs().x < b->getPosInWs().x;
        });
    }
    void changeActorNode(T ptr, int idNew, int idOld) {
        allNode[idOld].actors.remove(ptr);
        allNode[idNew].actors.addActor(ptr);
    }
    int getActorsNumber() {
        int a = 0;
        int num = row * column;
        for (int i = 0; i < num; i++) {
            a += allNode[i].actors.size();
        }

        return a;
    }

    ~GridMap() {         
        delete[] allNode;
        printf("releasedActorNumber:%d\n", releasedActorNum);
       
    }
};

#endif // GRIDMAP_H
