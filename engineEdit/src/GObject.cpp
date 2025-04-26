#include <GObject.h>
#include <GPlayerChar.h>
// #include <GWidget.h>
void GIWorld::render(sf::RenderWindow &window_) {  
    // 绘制actor
    layerGameRtt.clear();
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
    GIGame::getGameIns()
        ->getWorldActive()
        ->spaceManager->setActorsAlive(getPlayerCharactor()->mapNodeId);
    for (auto elem : GIGame::getGameIns()
                         ->getWorldActive()
                         ->spaceManager->actorsAlive) {
        elem->eventTick();
        elem->dataLoop();
        elem->drawActor(layerGameRtt, posForDraw);
    }
    layerGameRtt.display();
    
    window_.draw(layerGame);
    GIGame::getGameIns()
        ->getWorldActive()
        ->spaceManager->actorsAlive.clear();

    // 绘制UI
    GIWidget::drawAllWidget(window_);
    //  绘制鼠标/////////////////////////////
    if (GIMouse::mousePtr)
        GIMouse::mousePtr->drawMouseCusor(window_);
    // 显示碰撞
    GCollision::showCollisions(window_, posForDraw);
    // 显示DEBUG////////////////////////////////
    GDebug::debugDisplay(window_);
};
