#include "GBase.h"
//#include "GController.h"
#include "GPlayerChar.h"
#include "game.h"
#include <GCamera.h>
#include <GDebug.h>
#include <GWidget.h>
#include <GWorld.h>
#include <cmath>
#include <math.h>
#include<camera.h>
// 全局变量
GPlayerChar *playerCharacter = nullptr;
// GController *playerController = nullptr;
Game *gameIns = nullptr;
sf::RenderWindow *window = nullptr;

sf::Event *gameEvent = nullptr;
GCamera *gameCamera;
GWorld *world = nullptr;

int frameLimit = 60;
float pixSize = 0.01f;
int deltaTime;
GWidget *widgetPtr = nullptr;

// 接口
GPlayerChar *getPlayerCharactor() { return playerCharacter; }
//GController *getPlayerController() { return playerController; }
Game *getGameIns() { return gameIns; }
GWorld *getWorld() { return world; }
sf::RenderWindow *getWindow() { return window; }
GCamera *getGameCamera() { return gameCamera; }
GWidget *getWidgetPtr() { return widgetPtr; }

void setPlayerCharactor(class GPlayerChar *player_) {
    playerCharacter = player_;
}
// void setPlayerController(class GController *controller_) {
//     playerController = controller_;
// }
void setGameIns(class Game *game_) { gameIns = game_; }
void setWorld(class GWorld *world_) { world = world_; }
void setGameCamera(class GCamera *camera_) { gameCamera = camera_; }
void setWidgetPtr(class GWidget *widget_) { widgetPtr = widget_; }
void setWindow(sf::RenderWindow *window_) { window = window_; }

std::mutex actorsMutex;
std::list<class GActor *> actors;
void addActors(GActor *actor) {
    std::unique_lock lk(actorsMutex);
    actors.push_back(actor);
    // actors.emplace(actor);
};

sf::Font font[4];

// 全局方法

void openConsoleWindow() {
    static int flag = 1;
    HWND hwnd = GetConsoleWindow();
    if (flag) {
        ShowWindow(hwnd, HIDE_WINDOW);
        flag = 0;
    } else {
        ShowWindow(hwnd, SHOW_OPENWINDOW);
        flag = 1;
    }

    // ShowWindow(hwnd, SHOW_OPENWINDOW);
}
void resizeWindow(sf::RenderWindow *window) {

    static int height;
    static int width;
    // glfwGetWindowSize(window, &width, &height);
    width = window->getSize().x;
    height = window->getSize().y;
    int vwidth = height * 4 / 3;
    int vHeight = width / 4 * 3;
    if (width > vwidth)
        glViewport((width - vwidth) / 2, 0, vwidth, height);
    else
        glViewport(0, (height - vHeight) / 2, width, vHeight);
}
void initTools() {
    srand((int)time(NULL));

    if (font[0].loadFromFile("res/heiti.ttf"))
        printf("font1加载成功\n");
    else
        printf("font1加载失败\n");
    if (font[1].loadFromFile("res/kaiti.ttf"))
        printf("font2加载成功\n");
    else
        printf("font2加载失败\n");
    if (font[2].loadFromFile("res/zhunyuan.ttf"))
        printf("font3加载成功\n");
    else
        printf("font3加载失败\n");
}

sf::Text text;

void printText(std::wstring str, int x, int y, int size, sf::Color color,
               sf::Font *font_) {

    text.setFont(*font_);
    text.setString(str);
    text.setPosition(sf::Vector2f(x, y));
    text.setFillColor(color);
    text.setCharacterSize(size);

    window->draw(text);
}

void printNum(float __float, int x, int y, int size, sf::Color color,
              sf::Font *font_) {
    wchar_t num[32];
    swprintf_s(num, L"%f", __float);
    printText(num, x, y, size, color, font_);
}

void printNum(int __int, int x, int y, int size, sf::Color color,
              sf::Font *font_) {
    wchar_t num[32];
    swprintf_s(num, L"%d", __int);
    printText(num, x, y, size, color, font_);
}

