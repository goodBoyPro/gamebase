#ifndef GOBJECT_H
#define GOBJECT_H
#include "GBase.h"
#include <GDebug.h>
#include <gridmap.h>
class GObject {

  public:
    GObject(/* args */) = default;
    virtual ~GObject() = default;
    // std::atomic<bool> isValid;
    bool isValid = true;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          GComponent
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GComponent : public GObject {
  public:
    virtual ~GComponent() {}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          GControllerInterface
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GControllerInterface : public GObject {
  private:
    static GControllerInterface *playerController;
    int id;

  protected:
    bool __focusOnWidget = false;

  public:
    void setFoucusOnWidget(bool bFocusOnUi) { __focusOnWidget = bFocusOnUi; }
    bool getIsFocusOnWidget() { return __focusOnWidget; }
    static std::vector<GControllerInterface *> &getAllController() {
        static std::vector<GControllerInterface *> allCtrl;
        return allCtrl;
    }
    void setActive() { getAllController()[id] = this; }
    void disableActive() { getAllController()[id] = nullptr; }

  public:
    GControllerInterface() {
        getAllController().push_back(this);
        id = getAllController().size() - 1;
        for (std::function<void()> &func : bindKey)
            func = []() { printf("No Key Bind\n"); };
        for (std::function<void(float)> &func : bindAxis)
            func = [](float) { printf("No Key Bind\n"); };
    }
    std::function<void()> bindKey[128];
    std::function<void(float)> bindAxis[128];
    virtual void pollKey(sf::RenderWindow &window_, sf::Event &event_) = 0;
    virtual ~GControllerInterface() { getAllController()[id] = nullptr; }

  public:
    static GControllerInterface *getPlayerController() {
        return playerController;
    }
    static void setPlayerController(GControllerInterface *ptr) {
        playerController = ptr;
    }
};
inline GControllerInterface *GControllerInterface::playerController = nullptr;
inline GControllerInterface *getPlayerController() {
    return GControllerInterface::getPlayerController();
}
inline void setPlayerController(GControllerInterface *ptr) {
    GControllerInterface::setPlayerController(ptr);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          cameraInterface
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GCameraInterface : public GObject {
  public:
    static float sceneScale;
    IVector positionInWin = IVector(WINW / 2, WINH / 2);
    FVector3 posInWs = FVector3(0, 0, 0);
    ~GCameraInterface() {};
};
inline float GCameraInterface::sceneScale = 1;
inline FVector2 wsToWin(const FVector3 &PositionInWS,
                        const FVector3 &cameraPos_) {
    return {((PositionInWS.x - cameraPos_.x) / pixSize + WINW / 2.f),
            ((PositionInWS.y - cameraPos_.y) / pixSize + WINH / 2.f -
             (PositionInWS.z / pixSize))};
}
inline FVector3 winToWs(const IVector &positionInWin,
                        const FVector3 &cameraPos_) {
    return {(positionInWin.x - WINW / 2) * pixSize + cameraPos_.x,
            (positionInWin.y - WINH / 2) * pixSize + cameraPos_.y, 0};
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           WorldInterface
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GWorldInterface : public GObject {
  public:
    GridMap<GActor *> *spaceManager = nullptr;
    GWorldInterface();
    virtual ~GWorldInterface() { delete spaceManager; }
    virtual void render(sf::RenderWindow &window_);
    void createSpaceManager() {
        delete spaceManager;
        spaceManager =
            new GridMap<GActor *>(FVector2(-10, -10), 500, 500, 5, 5);
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           gameInterface
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GGameInterface : public GObject {
  private:
    static GGameInterface *gameIns;
    GWorldInterface *worldActive = nullptr;

  public:
    sf::Event event;
    bool bGameContinue = 1;
    GGameInterface() { setGameIns(this); };
    virtual ~GGameInterface() {
        setGameIns(nullptr);
        delete worldActive;
        worldActive = nullptr;
    }
    virtual void renderLoop2D() = 0;
    static GGameInterface *getGameIns() { return gameIns; }
    static void setGameIns(GGameInterface *ptr) { gameIns = ptr; }
    GWorldInterface *getWorldActive() { return worldActive; }
    // 说明： createWorld(new worldclass);
    GWorldInterface *createWorld(GWorldInterface *newWorld);
    void setWorldActive(GWorldInterface *ptr) { worldActive = ptr; }
};
inline GGameInterface *GGameInterface::gameIns = nullptr;

inline GWorldInterface *GGameInterface::createWorld(GWorldInterface *newWorld) {

    return newWorld;
}
// 全局世界指针需要在最开始设置，而不能在创建完成后设置
inline GWorldInterface::GWorldInterface() {
    delete GGameInterface::getGameIns()->getWorldActive();
    GGameInterface::getGameIns()->setWorldActive(this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////ui///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class safeMultiset {};
class _______ui : public GObject {
  public:
    struct compare {
        const bool operator()(_______ui *a, _______ui *b) const {
            return a->layer < b->layer;
        }
    };

  private:
    std::multiset<_______ui *, compare> viewPortChild;
    std::set<_______ui *> allChildUi;
    _______ui *__parent = nullptr;
    FVector2 __size = {1, 1};
    FVector2 __positionRltv = {0, 0};
    FVector2 __positionABS = {0, 0};
    int layer = 0;

  public:
    bool isButton = false;
    void setLayer(int layer_) { layer = layer_; }
    _______ui(_______ui *parent_) {}
    _______ui() {}
    virtual ~_______ui() {
      for(_______ui*u:allChildUi){
          delete u;
      }
    }
    gameSprite spr;
    void setSize(const FVector2 &size_) { __size = size_; };
    void setPosition(const FVector2 &pos_) {
        __positionRltv = pos_;
        __positionABS = __parent->__positionABS + __positionRltv;
        spr.setPosition(__positionABS);
        for (_______ui *u : allChildUi) {
            u->setPosition(u->__positionRltv);
        }
    }
    void addToViewPort() { __parent->viewPortChild.insert(this); }
    void removeFromViewPort(){__parent->viewPortChild.erase(this);}
    virtual void draw(sf::RenderWindow &window_) {
        const FVector2 &sizeTemp = spr.getLocalBounds().getSize();
        FVector2 scale = {__size.x / sizeTemp.x, __size.y / sizeTemp.y};
        spr.setScale(scale);
        window_.draw(spr);
        for(_______ui*u:viewPortChild){
            window_.draw(u->spr);
        }
    }
    virtual void pollKey(){for(_______ui*u:viewPortChild){
            pollKey();
        }
    }
    template <class T> _______ui createUI() { _______ui*x= new T(this);
        allChildUi.insert(x);
        return x;
    }
    void destroy() { __parent->allChildUi.erase(this);
        __parent->viewPortChild.erase(this);
        delete this;
    }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////GUiInterface///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GButtonInterface : public _______ui {
  public:
    ~GButtonInterface() { isButton = true; }
};
class GUiInterface : public _______ui {
  private:
    std::vector<GButtonInterface *> ___btns;

  public:
    virtual ~GUiInterface() {}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////widgetInterface///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stack>
class GWidgetInterface : public GObject {
  private:
    static std::vector<GWidgetInterface *> __widgetForRender;
    static GWidgetInterface *___focused___;
    int layer = 0;
    bool visible = true;
    bool isInViewPort = false;
    std::multiset<GUiInterface *> __child;
    _______ui root;

  public:
    void setLayer(int layer_) { layer = layer_; }
    void setVisible(bool visible_) { visible = visible_; }
    void setPosition(const FVector2 pos_) { root.setPosition(pos_); }
    virtual void onEventAny(sf::RenderWindow &window_, sf::Event &event_) {
        root.pollKey();
    }
    virtual void onKeyPressed(sf::Keyboard::Key keyCode) {}
    virtual void onMousePressed(sf::Mouse::Button btnCode) {}
    virtual ~GWidgetInterface() {};
    GWidgetInterface() {}
    template<class T>
    _______ui* createUI() { root.createUI<T>(); }
    void addToViewPort() {
        if (isInViewPort)
            return;
        isInViewPort = true;
        __widgetForRender.push_back(this);
        ___focused___ = this;
    }
    GWidgetInterface *pop() {

        if (!isInViewPort)
            return nullptr;
        isInViewPort = false;
        GWidgetInterface *cur = *(__widgetForRender.end() - 1);
        __widgetForRender.pop_back();
        if (__widgetForRender.empty())
            ___focused___ = nullptr;
        else
            ___focused___ = *(__widgetForRender.end() - 1);
        return cur;
    }
    static GWidgetInterface *getTop() { return ___focused___; }
    static void drawAllWidget(sf::RenderWindow &window_);
    virtual void draw(sf::RenderWindow &window_) { root.draw(window_); };
};
inline void GWidgetInterface::drawAllWidget(sf::RenderWindow &window_) {
    for (GWidgetInterface *w : GWidgetInterface::__widgetForRender) {
        if (w->visible)
            w->draw(window_);
    }
}
inline GWidgetInterface *GWidgetInterface::___focused___ = nullptr;
inline std::vector<GWidgetInterface *> GWidgetInterface::__widgetForRender;
/////////////////////////////////////////////////////////////////////////////////////////////////////
/// GMouseInterface
/// ////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
class GMouseInterface {
  public:
    static GMouseInterface *mousePtr;
    GMouseInterface() { mousePtr = this; }
    virtual ~GMouseInterface() {}
    virtual void drawMouseCusor(sf::RenderWindow &window_) {};
};
inline GMouseInterface *GMouseInterface::mousePtr = nullptr;
/////////////////////////////////////////////////////////////////////////////////////////////////////

#endif