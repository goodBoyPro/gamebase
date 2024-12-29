#include <GObject.h>
#include <GPlayerChar.h>
#include <GWidget.h>
void GWorldInterface::render(sf::RenderWindow &window_) {
    // 绘制actor
    FVector3 posForDraw = {((int)(getPlayerCharactor()->cameraComPtr->camera.posInWs.x*1000))/1000.f,
                           ((int)(getPlayerCharactor()->cameraComPtr->camera.posInWs.y*1000))/1000.f,
                           0};
    PRINTDEBUG(L"键鼠位置：%f,%f",
               winToWs(sf::Mouse::getPosition(*(Game::gameIns->gameWindow)),
                       posForDraw)
                   .x,
               winToWs(sf::Mouse::getPosition(*(Game::gameIns->gameWindow)),
                       posForDraw)
                   .y);
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
    // if (getWidgetPtr()) {
    //     getWidgetPtr()->draw(window_);
    // }
    GWidgetInterface::drawAllWidget(window_);
    //  绘制鼠标/////////////////////////////
    if (GMouseInterface::mousePtr)
        GMouseInterface::mousePtr->drawMouseCusor(window_);
    // 显示碰撞
    GCollision::showCollisions(window_, posForDraw);
    // 显示DEBUG////////////////////////////////
    GDebug::debugDisplay(window_);
};
