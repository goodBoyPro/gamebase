#include "shader.h"
#include <exception>
#include <iostream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
shader::shader(const char *vertexPath, const char *fragmentPath) {

    static int a = loadSource(vertexPath, fragmentPath);
    texBuffer[4] = loadTextureBufferRGB(GL_TEXTURE4, "bin/res/test.png");
    texBuffer[5] = loadTextureBufferRGBA(GL_TEXTURE5, "bin/res/a.png");
}

void shader::use() {
    glBindTexture(GL_TEXTURE_2D, texBuffer[4]);
    glBindTexture(GL_TEXTURE_2D, texBuffer[5]);
    glUniform1i(glGetUniformLocation(ID, "tex0"), 4);
    glUniform1i(glGetUniformLocation(ID, "tex1"), 5);
    glUseProgram(ID);
}

int shader::loadSource(const char *vertexPath, const char *fragmentPath) {
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    vertexFile.open(vertexPath);
    fragmentFile.open(fragmentPath);
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::stringstream vertexSStream;
    std::stringstream fragmentSStream;
    try {
        if (!vertexFile.is_open() || !fragmentFile.is_open()) {
            throw std::exception();
        }
        vertexSStream << vertexFile.rdbuf();
        fragmentSStream << fragmentFile.rdbuf();
        std::string vertexString;
        std::string fragmentString;
        vertexString = vertexSStream.str();
        fragmentString = fragmentSStream.str();

        vertexSource = vertexString.c_str();
        fragmentSource = fragmentString.c_str();

        unsigned int vertexShader, fragmentShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, NULL);
        glCompileShader(vertexShader);
        checkCompileError(vertexShader, "1");
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);
        checkCompileError(vertexShader, "1");
        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);
        checkCompileError(ID, "PROGRAM");
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

    } catch (const std::exception &e) {
        std::cout << "an error ocurred" << e.what() << '\n';
        // printf("an error ocurred");
        // printf(e.what());
    }
    return 0;
}

void shader::checkCompileError(guint ID, std::string type) {

    int success;
    char infolog[512];
    if (type == "PROGRAM") {
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infolog);
            printf("\nprogram linking Error:");
            printf(infolog);
        }
    } else {
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(ID, 512, NULL, infolog);
            printf("\nshader compile Error:");
            printf(infolog);
        }
    }
}
// gpuLocation:GL_TEXTURE0...
GLuint shader::loadTextureBufferRGB(guint gpuLocation, const char *imagePath) {
  
    unsigned int texBuffer;
    glGenTextures(1, &texBuffer);
    glActiveTexture(gpuLocation);
    glBindTexture(GL_TEXTURE_2D, texBuffer);

    int width, height, channel;
    unsigned char *data = stbi_load(imagePath, &width, &height, &channel, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("image load failed");
    }
    stbi_image_free(data);

    return texBuffer;
}

GLuint shader::loadTextureBufferRGBA(guint gpuLocation, const char *imagePath) {
    unsigned int texBuffer;
    glGenTextures(1, &texBuffer);
    glActiveTexture(gpuLocation);
    glBindTexture(GL_TEXTURE_2D, texBuffer);

    int width, height, channel;
    unsigned char *data = stbi_load(imagePath, &width, &height, &channel, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("image load failed");
    }
    stbi_image_free(data);

    return texBuffer;
}
