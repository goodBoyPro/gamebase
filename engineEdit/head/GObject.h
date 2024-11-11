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
//          GComponent            ////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GComponent : public GObject {
  public:
    virtual ~GComponent() {}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          GControllerInterface            ////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GControllerInterface : public GObject {
  private:
    static GControllerInterface *playerController;
  public:
    GControllerInterface() {
        for (std::function<void()> &func : bindKey)
            func = []() { printf("No Key Bind\n"); };
        for (std::function<void(float)> &func : bindAxis)
            func = [](float) { printf("No Key Bind\n"); };
    }
    std::function<void()> bindKey[128];
    std::function<void(float)> bindAxis[128];
    virtual void pollKey() = 0;
    virtual ~GControllerInterface() {}
    public:
      static GControllerInterface *getPlayerController() { return playerController; }
      static void setPlayerController(GControllerInterface *ptr) {
          playerController = ptr;
    }
};
inline GControllerInterface* GControllerInterface::playerController = nullptr;
inline GControllerInterface *getPlayerController(){
    return GControllerInterface::getPlayerController();
}
inline void setPlayerController(GControllerInterface *ptr){
    GControllerInterface::setPlayerController(ptr);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           gameInterface             ////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GGameInterface:public GObject{
  private:
  static GGameInterface*gameInsX;
  public:
  GGameInterface(){setGameIns(this);};
  virtual~GGameInterface(){}
  virtual void renderLoop2D()=0;
  static GGameInterface*getGameIns(){return gameInsX;}
  static void setGameIns(GGameInterface*ptr){gameInsX=ptr;}  
};
inline GGameInterface*gameInsX=nullptr;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          cameraInterface               ////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GCameraInterface:public GObject{
  private:
  static GCameraInterface*gameCameraX;
  public:
  ~GCameraInterface(){};
  static GCameraInterface*getGameCamera(){return gameCameraX;}
  static void setGameCamera(GCameraInterface*ptr){gameCameraX=ptr;}
};
inline GCameraInterface*gameCameraX=nullptr;
#endif