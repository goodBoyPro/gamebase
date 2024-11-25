#ifndef GOBJECT_H
#define GOBJECT_H
#include "GBase.h"
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
    static std::vector<GControllerInterface*>&getAllController(){
        static std::vector<GControllerInterface *> allCtrl;
        return allCtrl;
    }
    void setActive() { getAllController()[id] = this; }
    void disableActive() { getAllController()[id] = nullptr; }

  public:
    GControllerInterface() {
        getAllController().push_back(this);
        id = getAllController().size();
        for (std::function<void()> &func : bindKey)
            func = []() { printf("No Key Bind\n"); };
        for (std::function<void(float)> &func : bindAxis)
            func = [](float) { printf("No Key Bind\n"); };
    }
    std::function<void()> bindKey[128];
    std::function<void(float)> bindAxis[128];
    virtual void pollKey() = 0;
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
    friend IVector wsToWin(const FVector3 &PositionInWS);
    friend FVector3 winToWs(const IVector &positionInWin);

  public:
    IVector positionInWin = IVector(WINW / 2, WINH / 2);
    FVector3 posInWs = FVector3(0, 0, 0);
    ~GCameraInterface() {};
    static GCameraInterface *getGameCamera() { return gameCameraX; }
    static void setGameCamera(GCameraInterface *ptr) { gameCameraX = ptr; }
};
inline GCameraInterface *GCameraInterface::gameCameraX = nullptr;
inline void setGameCamera(class GCameraInterface *camera_) {
    GCameraInterface::setGameCamera(camera_);
}
inline GCameraInterface *getGameCamera() {
    return GCameraInterface::getGameCamera();
}
inline IVector wsToWin(const FVector3 &PositionInWS) {
    return {static_cast<int>(roundf(
                (PositionInWS.x - GCameraInterface::gameCameraX->posInWs.x) /
                    pixSize +
                WINW / 2.f)),
            static_cast<int>(roundf(
                (PositionInWS.y - GCameraInterface::gameCameraX->posInWs.y) /
                    pixSize +
                WINH / 2.f - (PositionInWS.z / pixSize)))};
}
inline FVector3 winToWs(const IVector &positionInWin) {
    return {(positionInWin.x - WINW / 2) * pixSize +
                GCameraInterface::gameCameraX->posInWs.x,
            (positionInWin.y - WINH / 2) * pixSize +
                GCameraInterface::gameCameraX->posInWs.y,
            0};
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           gameInterface
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GGameInterface : public GObject {
  private:
    static GGameInterface *gameIns;

  public:
    sf::Event event;
    bool bGameContinue = 1;
    GGameInterface() { setGameIns(this); };
    virtual ~GGameInterface() {}
    virtual void renderLoop2D() = 0;
    static GGameInterface *&getGameIns() { return gameIns; }
    static void setGameIns(GGameInterface *ptr) { gameIns = ptr; }
};
inline GGameInterface *GGameInterface::gameIns = nullptr;
inline GGameInterface *&getGameIns() { return GGameInterface::getGameIns(); }
inline void setGameIns(class GGameInterface *game_) {
    GGameInterface::setGameIns(game_);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////widgetInterface///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stack>
class WidgetInterface : public GObject {
  private:
    int id;

  public:
    std::vector<WidgetInterface *> &getAllWidget() {
        static std::vector<WidgetInterface *> allwidget;
        return allwidget;
    }
    std::stack<WidgetInterface *>&getWidgetStack(){}
    void setActive() { getAllWidget()[id] = this; }
    void disableActive() { getAllWidget()[id] = nullptr; }
    virtual ~WidgetInterface(){getAllWidget()[id] = nullptr;};
    WidgetInterface() {
        getAllWidget().push_back(this);
        id = getAllWidget().size();
    }
};

#endif