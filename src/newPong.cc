#include "newPong.h"

//void processInput(GLFWwindow* window, Shader& shaderProgram, Buffer& objBuffer){
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int Width, int Height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, Width, Height);
}


GLFWwindow* initGL() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = true;	//http://glew.sourceforge.net/basic.html
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return nullptr;
    }

    return window;
}

GLFWwindow* window = nullptr;

int main() {

    //float screenW = 1.0f;
    //float screenH = 1.0f;

    window = initGL();
    if (window == nullptr)
        return -1;

    Shader objShader;

    vector<float> vertices = {
        // positions
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
    };
    
    vector<int> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    Buffer objBuffer(vertices, indices);

    Rectangle* ball = new Rectangle(objShader, objBuffer);
    Player* playerOne = new Player(objShader, objBuffer);
    Player* playerTwo = new Player(objShader, objBuffer);

    playerOne->setPosition(-0.99f, 0.0f, 1.0f);
    playerTwo->setPosition(0.99f, 0.0f, 1.0f);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //ball
    float xSpeed = 0.015f;
    float ySpeed = 0.012f;

    float xBall = 0;
    float yBall = 0;

    float yPlayerOne = 0.0f;
    float yPlayerTwo = 0.0f;

    float ySpeedPlay = 0.04;

    float sizeBall = 0.02;

    float sizePlayerX = 0.0198;
    float sizePlayerY = 0.30;

    float spd = 1.08;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        //ball
        ball->scale(0.02f, 0.035f, 1.0f);
        ball->color(1.0f, 1.0f, 1.0f);
        ball->setPosition(xBall += xSpeed, yBall += ySpeed, 0.0);

        if (xBall + sizeBall / 2 > 0.99 - sizePlayerX / 2 && yBall - sizeBall / 2  < yPlayerTwo + sizePlayerY / 2
            && yBall + sizeBall / 2 > yPlayerTwo - sizePlayerY / 2) {
            spd = -spd;
            xSpeed = xSpeed * spd;
            //xBall = xBall 
            ySpeed = -ySpeed * spd;
        }

        if (xBall - sizeBall / 2 < -0.99 + sizePlayerX / 2 && yBall - sizeBall / 2 < yPlayerOne + sizePlayerY / 2
            && yBall + sizeBall / 2 > yPlayerOne - sizePlayerY / 2) {
            spd = -spd;
            xSpeed = -xSpeed * spd;
            ySpeed = ySpeed * spd;
        }

        if (yBall + sizeBall > 0.99) {
            ySpeed = -ySpeed;
        }

        if (yBall - sizeBall < -0.99) {
            ySpeed = -ySpeed;
        }

        if (xBall < -1.0 || xBall > 1.0) {
            yBall = 0;
            xBall = 0;
            xSpeed = 0.015f;
            ySpeed = 0.012f;
        }
        
        ball->draw();

        //playerOne
        playerOne->color(0.0f, 0.0f, 1.0f);
        playerOne->scale(0.02f, 0.3f, 1.0f);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            playerOne->setPosition(-0.99f, yPlayerOne += ySpeedPlay, 1.0f);
            
            if (yPlayerOne + sizePlayerY / 2 > 0.99)
                yPlayerOne = yPlayerOne - sizePlayerY/2;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            playerOne->setPosition(-0.99f, yPlayerOne -= ySpeedPlay, 1.0f);

            if (yPlayerOne - sizePlayerY / 2 < -0.99)
                yPlayerOne = yPlayerOne + sizePlayerY/2;
        }
        playerOne->draw();
        

        //playerTwo
        playerTwo->color(1.0f, 0.0f, 0.0f);
        playerTwo->scale(0.02f, 0.3f, 1.0f);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            playerTwo->setPosition(0.99f, yPlayerTwo += ySpeedPlay, 1.0f);

            if (yPlayerTwo + sizePlayerY / 2 > 0.99)
                yPlayerTwo = yPlayerTwo - sizePlayerY/2;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { 
            playerTwo->setPosition(0.99f, yPlayerTwo -= ySpeedPlay, 1.0f);

            if (yPlayerTwo - sizePlayerY / 2 < -0.99)
                yPlayerTwo = yPlayerTwo + sizePlayerY/2;
        }
        playerTwo->draw();

        // create transformations
        //glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        
        //objShader.useProgram();
        //unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        /*COLOR
        float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "vertexColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 1.0f, 1.0f);
        */
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}