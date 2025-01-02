#include "GBase.h"
#include "GPlayerChar.h"
#include <GL/glew.h>
// #include <GWidget.h>
#include <GWorld.h>
#include <cmath>
#include <math.h>
// 全局变量
GPlayerChar *playerCharacter = nullptr;

GWorld *world = nullptr;
int frameLimit = 60;
float pixSize = 0.01;
int deltaTime;
GWidget *widgetPtr = nullptr;

// 接口
GPlayerChar *getPlayerCharactor() { return playerCharacter; }
GWorld *getWorld() { return world; }

sf::RenderWindow *createwindow() {
    // sf::ContextSettings settings;

    // settings.depthBits = 24;
    // settings.stencilBits = 8;
    // const float W_H_ratio = static_cast<float>(WINW) / WINH;
    // const float FOV = 45.0f;
    const sf::String TITLE = "game";
    sf::RenderWindow *window=new sf::RenderWindow(
        sf::VideoMode(WINW, WINH, 32), TITLE,
        sf::Style::Titlebar|sf::Style::Close| sf::Style::Resize);

    window->setActive();

    // window.setMouseCursorVisible(false);
    // window.setMouseCursorGrabbed(true);

    // 初始化GLEW 必须再窗口之后
    // glewExperimental = GL_TRUE;
    // glewInit();
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_TEXTURE_2D);
    
    return window;
}

GWidget *getWidgetPtr() { return widgetPtr; }

void setPlayerCharactor(class GPlayerChar *player_) {
    playerCharacter = player_;
}
// void setPlayerController(class GController *controller_) {
//     playerController = controller_;
// }

void setWorld(class GWorld *world_) { world = world_; }

void setWidgetPtr(class GWidget *widget_) { widgetPtr = widget_; }



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
    if (!font[0].loadFromFile("res/font/heiti.ttf"))
        PRINTF("font1加载失败\n");
    if (!font[1].loadFromFile("res/font/kaiti.ttf"))
        PRINTF("font2加载失败\n");
    if (!font[2].loadFromFile("res/font/zhunyuan.ttf"))
        PRINTF("font3加载失败\n");
}

void printText(sf::RenderWindow&window_,const std::wstring &str, int x, int y, int size, sf::Color color,
               sf::Font &font_) {
    static sf::Text text;
    text.setFont(font_);
    text.setString(str);
    text.setPosition(sf::Vector2f(x, y));
    text.setFillColor(color);
    text.setCharacterSize(size);
    window_.draw(text);
}

void printNum(sf::RenderWindow&window_,float __float, int x, int y, int size, sf::Color color,
              sf::Font &font_) {
    wchar_t num[32];
    swprintf_s(num, L"%f", __float);
    printText(window_,num, x, y, size, color, font_);
}

void printNum(sf::RenderWindow&window_,int __int, int x, int y, int size, sf::Color color,
              sf::Font &font_) {
    wchar_t num[32];
    swprintf_s(num, L"%d", __int);
    printText(window_,num, x, y, size, color, font_);
}

FVector3 normalize(const FVector3 &G) {
    float a = pow(G.x * G.x + G.y * G.y, 0.5f);
    if (a)
        return {G.x / a, G.y / a, 0};
    return {0, 0, 0};
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

// 从形如“11,13”的字符串接受整数
sf::Vector2i strTo2Int(const std::string &str) {
    size_t pos = str.find(',');
    return {std::stoi(str.substr(0, pos)), std::stoi(str.substr(pos + 1))};
}
// 从形如“11,13”的字符串接受浮点
FVector3 strTo3Float(const std::string &str) {
    float a, b, c;
    size_t pos1 = str.find(',');
    a = std::stof(str.substr(0, pos1));
    size_t pos2 = str.find(',',pos1+1);
    b = std::stof(str.substr(pos1+1, pos2-pos1-1));
    c = std::stof(str.substr(pos2+1));
    return {a,b,c};
}

std::string combineStrings(std::initializer_list<const std::string>strs){
    std::string rtn;
    for(const std::string &str:strs){
        rtn += str;
    }
    return rtn;
}
}; // namespace nsg