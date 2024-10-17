#version 330 core
layout (location = 6) in vec3 aPos;
layout (location=7) in vec3 aColor;
layout (location=8) in vec2 UV;
uniform vec4 ourColor;
uniform mat4 trans;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
out vec4 vertexColor;
out vec2 texCoord;
mat4 transtest = mat4(1.0f); // 初始化为单位矩阵  
void main()
{
    
    
   gl_Position =projMat*viewMat*modelMat*vec4(aPos, 1.0);
   vertexColor=vec4(aColor,1.f);
   texCoord=UV;
}

