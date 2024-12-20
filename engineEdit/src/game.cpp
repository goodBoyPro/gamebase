#include "game.h"
#include <GActor.h>
#include <GL/glew.h>
#include <GController.h>
#include <GDebug.h>
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

    createwindow();
    setWinIcon();   
    mousePtr = new GMouse;
   
    
}

Game::~Game() {
    // delete getWindow();是静态对象，不需要清理
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
    for (auto ac : actors)
        delete ac;
}

void Game::renderLoop2D() {
    getWindow()->setFramerateLimit(frameLimit);
    std::unique_lock lk(actorsMutex, std::defer_lock);
    std::vector<GControllerInterface *> &allController =
        GControllerInterface::getAllController();
    xlib::getTimer().setPause(false);
    while (bGameContinue) {

       getPlayerController()->pollKey();
        // for(auto ctrl:allController){
        //     if(ctrl)
        //         ctrl->pollKey();
        // }       

        // resizeWindow(window);
        // 绘制地图//////////////////////////////
        if (getWorld())
            getWorld()->drawLoop();
        // 绘制actor
        GCameraInterface::posForDraw=GCameraInterface::getGameCamera()->posInWs;
        GActor::gridMapOfActor.setActorsAlive(getPlayerCharactor()->mapNodeId);
        for (auto elem : GActor::gridMapOfActor.actorsAlive) {
            elem->eventTick();
            elem->dataLoop();
            elem->drawActor();
        }
        lk.lock();
        GActor::gridMapOfActor.actorsAlive.clear();
        lk.unlock();
        // 绘制UI
        if (getWidgetPtr()) {
            getWidgetPtr()->draw();
        }

        //  绘制鼠标/////////////////////////////
        if (mousePtr)
            mousePtr->drawMouseCusor();
        // 显示碰撞
        GCollision::showCollisions();
        // 显示DEBUG////////////////////////////////
        GDebug::debugDisplay();
        PRINTDEBUG(L"drawCall:%ld", GActor::drawCallNum);
        GActor::drawCallNum = 0;

        getWindow()->display();
        getWindow()->clear();

    } // while
}
void Game::setWinIcon() {
    sf::Image ic;
    ic.loadFromFile("res/a.png");
    getWindow()->setIcon(48, 48, ic.getPixelsPtr());
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