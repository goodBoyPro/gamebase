#if !defined(SHADER_H)
#define SHADER_H
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <stdio.h>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using gvec3 =glm::vec3;
using gvec34 =glm::vec4;
using gMat4 = glm::mat4;
using guint = unsigned int;
class shader {

  public:
    unsigned int ID;//program id
    shader(const char *vertexPath, const char *fragmentPath);
    ~shader(){};

    const char *vertexSource;
    const char *fragmentSource;
    void sourceToConstChar(const char *path) {};
    void use();
 
  private:
    
    //公共资源加载只执行一次
    int loadSource(const char *vertexPath, const char *fragmentPath);

    void checkCompileError(guint id,std::string type);
    //加载贴图
    GLuint loadTextureBufferRGB(guint gpuLocation,const char *imagePath);
    GLuint loadTextureBufferRGBA(guint gpuLocation,const char *imagePath);
    guint texBuffer[16];
    
};

#endif // SHADER_H
