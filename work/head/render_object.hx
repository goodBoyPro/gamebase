#if !defined(RENDER_OBJECT_H)
#define RENDER_OBJECT_H
#include <shader.h>
class RenderObject {
  private:
    /* data */
  public:
    RenderObject(/* args */);
    ~RenderObject();

  public:
    // 用于本地空间转世界空间
    glm::mat4 modelMat = glm::mat4(1.0f);
    guint vao;
    guint vbo;
    guint ebo;
    shader shaderIns =
        shader("bin/res/vertexSource.vert", "bin/res/fragmentSource.vert");

    void init();
    void draw();
    // 草地渲染示例
    void drawGpuInstance();
};

#endif // RENDER_OBJECT_H
