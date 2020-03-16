#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include <cstdlib>
#include <vector>
//#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm//gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glad/glad.h>

#define screenWidth 1280
#define screenHeight 720

using namespace std;

GLFWwindow* initGL();

class Shader {
private:
    unsigned int shaderProgram;
    //Rectangle& 
public:
    Shader();

    ~Shader();

    //Shader(Rectangle& );

    void useProgram();

    void setVertexColor(float r, float g, float b);

    //void setVertexScale(float b, float h, float v);

    //void setVertexTranslation(float b, float h, float v);

    void setTransform(const glm::mat4& transform);
};

class Buffer {
private:
    unsigned int vao;
public:
    Buffer(vector<float>& vertices, vector<int>& indices);

    void bind();
};

class Rectangle {
private:
    Shader& shaderProgram;
    Buffer& buffer;
    glm::vec3 s;
    glm::vec3 t;

public:
    Rectangle(Shader& shader, Buffer& buffer);

    virtual void color(float r, float g, float b);

    //virtual void rotate();

    virtual void scale(float x, float y, float z);

    virtual void setPosition(float x, float y, float z);

    virtual void draw();
};

class Player : public Rectangle {
public:
    Player(Shader& shader, Buffer& buffer);

    //lint score();
};