FVector3 normalize(const FVector3 &G) {
    float a = pow(G.x * G.x + G.y * G.y, 0.5f);
    if (a)
        return {G.x / a, G.y / a,0};
    return {0, 0,0};
};
IVector wsToWin(const FVector3 &PositionInWS) {
    return {(PositionInWS.x - gameCamera->posInWs.x) / pixSize + WINW / 2,
            (PositionInWS.y - gameCamera->posInWs.y) / pixSize + WINH / 2-PositionInWS.z/pixSize};
}
FVector3 winToWs(const IVector &positionInWin) {
    return {(positionInWin.x - WINW / 2) * pixSize + gameCamera->posInWs.x,
            (positionInWin.y - WINH / 2) * pixSize + gameCamera->posInWs.y,0};
}

// FVector vectorWsToWin(const FVector &big, const FVector &end) {
//     return FVector((end.x - big.x) / (getPixSizeWS().x),
//                    (-end.y + big.y) / (getPixSizeWS().y));
// }

sf::Texture texMouse;
sf::Sprite sprMouse;
void mouseCursor() {

    printText(L"我是鼠标", sf::Mouse::getPosition(*window).x,
              sf::Mouse::getPosition(*window).y, 60, sf::Color(255, 0, 0));
};

namespace nsg {
// 变量
void funcEmpty() { printf("emptyFunc"); };
float fInterpTo(float current, float target, float speed) {
    // ;
    // if (speed == 0)
    //     return target;
    // float t = (target - current) / speed;
    // if(t>0.00001)
    // return std::min(current + t,target);
    // else
    //     return std::max(current + t, target);
    // 确保speed在合理的范围内（0到1之间）
    speed = std::clamp(speed, 0.0f, 1.0f);

    // 计算current和target之间的差值
    float difference = target - current;

    // 计算这次迭代中current应该改变的量
    // 使用fabs来确保difference为正，从而speed的符号由difference决定
    float step = difference * speed;

    // 更新current值
    current += step;

    // 如果current超过target（由于浮点精度问题可能会发生），则将其设置为target
    if (std::fabs(current - target) < 0.000001) {
        current = target;
    }

    return current;
}

// 简单的线性插值，不改变原函数中的计算方式，但用于对比
float linearInterpolate(float current, float target, float change) {
    return current + change;
}

// 二次缓出插值
float quadraticEaseOut(float t) { return -t * (t - 2); }

// 使用缓动函数的插值
float smoothInterpolateTo(float current, float target, float speed,
                          float deltaTime) {
    // 确保speed在合理的范围内（但在这个示例中，我们不限制它）
    // speed现在可以理解为每次迭代的“进度比例”

    // 计算current和target之间的差值
    float difference = target - current;

    // 使用deltaTime（两次调用之间的时间差）来计算应该改变的量
    // 这里我们简单地用speed代替复杂的缓动计算中的“时间比例”
    // 但为了展示缓动，我们将使用缓动函数来计算一个“平滑因子”
    float progress =
        speed * deltaTime; // 假设speed在这里与时间和进度的关系成正比
    float t = progress; // t用于缓动函数，通常范围在0到1之间

    // 限制t在0到1之间
    t = std::clamp(t, 0.0f, 1.0f);

    // 应用缓动函数计算平滑因子
    float easingFactor = quadraticEaseOut(t);

    // 计算并应用这次迭代中current应该改变的量
    float step = difference * easingFactor;

    // 更新current值
    current += step;

    // 如果current接近target（由于浮点精度问题），则将其设置为target
    if (std::fabs(current - target) < 0.00001f) {
        current = target;
    }

    return current;
}

// 注意：在实际应用中，deltaTime应该由你的游戏引擎或应用程序框架提供，
// 它表示从上一次调用此函数到当前调用之间的时间差（以秒为单位）。

//从形如“11,13”的字符串接受整数
sf::Vector2i strTo2Int(const std::string &str)
{
  size_t pos = str.find(',');
  return {
      std::stoi(str.substr(0, pos)),
      std::stoi(str.substr(pos + 1))};
}
//从形如“11,13”的字符串接受浮点
sf::Vector2f strTo2Float(const std::string &str)
{
  size_t pos = str.find(',');
  return {
      std::stof(str.substr(0, pos)),
      std::stof(str.substr(pos + 1))};
}

}; // namespace nsg