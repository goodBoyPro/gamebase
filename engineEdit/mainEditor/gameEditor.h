#if !defined(GAMEEDITOR)
#define GAMEEDITOR
#include "editorCommand.h"
#include "editorSave.h"
#include "editorServer.h"
#include <GDebug.h>
#include <GObject.h>
#include <GSource.h>
#include <SFML/Graphics.hpp>
#include <set>
#include <vector>
#define ACTORMODE 0
#define LANDMODE 1
#define GAMEMODE 2
namespace ens {
inline int editMode = 2;
// inline sf::RenderWindow &getEditorWindow() {
//     static sf::RenderWindow window(sf::VideoMode(WINW, WINH), "editor");
//     return window;
// }
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class Info {
  public:
    struct Information {
        int fileID;
        int picIndex;
    } info;
};
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class EditorCamera : public GICamera {
  public:
    EditorCamera() {}
    static EditorCamera editorCamera;
    static void pollKey(sf::Event &event) {}
};
inline EditorCamera EditorCamera::editorCamera;
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class WindowFlag {
  public:
    static WindowFlag flag;
    gameSprite spr;
    sf::Texture tex;
    sf::ConvexShape shape;
    FVector3 posInWs = {0, 0, 0};
    WindowFlag() {
        shape.setPointCount(3);
        shape.setPoint(0, {0, 0});
        shape.setPoint(1, {4, 4});
        shape.setPoint(2, {-4, 4});
        shape.setFillColor(sf::Color(0, 255, 0, 255));
    }
    void draw(sf::RenderWindow &window_, const FVector3 cameraPos_) {
        FVector2 piwin = wsToWin(flag.posInWs, cameraPos_);
        flag.shape.setPosition(piwin.x, piwin.y);
        window_.draw(flag.shape);
    };
};
inline WindowFlag WindowFlag::flag;
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class SelectRect {
  public:
    static bool bDraw;
    // 确保只在第一次点击时设置
    bool bSetBegin = true;
    static SelectRect rect;
    sf::ConvexShape bound;
    FVector2 beginPoint;
    SelectRect() {
        bound.setPointCount(4);
        bound.setPoint(0, {0, 0});
        bound.setPoint(1, {1, 0});
        bound.setPoint(2, {1, 1});
        bound.setPoint(3, {0, 1});
        bound.setFillColor(sf::Color(0, 0, 0, 0));
        bound.setOutlineColor(sf::Color(255, 0, 0, 255));
        bound.setOutlineThickness(3);
    }
    void draw(sf::RenderWindow &window_) {
        if (bDraw) {
            bound.setPosition(beginPoint);
            FVector2 size = {sf::Mouse::getPosition(window_).x - beginPoint.x,
                             sf::Mouse::getPosition(window_).y - beginPoint.y};
            bound.setOutlineThickness(3 / size.x);
            bound.setScale(size);
            window_.draw(bound);
        }
    }
};
inline bool SelectRect::bDraw = false;
inline SelectRect SelectRect::rect;
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
inline std::mutex mutForAllMeo;
class MovableEditObj : public Info {

  public:
    // enum EType { eactor, elandBlock };
    nsReg::ActorType type = nsReg::eActorStatic;
    struct Compare {
        bool operator()(const MovableEditObj *a,
                        const MovableEditObj *b) const {
            return a->posInWs.y < b->posInWs.y;
        }
    };

  private:
    FVector3 posInWs = {0, 0, 0};
    FVector3 sizeInWs = {1, 1, 1};

  public:
    static std::multiset<MovableEditObj *> allMEO;
    static void clearAllMeo() {
        std::unique_lock lk(mutForAllMeo);
        allMEO.clear();
    }
    static void allMeoInsert(MovableEditObj *ptr) {
        std::unique_lock lk(mutForAllMeo);
        allMEO.insert(ptr);
    };
    static void allMeoRemove(MovableEditObj *ptr) {
        std::unique_lock lk(mutForAllMeo);
        allMEO.erase(ptr);
    };
    static void allMeoPoll(std::function<void(MovableEditObj *)> cbk) {
        std::unique_lock lk(mutForAllMeo);
        for (MovableEditObj *meo : allMEO) {
            cbk(meo);
        }
    }

