#if !defined(GCOMPONENTCAMERA)
#define GCOMPONENTCAMERA
#include <GActor.h>
class GComponentCamera : public GActorComponent {
  private:
    /* data */
  public:
    GCameraInterface camera;
    GComponentCamera(/* args */);
    ~GComponentCamera();

  public:
    virtual void setPosInWs(const FVector3 pos_) {
        GActorComponent::setPosInWs(pos_);
        camera.posInWs = pos_;
    }
};
//////////////////////////////////////////////////////////////////////////////////////////////
/// xxxxxxx////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
inline GComponentCamera::GComponentCamera(/* args */) {}

inline GComponentCamera::~GComponentCamera() {}

#endif // GCOMPONENTCAMERA
