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
    void draw() {
        IVector piwin = wsToWin(flag.posInWs);
        flag.shape.setPosition(piwin.x, piwin.y);
        getWindow()->draw(flag.shape);
    };
};
inline WindowFlag WindowFlag::flag;
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
inline std::mutex mutForAllMeo;
class MovableEditObj : public Info {

  public:
    enum EType { eactor, elandBlock };
    EType type = eactor;
    struct Compare {
        bool operator()(const MovableEditObj *a,
                        const MovableEditObj *b) const {
            return a->posInWs.y < b->posInWs.y;
        }
    };

  private:
    FVector3 posInWs = {0, 0, 0};

  public:
    static std::multiset<MovableEditObj *> allMEO;
    void allMeoInsert(MovableEditObj *ptr) {
        std::unique_lock lk(mutForAllMeo);
        allMEO.insert(ptr);
    };
    void allMeoRemove(MovableEditObj *ptr) {
        std::unique_lock lk(mutForAllMeo);
        allMEO.erase(ptr);
    };
    static MovableEditObj *selectedObjForMove;
    static MovableEditObj *selectedObjForEdit;

    sf::Sprite spr;
    sf::Texture tex;
    sf::ConvexShape centerPoint;
    sf::ConvexShape shapeBound;
    sf::ConvexShape shapeForSelect;

    const FVector3 getPosInWs() const { return posInWs; }
    virtual void setPosInWs(const FVector3 &pos_) {
        posInWs = pos_;
        info.position = pos_;
    }
    IVector psInWin;
    bool isValid = true;

