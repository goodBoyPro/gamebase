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
    mousePtr = new GMouse;
}

Game::~Game() {
    delete gameWindow;
    delete mousePtr;
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

        deltaTime = GetTickCount() - timeFlag;
    } // while
#ifdef GAMEDEBUG
    printf("dataloop stoped\n");
#endif
}

void Game::renderLoop2D() {
    sf::RenderWindow &window_ = *gameWindow;
    window_.setFramerateLimit(frameLimit);
    std::vector<GControllerInterface *> &allController =
        GControllerInterface::getAllController();
    xlib::getTimer().setPause(false);
    while (bGameContinue && window_.isOpen()) {

        getPlayerController()->pollKey(window_, event);
        // for(auto ctrl:allController){
        //     if(ctrl)
        //         ctrl->pollKey();
        // }

        // resizeWindow(window);
        // 绘制地图//////////////////////////////
        if (getWorld())
            getWorld()->drawLoop();
        // 绘制actor
        FVector3 posForDraw = getPlayerCharactor()->cameraComPtr->camera.posInWs;
        PRINTDEBUG(
            L"键鼠位置：%f,%f",
            winToWs(sf::Mouse::getPosition(*(Game::gameIns->gameWindow)),posForDraw).x,
            winToWs(sf::Mouse::getPosition(*(Game::gameIns->gameWindow)),posForDraw).y);
        GGameInterface::getGameIns()
            ->getWorldActive()
            ->spaceManager->setActorsAlive(getPlayerCharactor()->mapNodeId);
        for (auto elem : GGameInterface::getGameIns()
                             ->getWorldActive()
                             ->spaceManager->actorsAlive) {
            elem->eventTick();
            elem->dataLoop();
            elem->drawActor(window_, posForDraw);
        }

        GGameInterface::getGameIns()
            ->getWorldActive()
            ->spaceManager->actorsAlive.clear();

        // 绘制UI
        if (getWidgetPtr()) {
            getWidgetPtr()->draw(window_);
        }

        //  绘制鼠标/////////////////////////////
        if (mousePtr)
            mousePtr->drawMouseCusor(window_);
        // 显示碰撞
        GCollision::showCollisions(window_, posForDraw);
        // 显示DEBUG////////////////////////////////
        GDebug::debugDisplay(window_);
        PRINTDEBUG(L"drawCall:%ld", GActor::drawCallNum);
        GActor::drawCallNum = 0;

        window_.display();
        window_.clear();

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