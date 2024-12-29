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

  public:
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

  public:
    void setLayer(int layer_) { layer = layer_; }
    void setVisible(bool visible_) { visible = visible_; }
    virtual void onEventAny(sf::RenderWindow &window_){}
    virtual void onKeyPressed(sf::Keyboard::Key keyCode) {}
    virtual void onMousePressed(sf::Mouse::Button btnCode) {}
    virtual ~GWidgetInterface() {};
    GWidgetInterface() {}
    void addToViewPort() {
      if(isInViewPort)
          return;
        isInViewPort = true;
        __widgetForRender.push_back(this);
        ___focused___ = this;
    }
    void pop() {
      if(!isInViewPort)
          return;
        isInViewPort = false;
        __widgetForRender.pop_back();
        if (__widgetForRender.empty())
            ___focused___ = nullptr;
        else
            ___focused___ = *(__widgetForRender.end() - 1);
    }
    static GWidgetInterface *getTop() { return ___focused___; }
    static void drawAllWidget(sf::RenderWindow &window_);
    virtual void draw(sf::RenderWindow &window_) {};
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
    virtual~GMouseInterface(){}
    virtual void drawMouseCusor(sf::RenderWindow &window_) {};
};
inline GMouseInterface *GMouseInterface::mousePtr = nullptr;
/////////////////////////////////////////////////////////////////////////////////////////////////////

#endif