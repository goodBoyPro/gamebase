
// 声明全局变量和预定义
#ifndef GBASE_H
#define GBASE_H
#include <string>


#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <list>
#include <mutex>
#include <stdio.h>
#include <timer.h>
#include <unordered_set>
#include <vector>
#include <windows.h>
#include<atomic>
#include <algorithm>

#define WINH 900
#define WINW 1200

#define FVector sf::Vector2f
#define IVector sf::Vector2i

#define DEBUG
#ifdef DEBUG
#define PRINTF(str)  printf("file【%s】\nline【%d】\nfunction【%s】\nvarname【"#str"】\ntext【%s】\n", __FILE__, __LINE__, __func__,str);
#else 
#define PRINTF(str)
#endif

typedef std::atomic<int> intAtomic;
typedef std::atomic<float> floatAtomic;

template <class T> struct GVector {
    GVector() {};
    GVector(T Gx, T Gy) {
       
        x = Gx;
        y = Gy;
    };
    T x = 0, y = 0;
    GVector operator+(const GVector &G) { return GVector(x + G.x, y + G.y); }
    GVector operator-(const GVector &G) { return GVector(x - G.x, y - G.y); }
    float gLenth(const GVector &G) { return pow(G.x * G.x + G.y * G.y, 0.5f); };
    GVector gNormalize(const GVector &G) {
        T a = gLenth(G);
        return GVector(G.x / a, G.y / a);
    };
};
///////////////////////////////////////////////////////////////////////////////
class GPlayerChar *getPlayerCharactor();
class GController *getPlayerController();
class Game *getGameIns();
class GWorld *getWorld();
class GCamera *getGameCamera();
class GWidget *getWidgetPtr();
class sf::RenderWindow *getWindow();

void setPlayerCharactor(class GPlayerChar *player_);
void setPlayerController(class GController *controller_);
void setGameIns(class Game *game_);
void setWorld(class GWorld *world_);
void setGameCamera(class GCamera *camera_);
void setWidgetPtr(class GWidget *widget_);
void setWindow(sf::RenderWindow *window_);



// 全局变量
//extern class GPlayerChar *playerCharacter;
//extern class GController *playerController;
//extern class Game *gameIns;
//extern sf::RenderWindow *window;
extern sf::Event *gameEvent;
//extern class GWorld *world;
extern int frameLimit;
extern float pixSize;
// 全局相机
//extern class GCamera *gameCamera;
extern int deltaTime;
// Actors
extern std::mutex actorsMutex;
extern std::list<class GActor *> actors;
void addActors(GActor *actor);
extern class GWidget *widgetPtr;

///////////////////////////////////////////////////////////////////////////////////
// 全局方法//

// 锁定画面宽高
void resizeWindow(sf::RenderWindow *window);
// 开关控制台
void openConsoleWindow();
// 游戏中用到的字体
extern sf::Font font[4];

// 初始化游戏资源
void initTools();
// 缺省值应该写在声明中才会重载
// 输出文字，参数列表：文字内容，位置x,位置y,尺寸，颜色，字体
void printText(std::wstring str = L"未输入文字", int x = 0, int y = 0,
               int size = 30, sf::Color color = sf::Color(255, 255, 255),
               sf::Font *font_ = font);
void printNum(float __float, int x = 0, int y = 0, int size = 30,
              sf::Color color = sf::Color(255, 255, 255),
              sf::Font *font_ = font);
void printNum(int __int, int x = 0, int y = 0, int size = 30,
              sf::Color color = sf::Color(255, 255, 255),
              sf::Font *font_ = font);
// 显示鼠标

// 空间变换
IVector wsToWin(const FVector &PositionInWS);
FVector winToWs(const IVector &positionInWin);
template <class T> float getLength(const T &vec) {
    return pow(vec.x * vec.x + vec.y * vec.y, 0.5f);
}
// 标准化
FVector normalize(const FVector &G);

namespace nsg {

void funcEmpty();
float fInterpTo(float current, float target, float speed);
// 使用缓动函数的插值
float smoothInterpolateTo(float current, float target, float speed,
                          float deltaTime);
};

#endif