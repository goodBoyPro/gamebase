#include <gameEditor.h>
namespace ens {
void MovableEditObj::pollKeyActorMdoe(sf::RenderWindow &window_,
                                      sf::Event &event_) {
    static bool bMove = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if(!bMove)
        for (auto it = allMEO.rbegin(); it != allMEO.rend(); ++it) {
            sf::FloatRect bound;
            if ((*it)->type != nsReg::eLandBlock)
                bound = (*it)->shapeForSelect.getGlobalBounds();

            if (bound.contains(window_.mapPixelToCoords(mousePos))) {
                if (MovableEditObj::selectedObjs.find(*it) ==
                    MovableEditObj::selectedObjs.end()) {
                    std::unique_lock lk(MovableEditObj::mutForSelectedObjs);
                    MovableEditObj::selectedObjs.clear();
                    MovableEditObj::selectedObjs.insert(*it);
                    (*it)->returnMessage();
                    lk.unlock();
                }
                bMove = true;
                break;
            }
        }
        // 如果没找到
        if (!bMove) {
            if (SelectRect::rect.bSetBegin) {
                SelectRect::rect.beginPoint = {
                    sf::Mouse::getPosition(window_).x,
                    sf::Mouse::getPosition(window_).y};
                SelectRect::rect.bDraw = true;
                SelectRect::rect.bSetBegin = false;
            }
            std::unique_lock lk(MovableEditObj::mutForSelectedObjs);
            MovableEditObj::selectedObjs.clear();
            lk.unlock();
        }

        else if (!SelectRect::bDraw) {
            for (MovableEditObj *meo : MovableEditObj::selectedObjs) {
                meo->setPosInWs(meo->getPosInWs() + deltaWorldMove);
            }
        }
    }

    if (event_.type == sf::Event::MouseButtonReleased) // 有问题，需要指定按键
    {
        if (event_.key.code == sf::Mouse::Left) {
            bMove = false;
            if (SelectRect::rect.bDraw) {
                for (MovableEditObj *meo : MovableEditObj::allMEO) {
                    if (SelectRect::rect.bound.getGlobalBounds().contains(
                            meo->spr.getPosition()) &&
                        meo->type == nsReg::eActorStatic) {
                        std::unique_lock lk(MovableEditObj::mutForSelectedObjs);
                        MovableEditObj::selectedObjs.insert(meo);
                        lk.unlock();
                    }
                }
            }
            SelectRect::rect.bDraw = false;
            SelectRect::rect.bSetBegin = true;
        }
    }
}
void MovableEditObj::pollKeyLandMdoe(sf::RenderWindow &window_,
                                     sf::Event &event_) {
    static bool bMove = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if(!bMove)
            for (auto it = allMEO.rbegin(); it != allMEO.rend(); ++it) {
                sf::FloatRect bound;
                if ((*it)->type == nsReg::eLandBlock)
                    bound = (*it)->shapeForSelect.getGlobalBounds();

                if (bound.contains(window_.mapPixelToCoords(mousePos))) {
                    if (MovableEditObj::selectedObjs.find(*it) ==
                        MovableEditObj::selectedObjs.end()) {
                        std::unique_lock lk(MovableEditObj::mutForSelectedObjs);
                        MovableEditObj::selectedObjs.clear();
                        MovableEditObj::selectedObjs.insert(*it);
                        lk.unlock();
                    }
                    bMove = true;
                    break;
                }
            }
        // 如果没找到
        if (!bMove) {
            if (SelectRect::rect.bSetBegin) {
                SelectRect::rect.beginPoint = {
                    sf::Mouse::getPosition(window_).x,
                    sf::Mouse::getPosition(window_).y};
                SelectRect::rect.bDraw = true;
                SelectRect::rect.bSetBegin = false;
            }
            std::unique_lock lk(MovableEditObj::mutForSelectedObjs);
            MovableEditObj::selectedObjs.clear();
            lk.unlock();
        }

        else if (!SelectRect::bDraw) {
            for (MovableEditObj *meo : MovableEditObj::selectedObjs) {
                meo->setPosInWs(meo->getPosInWs() + deltaWorldMove);
            }
        }
    }

    if (event_.type == sf::Event::MouseButtonReleased) // 有问题，需要指定按键
    {
        for (MovableEditObj *meo : selectedObjs) {
            ((LandBlock *)meo)->normalizePos();
        }
        bMove = false;
        if (SelectRect::rect.bDraw) {
            for (MovableEditObj *meo : MovableEditObj::allMEO) {
                if (SelectRect::rect.bound.getGlobalBounds().contains(
                        meo->spr.getPosition()) &&
                    meo->type == nsReg::eLandBlock) {
                    MovableEditObj::selectedObjs.insert(meo);
                }
            }
        }
        SelectRect::rect.bDraw = false;
        SelectRect::rect.bSetBegin = true;
    }
}
void MovableEditObj::pollKey(sf::RenderWindow &window_, sf::Event &event_) {

    if (event_.type == sf::Event::MouseWheelScrolled) {
        float delta = event_.mouseWheelScroll.delta;
        static float pixSize0 = pixSize;
        if (delta > 0) {
            GCameraInterface::sceneScale += 0.1;
            if (GCameraInterface::sceneScale > 5)
                GCameraInterface::sceneScale = 5;
            pixSize = pixSize0 / GCameraInterface::sceneScale;
        }
        if (delta < 0) {
            GCameraInterface::sceneScale -= 0.1;
            if (GCameraInterface::sceneScale < 0.001)
                GCameraInterface::sceneScale = 0.001;
            pixSize = pixSize0 / GCameraInterface::sceneScale;
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        if (window_.hasFocus())
            WindowFlag::flag.posInWs = {
                winToWs(sf::Mouse::getPosition(window_)).x,
                winToWs(sf::Mouse::getPosition(window_)).y, 0};
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
        EditorCamera::editorCamera.posInWs -= deltaWorldMove;
    }
}
}; // namespace ens