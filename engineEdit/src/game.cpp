#include "game.h"
#include <GActor.h>
#include <GCamera.h>
#include <GController.h>
#include <GDebug.h>
#include <GMouse.h>
#include <worldTest.h>
sf::RenderTexture *renderTexGl;
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
sf::RenderWindow *createwindow() {
    sf::ContextSettings settings;

    settings.depthBits = 24;
    settings.stencilBits = 8;
    //const float W_H_ratio = static_cast<float>(WINW) / WINH;
    //const float FOV = 45.0f;
    const sf::String TITLE = "game";
    static sf::RenderWindow window(
        sf::VideoMode(WINW, WINH, 32), TITLE,
        sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, settings);

    window.setActive();

     window.setMouseCursorVisible(false);
     window.setMouseCursorGrabbed(true);
     
    // 初始化GLEW 必须再窗口之后
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    return &window;
}

Game::Game() {
    // 首先调用来初始化opengl
    // window = new sf::RenderWindow(sf::VideoMode(WINW, WINH), "game");
    window = createwindow();
    setWinIcon();
    gameIns = this;
    renderTexGl = createRenderTexGl(window);

    mouseGame = new GMouse;
    gameEvent = &event;
    gameController = new GController;
    playerController = gameController;

    world1 = new worldTest;
    world = world1;

    std::thread dLoop(dataLoop, this);
    renderLoop2D();
    dLoop.join();
}

Game::~Game() {
    delete mouseGame;
    delete window;
    delete gameController;
    delete world1;
}
std::vector<GActor *> actorsOn;
sf::Time sft__ = sf::microseconds(1);
void sleepCustom() { sf::sleep(sft__); }
void Game::dataLoop() {
    static DWORD timeFlag = 0;

    std::list<GActor *>::iterator it = actors.begin();
    std::unique_lock lk(actorsMutex, std::defer_lock);

    while (bGameContinue) {
        timeFlag = GetTickCount();
        it = actors.begin();
        lk.lock();
        actorsOn.clear();
        // 相机跟随:在开启场景剔除功能后，不在这里调用可能会因为玩家离相机过远无法执行
        playerCharacter->cameraFollowPlayer();
        for (; it != actors.end();) {
            if ((*it)->isValid == 0) {

                delete (*it);
                it = actors.erase(it);

                continue;
            }
            // 性能较差
            // thread_pool::ThreadPool.addTask([=]() { (*it)->dataLoop(); });
            //(*it)->dataLoop();
            if (fabs((*it)->posInWs.x - gameCamera->posInWs.x) < loopDistance &&
                fabs((*it)->posInWs.y - gameCamera->posInWs.y) < loopDistance) {

                actorsOn.push_back(*it);
                (*it)->dataLoop();
            }

            it++;
        }
        lk.unlock();
        
        world->dataLoop();
        if (GetTickCount() == timeFlag)
            threadSleep(1);

        deltaTime = GetTickCount() - timeFlag;
    } // while
    for (auto ac : actors)
        delete ac;
}

void Game::renderLoop2D() {

     window->setFramerateLimit(frameLimit);
    std::unique_lock lk(actorsMutex, std::defer_lock);

    while (bGameContinue) {

        pollKey();

       // resizeWindow(window);
        // 绘制地图//////////////////////////////
        worldCurrent->drawLoop();
        // 绘制actor////////////////////////////
        lk.lock();
         
        // std::vector<GActor *> temp(actors.begin(), actors.end());
        std::sort(actorsOn.begin(), actorsOn.end(), [](GActor *a, GActor *b) {
            if (a->posInWs.y != b->posInWs.y)
                return a->posInWs.y < b->posInWs.y;
            return a->posInWs.x < b->posInWs.x;
        });
        for (auto ac : actorsOn) {

            ac->drawLoop();
        }

        lk.unlock();
        // 绘制UI
        if (widgetPtr) {
            //window->setView(GWidget::viewPort);
            widgetPtr->draw();
        }
        //window->setView(window->getDefaultView());
        // 绘制鼠标/////////////////////////////
        if (mouseGame)
            mouseGame->drawMouseCusor();
        // 显示DEBUG////////////////////////////////
        debugDisplay();

        window->display();
        window->clear();

    } // while
}

sf::Sprite sprGl;

void Game::pollKey() {
    if (playerController) {
        playerController->pollKey();
    }
}

void Game::setWinIcon() {
    sf::Image ic;
    ic.loadFromFile("res/a.png");    
    window->setIcon(48, 48,ic.getPixelsPtr());
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