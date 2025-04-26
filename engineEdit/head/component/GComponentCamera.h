#if !defined(GCOMPONENTCAMERA)
#define GCOMPONENTCAMERA
#include <GActor.h>
class GComponentCamera : public GActorComponent {
  private:
    /* data */
  public:
    GICamera camera;
    GComponentCamera(/* args */);
    ~GComponentCamera();

  public:
  void eventTick(){}
  void eventBegin(){}
    virtual void setPosInWs(const FVector3 pos_) {
        GActorComponent::setPosInWs(pos_);
        camera.posInWs = getPosInWs();
    }
};
//////////////////////////////////////////////////////////////////////////////////////////////
/// xxxxxxx////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
inline GComponentCamera::GComponentCamera(/* args */) {}

inline GComponentCamera::~GComponentCamera() {}

#endif // GCOMPONENTCAMERA
