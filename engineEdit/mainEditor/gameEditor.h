#if !defined(GAMEEDITOR)
#define GAMEEDITOR
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
class Info{
    public:
      struct Information {
          int fileID;
          int picIndex;
          FVector3 position;
      }info;
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
class MovableEditObj:public Info {

  public:
    struct Compare {
        bool operator()(const MovableEditObj *a,
                        const MovableEditObj *b) const {
            return a->posInWs.y < b->posInWs.y;
        }
    };

  public:
    static std::vector<MovableEditObj *> allMEO;
    static MovableEditObj *selectedObj;
    sf::Sprite spr;
    sf::Texture tex;
    sf::ConvexShape centerPoint;
    // IVector posInWin;
    FVector3 posInWs = {0, 0, 0};
    static IVector mousePos;
    static IVector deltaMove;
    static IVector posPre;
    MovableEditObj() {
        
        allMEO.push_back(this);
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
        window_.draw(spr);
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
            EditorCamera::editorCamera.posInWs += deltaWorldMove;
        }
        if (event_.type == sf::Event::MouseButtonReleased) {
            selectedObj = nullptr;
        }
    }
};
inline std::vector<MovableEditObj *> MovableEditObj::allMEO;
inline MovableEditObj *MovableEditObj::selectedObj;
inline IVector MovableEditObj::mousePos;
inline IVector MovableEditObj::deltaMove;
inline IVector MovableEditObj::posPre;
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class Editor {
  public:
    void loop() {
        MovableEditObj obja;
        obja.posInWs = {0, 0, 0};
        MovableEditObj objb;
        objb.posInWs = {0, 0, 0};
        sf::RenderWindow &window = getEditorWindow();
        sf::Event event;
        while (window.isOpen()) {
            if (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                MovableEditObj::pollKey(window, event);
            }
            window.clear();
            std::multiset<MovableEditObj *, MovableEditObj::Compare> allObj;
            for (auto obj : MovableEditObj::allMEO) {
                allObj.insert(obj);
            }
            for (auto obj : allObj) {
                obj->draw(window);
            }

            window.display();
        }
    }
};
}; // namespace ens
#endif // GAMEEDITOR