    void destroy() { isValid = false; }
    static IVector mousePos;
    static IVector deltaMove;
    static IVector posPre;
    void setSprFileIdPicIndex(int fileId_, int picIndex_) {
        getSource().setSprite(spr, fileId_, picIndex_);
        info.fileID = fileId_;
        info.picIndex = picIndex_;
    }
    MovableEditObj(int fileId_, int picIndex_) {
        allMeoInsert(this);
        centerPoint.setPointCount(3);
        centerPoint.setPoint(0, {0, 0});
        centerPoint.setPoint(1, {4, 4});
        centerPoint.setPoint(2, {-4, 4});
        centerPoint.setFillColor(sf::Color(255, 0, 0));
        setSprFileIdPicIndex(fileId_, picIndex_);
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
    virtual void draw(sf::RenderWindow &window_) {
        psInWin = wsToWin(posInWs);
        spr.setPosition(psInWin.x, psInWin.y);
        FVector2 sc = spr.getScale();
        spr.scale(GCameraInterface::sceneScale, GCameraInterface::sceneScale);
        window_.draw(spr);
        spr.setScale(sc);
        centerPoint.setPosition(psInWin.x, psInWin.y);
        window_.draw(centerPoint);
    }
    static void drawBound(sf::RenderWindow &window_) {
        if (selectedObjForEdit) {
            selectedObjForEdit->shapeBound.setPosition(
                selectedObjForEdit->psInWin.x, selectedObjForEdit->psInWin.y);
            FVector2 sc = selectedObjForEdit->shapeBound.getScale();
            selectedObjForEdit->shapeBound.scale(GCameraInterface::sceneScale,
                                                 GCameraInterface::sceneScale);
            window_.draw(selectedObjForEdit->shapeBound);
            selectedObjForEdit->shapeBound.setScale(sc);
        }
    }
    void drawHandler(sf::RenderWindow &window_) {
        shapeForSelect.setPosition(psInWin.x, psInWin.y);
        window_.draw(shapeForSelect);
    }

    static void pollKey(sf::RenderWindow &window_, sf::Event &event_);
};

inline std::multiset<MovableEditObj *> MovableEditObj::allMEO;
inline MovableEditObj *MovableEditObj::selectedObjForMove;
inline MovableEditObj *MovableEditObj::selectedObjForEdit;
inline IVector MovableEditObj::mousePos;
inline IVector MovableEditObj::deltaMove;
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
        type = eactor;
    }
    virtual void draw(sf::RenderWindow &window_) {
        MovableEditObj::draw(window_);
    }
};
class LandBlock : public MovableEditObj {
  public:
    static FVector3 beginPoint;
    static float blockSize;
    static int rows;
    static int columns;
    static void createLand(const FVector3 &bp, float blocksize_, int rows_,
                           int columns_) {
        beginPoint = bp;
        blockSize = blocksize_;
        rows = rows_;
        columns = columns_;
    };
    virtual void setPosInWs(const FVector3 &pos_) override {
        MovableEditObj::setPosInWs(pos_);
        normalizePos();
    }
    void normalizePos() {
        int r = (getPosInWs().y - beginPoint.y) / blockSize;
        int c = (getPosInWs().x - beginPoint.x) / blockSize;
        MovableEditObj::setPosInWs(FVector3{c * blockSize, r * blockSize, 0} +
                                   beginPoint);
    };
    LandBlock(int fileId_, int picIndex_) : MovableEditObj(fileId_, picIndex_) {
        type = elandBlock;
        float r = blockSize / pixSize / spr.getLocalBounds().getSize().x;
        spr.setScale(r, r);
        shapeBound.setScale(r, r);
    }
    virtual void draw(sf::RenderWindow &window_) {
        MovableEditObj::draw(window_);
        // if (editMode == LANDMODE)
        //     drawBound(window_);
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
    sf::RenderWindow *window;
    sf::Event event;
    void setCommand();
    Editor() {
        initTools();

        window = &getEditorWindow();
        setWindow(window);
        setCommand();
        // std::thread tInput(&editorCommand::loop, &editorCommand::edc);
        loop();
        // tInput.join();
    }
    void loop() {
        sf::RenderWindow &window = getEditorWindow();
        window.setPosition(IVector(100, 100));
        auto it = MovableEditObj::allMEO.begin();
        while (window.isOpen()) {
            if (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::KeyPressed) {
                    switch (event.key.code) {
                    case sf::Keyboard::Num1:
                        editMode = GAMEMODE;
                        break;
                    case sf::Keyboard::Num2:
                        editMode = ACTORMODE;
                        break;
                    case sf::Keyboard::Num3:
                        editMode = LANDMODE;
                        break;
                    case sf::Keyboard::K:
                        editorSave::saveTofile("test");
                        break;
                    default:
                        break;
                    }
                }

                MovableEditObj::pollKey(window, event);
            }
            std::multiset<MovableEditObj *, MovableEditObj::Compare> allObj;
            std::unique_lock lk(mutForAllMeo);
            for (it = MovableEditObj::allMEO.begin();
                 it != MovableEditObj::allMEO.end();) {
                if ((*it)->isValid) {
                    allObj.insert(*it);
                    it++;
                } else {
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
                       winToWs(sf::Mouse::getPosition(*getWindow())).x,
                       winToWs(sf::Mouse::getPosition(*getWindow())).y);
            GCameraInterface::posForDraw =
                GCameraInterface::getGameCamera()->posInWs;
            for (auto obj : allObj) {
                obj->draw(window);
            }

            for (auto obj : allObj) {
                if (editMode == LANDMODE &&
                    obj->type == MovableEditObj::elandBlock) {
                    obj->drawHandler(window);
                } else if (editMode == ACTORMODE &&
                           obj->type == MovableEditObj::eactor) {
                    obj->drawHandler(window);
                }
            }
            MovableEditObj::drawBound(window);
            WindowFlag::flag.draw();
            // 显示DEBUG////////////////////////////////
            GDebug::debugDisplay();
            window.display();
        }
        editorCommand::edc.bRun = false;
        for (auto obj : MovableEditObj::allMEO) {
            delete obj;
        }
    }
};
inline void Editor::setCommand() {
    editorCommand::edc.command["setmode"] = []() {
        editMode = std::stoi(editorCommand::edc.input[1]);
    };
    editorCommand::edc.command["cr"] = [&]() {
        MovableEditObj *obj = nullptr;
        if (editMode == ACTORMODE)
            obj = createAtLocation(
                new Actor(std::stoi(editorCommand::edc.input[1]),
                          std::stoi(editorCommand::edc.input[2])),
                WindowFlag::flag.posInWs);
        else if (editMode == LANDMODE)
            obj = createAtLocation(
                new LandBlock(std::stoi(editorCommand::edc.input[1]),
                              std::stoi(editorCommand::edc.input[2])),
                WindowFlag::flag.posInWs);
        MovableEditObj::selectedObjForEdit = obj;
    };
    editorCommand::edc.command["crm"] = [&]() {
        MovableEditObj *obj = nullptr;
        if (editMode == ACTORMODE)
            obj = createAtLocation(
                new Actor(std::stoi(editorCommand::edc.input[1]),
                          std::stoi(editorCommand::edc.input[2])),
                {winToWs(sf::Mouse::getPosition(*getWindow())).x,
                 winToWs(sf::Mouse::getPosition(*getWindow())).y, 0});
        else if (editMode == LANDMODE)
            obj = createAtLocation(
                new LandBlock(std::stoi(editorCommand::edc.input[1]),
                              std::stoi(editorCommand::edc.input[2])),
                {winToWs(sf::Mouse::getPosition(*getWindow())).x,
                 winToWs(sf::Mouse::getPosition(*getWindow())).y, 0});
        MovableEditObj::selectedObjForEdit = obj;
    };
    editorCommand::edc.command["create"] = [&]() {
        int fileid = std::stoi(editorCommand::edc.input[1]);
        if (!getSource().checkTexArrayValid(fileid)) {
            EditorServer::server.sendMesssage("server:no such fileId");
            return;
        }
        if (getSource().checkTexArrayType(fileid) != textureArray::actor &&
            editMode == ACTORMODE) {
            EditorServer::server.sendMesssage("server:not actor");
            return;
        }
        if (getSource().checkTexArrayType(fileid) != textureArray::landblock &&
            editMode == LANDMODE) {
            EditorServer::server.sendMesssage("server:not landblock");
            return;
        }
        if (editMode == ACTORMODE)
            for (int i = 0; i < std::stoi(editorCommand::edc.input[3]); i++) {
                MovableEditObj *obj = createAtLocation(
                    new Actor(fileid, std::stoi(editorCommand::edc.input[2])),
                    WindowFlag::flag.posInWs);
                MovableEditObj::selectedObjForEdit = obj;
            }
        if (editMode == LANDMODE)
            for (int i = 0; i < std::stoi(editorCommand::edc.input[3]); i++) {
                MovableEditObj *obj = createAtLocation(
                    new LandBlock(fileid,
                                  std::stoi(editorCommand::edc.input[2])),
                    WindowFlag::flag.posInWs);
                MovableEditObj::selectedObjForEdit = obj;
            }
    };
};

inline void MovableEditObj::pollKey(sf::RenderWindow &window_,
                                    sf::Event &event_) {
    mousePos = sf::Mouse::getPosition(window_);
    deltaMove = mousePos - posPre;
    FVector3 deltaWorldMove = {deltaMove.x * pixSize, deltaMove.y * pixSize, 0};
    posPre = sf::Mouse::getPosition(window_);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!selectedObjForMove) {
            selectedObjForEdit = nullptr;
            for (auto it = allMEO.rbegin(); it != allMEO.rend(); ++it) {
                sf::FloatRect bound;
                if (editMode == ACTORMODE &&
                    (*it)->type == MovableEditObj::eactor)
                    bound = (*it)->shapeForSelect.getGlobalBounds();
                else if (editMode == LANDMODE &&
                         (*it)->type == MovableEditObj::elandBlock)
                    bound = (*it)->shapeForSelect.getGlobalBounds();
                if (bound.contains(window_.mapPixelToCoords(mousePos))) {
                    selectedObjForMove = (*it);
                    selectedObjForEdit = (*it);
                    break;
                }
            }
        } else {
            // selectedObjForMove->posInWs += deltaWorldMove;
            selectedObjForMove->setPosInWs(selectedObjForMove->getPosInWs()+deltaWorldMove);
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        if (window_.hasFocus())
            WindowFlag::flag.posInWs = {
                winToWs(sf::Mouse::getPosition(*getWindow())).x,
                winToWs(sf::Mouse::getPosition(*getWindow())).y, 0};
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
        EditorCamera::editorCamera.posInWs -= deltaWorldMove;
    }
    if (event_.type == sf::Event::MouseButtonReleased) {
        if (selectedObjForMove && editMode == LANDMODE &&
            selectedObjForMove->type == MovableEditObj::elandBlock)
            ((LandBlock *)selectedObjForMove)->normalizePos();
        selectedObjForMove = nullptr;
    }
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
}
}; // namespace ens
#endif // GAMEEDITOR
       //////////////////////////////////////////////////////////////////////////////////
       //////////////////////////////////////////////////////////////////////////////////
       //////////////////////////////////////////////////////////////////////////////////
       // cr fileid picindex
       // crm fileid picindex
       // crl fileid picIndex times
       // settex fileid picindex