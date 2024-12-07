#if !defined(GAMEEDITOR)
#define GAMEEDITOR
#include <GDebug.h>
#include <GObject.h>
#include <SFML/Graphics.hpp>
#include <set>
#include <vector>
namespace ens {
inline sf::RenderWindow &getEditorWindow() {
    static sf::RenderWindow window(sf::VideoMode(WINW, WINH), "editor");
    return window;
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class Info {
  public:
    struct Information {
        int fileID;
        int picIndex;
        FVector3 position;
    } info;
};
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class EditorCamera : public GCameraInterface {
  public:
    EditorCamera() { GCameraInterface::setGameCamera(this); }
    static EditorCamera editorCamera;
    static void pollKey(sf::Event &event) {}
};
inline EditorCamera EditorCamera::editorCamera;
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class MovableEditObj : public Info {

  public:
    struct Compare {
        bool operator()(const MovableEditObj *a,
                        const MovableEditObj *b) const {
            return a->posInWs.y < b->posInWs.y;
        }
    };

  public:
    static std::multiset<MovableEditObj *> allMEO;
    static MovableEditObj *selectedObj;
    sf::Sprite spr;
    sf::Texture tex;
    sf::ConvexShape centerPoint;
    // IVector posInWin;
    FVector3 posInWs = {0, 0, 0};
    bool isValid = true;
    void destroy() { isValid = false; }
    static IVector mousePos;
    static IVector deltaMove;
    static IVector posPre;
    MovableEditObj() {

        allMEO.insert(this);
        centerPoint.setPointCount(3);
        centerPoint.setPoint(0, {0, 0});
        centerPoint.setPoint(1, {4, 4});
        centerPoint.setPoint(2, {-4, 4});
        centerPoint.setFillColor(sf::Color(255, 0, 0));
        tex.loadFromFile("res/a.png");
        spr.setTexture(tex);
        spr.setPosition(10, 10);
    }
    void draw(sf::RenderWindow &window_) {
        IVector psInWin = wsToWin(posInWs);
        spr.setPosition(psInWin.x, psInWin.y);
        window_.draw(spr);
        centerPoint.setPosition(psInWin.x, psInWin.y);
        window_.draw(centerPoint);
    }

    static void pollKey(sf::RenderWindow &window_, sf::Event &event_) {
        mousePos = sf::Mouse::getPosition(window_);
        deltaMove = mousePos - posPre;
        FVector3 deltaWorldMove = {deltaMove.x * pixSize, deltaMove.y * pixSize,
                                   0};
        posPre = sf::Mouse::getPosition(window_);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!selectedObj) {
                for (auto ob : allMEO) {
                    sf::FloatRect bound = ob->spr.getGlobalBounds();
                    if (bound.contains(window_.mapPixelToCoords(mousePos))) {
                        selectedObj = ob;
                        break;
                    }
                }
            } else {
                selectedObj->posInWs += deltaWorldMove;
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
            EditorCamera::editorCamera.posInWs -= deltaWorldMove;
        }
        if (event_.type == sf::Event::MouseButtonReleased) {
            selectedObj = nullptr;
        }
    }
};
inline std::multiset<MovableEditObj *> MovableEditObj::allMEO;
inline MovableEditObj *MovableEditObj::selectedObj;
inline IVector MovableEditObj::mousePos;
inline IVector MovableEditObj::deltaMove;
inline IVector MovableEditObj::posPre;
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class Editor {
  public:
    Editor() { initTools(); }
    void loop() {
        for (int i = 0; i < 100; i++) {
            MovableEditObj *x = new MovableEditObj;
            x->posInWs = {0, 0, 0};
        }
        sf::RenderWindow &window = getEditorWindow();
        setWindow(&window);
        sf::Event event;
        auto it = MovableEditObj::allMEO.begin();
        while (window.isOpen()) {
            if (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                MovableEditObj::pollKey(window, event);
            }
            std::multiset<MovableEditObj *, MovableEditObj::Compare> allObj;
            for (it = MovableEditObj::allMEO.begin();
                 it != MovableEditObj::allMEO.end();) {
                if ((*it)->isValid) {
                    allObj.insert(*it);
                    it++;
                } else {
                    it = MovableEditObj::allMEO.erase(it);
                }
            }
            ///////////////////////////////////////////////////////////////////////////
            // 渲染///////////////////////////////////////////////////////////////////////////
            window.clear();
            PRINTDEBUG(L"键鼠位置：%f,%f",
                       winToWs(sf::Mouse::getPosition(*getWindow())).x,
                       winToWs(sf::Mouse::getPosition(*getWindow())).y);
            for (auto obj : allObj) {
                obj->draw(window);
            }
            // 显示DEBUG////////////////////////////////
            GDebug::debugDisplay();
            window.display();
        }
        for (auto obj : MovableEditObj::allMEO) {
            delete obj;
        }
    }
};
}; // namespace ens
#endif // GAMEEDITOR