    static std::set<MovableEditObj *> selectedObjs;
    static std::mutex mutForSelectedObjs;
    sf::Sprite spr;
    sf::Texture tex;
    sf::ConvexShape centerPoint;
    sf::ConvexShape shapeBound;
    sf::ConvexShape shapeForSelect;

    const FVector3 getPosInWs() const { return posInWs; }
    virtual void setPosInWs(const FVector3 &pos_) { posInWs = pos_; }
    FVector2 psInWin;
    bool isValid = true;

    void destroy() { isValid = false; }
    static void updateMouseState(sf::RenderWindow &window_) {
        mousePos = sf::Mouse::getPosition(window_);
        deltaMove = mousePos - posPre;
        posPre = sf::Mouse::getPosition(window_);
        deltaWorldMove = {deltaMove.x * pixSize, deltaMove.y * pixSize, 0};
    };
    static IVector mousePos;
    static IVector deltaMove;
    static FVector3 deltaWorldMove;
    static IVector posPre;
    void setSprite(int fileId_, int picIndex_) {
        getSource().setSprite(spr, fileId_, picIndex_);
        info.fileID = fileId_;
        info.picIndex = picIndex_;
    }
    const FVector3 &getSizeInWs() const { return sizeInWs; }
    void setSizeWS(const FVector3 &size_) { sizeInWs = size_; }
    MovableEditObj(int fileId_, int picIndex_) {
        allMeoInsert(this);
        centerPoint.setPointCount(3);
        centerPoint.setPoint(0, {0, 0});
        centerPoint.setPoint(1, {4, 4});
        centerPoint.setPoint(2, {-4, 4});
        centerPoint.setFillColor(sf::Color(255, 0, 0));
        setSprite(fileId_, picIndex_);
        shapeBound.setPointCount(4);
        sf::FloatRect lb = spr.getLocalBounds();
        shapeBound.setPoint(0, {lb.left, lb.top});
        shapeBound.setPoint(1, {lb.left + lb.getSize().x, lb.top});
        shapeBound.setPoint(
            2, {lb.left + lb.getSize().x, lb.top + lb.getSize().y});
        shapeBound.setPoint(3, {lb.left, lb.top + lb.getSize().y});
        shapeBound.setOrigin(spr.getOrigin());
        shapeBound.setFillColor(sf::Color(0, 0, 0, 0));
        shapeBound.setOutlineColor(sf::Color(255, 255, 0));
        shapeBound.setOutlineThickness(3);
        float sizeShapeForSelect = 14;
        shapeForSelect.setPointCount(4);
        shapeForSelect.setPoint(0, {0, 0});
        shapeForSelect.setPoint(1, {sizeShapeForSelect, 0});
        shapeForSelect.setPoint(2, {sizeShapeForSelect, sizeShapeForSelect});
        shapeForSelect.setPoint(3, {0, sizeShapeForSelect});
        shapeForSelect.setOrigin(7, 7);
        shapeForSelect.setFillColor(sf::Color(0, 0, 0, 0));
        shapeForSelect.setOutlineColor(sf::Color(255, 255, 0));
        shapeForSelect.setOutlineThickness(3);
    }
    virtual void draw(sf::RenderWindow &window_, const FVector3 &cameraPos_) {
        psInWin = wsToWin(posInWs, cameraPos_);
        spr.setPosition(psInWin.x, psInWin.y);
        float xS = sizeInWs.x / pixSize / spr.getLocalBounds().getSize().x;
        float yS = sizeInWs.y / pixSize / spr.getLocalBounds().getSize().y;
        spr.setScale(xS, yS);
        window_.draw(spr);
        centerPoint.setPosition(psInWin.x, psInWin.y);
        window_.draw(centerPoint);
    }
    static void drawBounds(sf::RenderWindow &window_) {
        std::unique_lock lk(MovableEditObj::mutForSelectedObjs);
        if (!selectedObjs.empty())
            for (auto s : selectedObjs) {
                s->shapeBound.setPosition(s->psInWin.x, s->psInWin.y);
                float xS = s->getSizeInWs().x / pixSize /
                           s->spr.getLocalBounds().getSize().x;
                float yS = s->getSizeInWs().y / pixSize /
                           s->spr.getLocalBounds().getSize().y;
                s->shapeBound.setOutlineThickness(400 * pixSize);
                s->shapeBound.setScale(xS, yS);
                window_.draw(s->shapeBound);
            }
    }
    void drawHandler(sf::RenderWindow &window_) {
        shapeForSelect.setPosition(psInWin.x, psInWin.y);
        window_.draw(shapeForSelect);
    }

