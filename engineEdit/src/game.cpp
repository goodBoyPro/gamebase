#include "game.h"
#include <GActor.h>
#include <GController.h>
#include <GDebug.h>
#include <GL/glew.h>
#include <GMouse.h>
#include <worldTest.h>

sf::RenderTexture *renderTexSpr;
// 单例对象
sf::RenderTexture *createRenderTexGl(sf::RenderWindow *window) {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;

    static sf::RenderTexture rt;

    rt.create(WINW, WINH, window->getSettings());
    rt.setActive();
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    return &rt;
}

Game::Game() {
    // 首先调用来初始化opengl
    // window = new sf::RenderWindow(sf::VideoMode(WINW, WINH), "game");
    gameIns = this;
    gameWindow = createwindow();
    setWinIcon(*gameWindow);
    GIMouse *mouse = new GMouse;
}

Game::~Game() {
    delete gameWindow;
   
}
void Game::gameBegin() {
    std::thread dLoop(dataLoop, this);
    renderLoop2D();
    dLoop.join();
}

sf::Time sft__ = sf::microseconds(1);
void sleepCustom() { sf::sleep(sft__); }
void Game::dataLoop() {
    static DWORD timeFlag = 0;

    while (bGameContinue) {
        timeFlag = GetTickCount();

        if (getWorld())
            getWorld()->dataLoop();
        if (GetTickCount() == timeFlag)
            threadSleep(1);
    } // while
#ifdef GAMEDEBUG
    printf("dataloop stoped\n");
#endif
}

void Game::renderLoop2D() {
    sf::RenderWindow &window_ = *gameWindow;
    window_.setFramerateLimit(frameLimit);
    xlib::getTimer().setPause(false);
    
    while (bGameContinue && window_.isOpen()) {
        
        getPlayerController()->pollKey(window_, event);
        getWorldActive()->render(window_);
        PRINTDEBUG(L"drawCall:%ld", GActor::drawCallNum);
        GActor::drawCallNum = 0;

        window_.display();
        window_.clear();
        setDeltaTime();
    } // while
    window_.close();
    xlib::getTimer().setPause(true);
    delete getGameIns()->getWorldActive();
    getGameIns()->setWorldActive(nullptr);
    xlib::getTimer().clearAllTasks();
    xlib::getTimer().setPause(false);
}
void Game::setWinIcon(sf::RenderWindow &window_) {
    sf::Image ic;
    ic.loadFromFile("res/a.png");
    window_.setIcon(48, 48, ic.getPixelsPtr());
}

// 测试内容////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// void Game::renderLoop() {

//     static GDebug debug;
//     window->setFramerateLimit(frameLimit);

//     std::unique_lock lk(actorsMutex, std::defer_lock);

//     RenderObject ob;
//     while (bGameContinue) {
//         pollKey();
//         // renderTexGl->setActive();
//         glClearColor(0, 0, 0, 0);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         ob.drawGpuInstance();
//         // renderTexGl->display();

//         // renderTexGl->pushGLStates();
//         window->setActive();
//         // sprGl.setTexture(renderTexGl->getTexture());
//         // window->draw(sprGl);

//         // printText();

//         window->display();
//         // window->clear();
//         // renderTexGl->popGLStates();
//     } // while
// }

// void Game::renderRtLoop() {
//     std::unique_lock lk(actorsMutex, std::defer_lock);
//     renderTexGl = createRenderTexGl(window);

//     RenderObject ob;
//     while (bGameContinue) {
//         lk.lock();
//         renderTexGl->setActive();
//         glClearColor(0, 0, 0, 0);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         ob.drawGpuInstance();
//         renderTexGl->display();

//         // window->setActive();
//         // window->display();

//     } // while
// }