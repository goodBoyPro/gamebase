#include <camera.h>

camera::camera(gvec3 _position, gvec3 _target, gvec3 _worldUp) {
    ;
    position = _position;
    forward = glm::normalize(_target - position);
    worldUp = _worldUp;
    right = glm::cross(forward, worldUp);
    up = glm::cross(right, forward);

    
}

camera::camera(gvec3 _position, float _pitch, float _yaw, gvec3 _worldUp) {
    ;
    position = _position;
    worldUp = _worldUp;
    pitch = _pitch;
    yaw = _yaw;
    forward.x = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
    forward.y = glm::sin(glm::radians(pitch));
    forward.z = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
    right = glm::cross(forward, worldUp);
    up = glm::cross(right, forward);
    //初始化裁剪矩阵
    projMat = glm::perspective(fov,wDh,0.1f,zFar);
}

camera::~camera() {}

gMat4 camera::getViewMat() {
    updateCameraVector();
    return glm::lookAt(position, position + forward, worldUp);
}
void camera::updateCameraVector() {
    ;
    forward.x = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
    forward.y = glm::sin(glm::radians(pitch));
    forward.z = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
}


void camera::input(float _pitch, float _yaw) {
    pitch += _pitch / 100;
    yaw += _yaw / 100;   
}
void camera::move(float z) {
    position+= forward*z;
    
}