    static void pollKeyActorMdoe(sf::RenderWindow &window_, sf::Event &event_);
    static void pollKeyLandMdoe(sf::RenderWindow &window_, sf::Event &event_);
    static void pollKey(sf::RenderWindow &window_, sf::Event &event_,const FVector3 &cameraPos_);
    void returnMessage() {
        const std::string &msg = nsg::combineStrings(
            {"selected ", std::to_string(sizeInWs.x), " ",
             std::to_string(sizeInWs.y), " ", std::to_string(sizeInWs.z), " ",
             std::to_string(info.fileID), " ", std::to_string(info.picIndex),
             " ", std::to_string(0)});
        EditorServer::server.sendMesssage(msg);
    }
};

inline std::multiset<MovableEditObj *> MovableEditObj::allMEO;
inline std::mutex MovableEditObj::mutForSelectedObjs;
inline std::set<MovableEditObj *> MovableEditObj::selectedObjs;
inline IVector MovableEditObj::mousePos;
inline IVector MovableEditObj::deltaMove;
inline FVector3 MovableEditObj::deltaWorldMove;
inline IVector MovableEditObj::posPre;
template <class T> inline T *createAtLocation(T *ptr, const FVector3 &pos_) {
    ptr->setPosInWs(pos_);
    return ptr;
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class Actor : public MovableEditObj {
  public:
    Actor(int fileId_, int picIndex_) : MovableEditObj(fileId_, picIndex_) {
        type = nsReg::eActorStatic;
    }
    virtual void draw(sf::RenderWindow &window_, const FVector3 &cameraPos_) {
        MovableEditObj::draw(window_, cameraPos_);
    }
};
class LandBlock : public MovableEditObj {
  public:
    static FVector3 beginPoint;
    static float blockSize;
    static int rows;
    static int columns;
    static void setLandOptions(const FVector3 &bp, float blocksize_, int rows_,
                               int columns_) {
        beginPoint = bp;
        blockSize = blocksize_;
        rows = rows_;
        columns = columns_;
    };
    static void clearLandBlocks() {
        std::unique_lock lk(mutForAllMeo);
        for (auto it = MovableEditObj::allMEO.begin();
             it != MovableEditObj::allMEO.end();) {

            if ((*it)->type == nsReg::eLandBlock) {
                it = MovableEditObj::allMEO.erase(it);
            } else
                it++;
        }
    }
    static void spawnRandomLandblocks() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                int fileId_ = 3;
                int picindex_ = rand() % 9;
                FVector3 pos = {beginPoint.x + blockSize * j,
                                beginPoint.y + blockSize * i, 0};
                createAtLocation(new LandBlock(fileId_, picindex_), pos)
                    ->setSizeWS({blockSize, blockSize, 0});
            }
        }
    }
    virtual void setPosInWs(const FVector3 &pos_) override {
        MovableEditObj::setPosInWs(pos_);
    }
    void normalizePos() {
        int r = (getPosInWs().y - beginPoint.y) / blockSize;
        int c = (getPosInWs().x - beginPoint.x) / blockSize;
        MovableEditObj::setPosInWs(FVector3{c * blockSize, r * blockSize, 0} +
                                   beginPoint);
    };
    LandBlock(int fileId_, int picIndex_) : MovableEditObj(fileId_, picIndex_) {
        type = nsReg::eLandBlock;
        float r = blockSize / pixSize / spr.getLocalBounds().getSize().x;
        spr.setScale(r, r);
        shapeBound.setScale(r, r);
    }
    virtual void draw(sf::RenderWindow &window_, const FVector3 &cameraPos_) {
        MovableEditObj::draw(window_, cameraPos_);
    }
};
inline FVector3 LandBlock::beginPoint = {-100, -100, 0};
inline float LandBlock::blockSize = 3;
inline int LandBlock::rows = 50;
inline int LandBlock::columns = 50;
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
class Editor {
  public:
    sf::RenderWindow *windowEditor;
    sf::Event event;
    void setCommand();
    static bool isMouseInWindow(sf::RenderWindow &window_);
    Editor() {
        initTools();

        windowEditor =
            new sf::RenderWindow(sf::VideoMode(WINW, WINH), "editor");
        setCommand();
        // std::thread tInput(&editorCommand::loop, &editorCommand::edc);
        loop();
        // tInput.join();
    }
    ~Editor() { delete windowEditor; }
    static void openPad() {}
    void loop() {
        sf::RenderWindow &window = *windowEditor;
        window.setPosition(IVector(100, 100));
        auto it = MovableEditObj::allMEO.begin();
        std::multiset<MovableEditObj *, MovableEditObj::Compare> allObj;
        while (window.isOpen()) {
            FVector3 cameraPosForRender = EditorCamera::editorCamera.posInWs;
            MovableEditObj::updateMouseState(window);
            if (window.pollEvent(event) && window.hasFocus()) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.code) {
                    case sf::Keyboard::K:
                        editorSave::saveTofile("test");
                        break;
                    case sf::Keyboard::Tab:
                        Editor::openPad();
                        break;
                    default:
                        break;
                    }
                }
                MovableEditObj::pollKey(window, event,cameraPosForRender);
                if (editMode == ACTORMODE) {
                    MovableEditObj::pollKeyActorMdoe(window, event);
                } else if (editMode == LANDMODE) {
                    MovableEditObj::pollKeyLandMdoe(window, event);
                }
            }
            allObj.clear();
            std::unique_lock lk(mutForAllMeo);
            for (it = MovableEditObj::allMEO.begin();
                 it != MovableEditObj::allMEO.end();) {
                if ((*it)->isValid) {
                    allObj.insert(*it);
                    it++;
                } else {
                    delete (*it);
                    it = MovableEditObj::allMEO.erase(it);
                }
            }
            lk.unlock();
            ///////////////////////////////////////////////////////////////////////////
            // 渲染///////////////////////////////////////////////////////////////////////////
            window.clear();
            const wchar_t *showmode[3] = {L"游戏模式", L"ACTOR模式",
                                          L"LAND模式"};
            const wchar_t *temp;
            switch (editMode) {
            case GAMEMODE:
                temp = showmode[0];
                break;
            case ACTORMODE:
                temp = showmode[1];
                break;
            case LANDMODE:
                temp = showmode[2];
                break;
            default:
                break;
            }
            PRINTDEBUG(L"%S", temp);
            PRINTDEBUG(L"键鼠位置：%f,%f",
                       winToWs(sf::Mouse::getPosition(window),cameraPosForRender).x,
                       winToWs(sf::Mouse::getPosition(window),cameraPosForRender).y);
            PRINTDEBUG(L"对象总数:%d", MovableEditObj::allMEO.size());
            for (auto obj : allObj) {
                obj->draw(window, cameraPosForRender);
            }

            for (auto obj : allObj) {
                if (editMode == LANDMODE && obj->type == nsReg::eLandBlock) {
                    obj->drawHandler(window);
                } else if (editMode == ACTORMODE &&
                           obj->type == nsReg::eActorStatic) {
                    obj->drawHandler(window);
                }
            }

            MovableEditObj::drawBounds(window);
            SelectRect::rect.draw(window);
            WindowFlag::flag.draw(window, cameraPosForRender);
            // 显示DEBUG////////////////////////////////
            GDebug::debugDisplay(window);
            window.display();
        }
        editorCommand::edc.bRun = false;
        for (auto obj : MovableEditObj::allMEO) {
            delete obj;
        }
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

}; // namespace ens
#endif // GAMEEDITOR
       //////////////////////////////////////////////////////////////////////////////////
       //////////////////////////////////////////////////////////////////////////////////
       //////////////////////////////////////////////////////////////////////////////////
       // cr fileid picindex
       // crm fileid picindex
       // crl fileid picIndex times
       // settex fileid picindex