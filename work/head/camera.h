#if !defined(CAMERA_H)
#define CAMERA_H
#include <shader.h>
class camera {

  public:
    camera() = default;
    camera(gvec3 _position, gvec3 _target, gvec3 _worldUp);
    camera(gvec3 _position, float _pitch, float _yaw,
           gvec3 _worldUp = {0, 1, 0});
    ~camera();
    gMat4 getViewMat();
    gvec3 position;
    gvec3 forward;
    gvec3 right;
    gvec3 up;
    gvec3 worldUp;
    float pitch, yaw;

    void input(float _pitch, float _yaw);

    void move(float z);

  public:
    gMat4 projMat = gMat4(1.0f);
    // 视角，宽高比，最远距离
    float fov = 45.f, wDh = 8 / 6, zFar = 100.f;

  private:
    void updateCameraVector();
};

#endif // CAMERA_H
