#ifndef GOBJECT_H
#define GOBJECT_H
#include "GBase.h"
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
  private:
    static GCameraInterface *gameCameraX;

  public:
    static float sceneScale;
    IVector positionInWin = IVector(WINW / 2, WINH / 2);
    FVector3 posInWs = FVector3(0, 0, 0);
    ~GCameraInterface() {};
    // static GCameraInterface *getGameCamera() { return gameCameraX; }
    // static void setGameCamera(GCameraInterface *ptr) { gameCameraX = ptr; }
};
inline float GCameraInterface::sceneScale = 1;
inline GCameraInterface *GCameraInterface::gameCameraX = nullptr;
// inline void setGameCamera(class GCameraInterface *camera_) {
//     GCameraInterface::setGameCamera(camera_);
// }
// inline GCameraInterface *getGameCamera() {
//     return GCameraInterface::getGameCamera();
// }
inline IVector wsToWin(const FVector3 &PositionInWS,
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
    virtual void render();
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
class WidgetInterface : public GObject {
  private:
    std::stack<WidgetInterface *> ___widgetStack__;
    static WidgetInterface *___focused;
    int layer=0;
  public:
    void setLayer(int layer_) { layer = layer_; }
    std::stack<WidgetInterface *> &getWidgetStack() { return ___widgetStack__; }
    virtual void onKeyPressed(sf::Keyboard::Key keyCode){}
    virtual void onMousePressed(sf::Mouse::Button btnCode){}
    virtual ~WidgetInterface() {};
    WidgetInterface() {}
};
inline WidgetInterface *WidgetInterface::___focused=nullptr;

inline void GWorldInterface::render() {};
#endif