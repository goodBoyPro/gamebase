#ifndef GCAMERA_H
#define GCAMERA_H
#include <GActor.h>
#include <vector>
#include<camera.h>
class GCamera :public GObject {
  private:
    char x[8];

  public:
    GCamera() { }
    ~GCamera(){};
    IVector positionInWin = IVector(WINW / 2, WINH / 2);
    FVector posInWs=FVector(0,0);
    
    
    
   
};
#endif
