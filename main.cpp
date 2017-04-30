// C Library Includes
#include <signal.h>

// Std. Includes
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// SDL_Mixer
#include <SDL/SDL_mixer.h>

// My includes
#include "Game.h"
#include "Resource_Manager.h"

// Function prototypes
void sigint_handler(int sig);
void printFps();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Properties
Game game;

// The MAIN function, from here we start our application and run our Game loop
int main (int argc, char *argv[])
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWmonitor* primary;
    const GLFWvidmode* primary_mode;
    primary = glfwGetPrimaryMonitor();
    primary_mode = glfwGetVideoMode(primary);
    game.Width=primary_mode->width;
    game.Height=primary_mode->height;

    // Window
    GLFWwindow* window = glfwCreateWindow(game.Width, game.Height, "Underwater exploration", glfwGetPrimaryMonitor(), nullptr); // Windowed
    glViewport(0, 0, game.Width, game.Height);
    glfwMakeContextCurrent(window);


    // Set the required callback functions
    signal(SIGINT, sigint_handler);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();
    glGetError();

    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(game.Height/160);

    // Time variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;


    // Initialize game
    game.Init();

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        printFps();
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // Manage user input
        game.ProcessInput(deltaTime);

        // Update Game state
        game.Update(deltaTime, currentFrame);

        // Render
        //glClearColor(0.4f, 0.0f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        game.Render();

        // Swap the buffers
        glfwSwapBuffers(window);
    }
    // Delete all resources as loaded using the resource manager
    ResourceManager::Clear();
    glfwTerminate();
    return 0;
}


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, __attribute__((unused)) int scancode, int action, __attribute__((unused)) int mods)
{
    //cout << key << endl;
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            game.Keys[key] = GL_TRUE;
        else if(action == GLFW_RELEASE){
            game.Keys[key] = GL_FALSE;
            game.ProcessedKeys[key] = GL_FALSE;
        }
    }
}

void mouse_callback(__attribute__((unused)) GLFWwindow* window, double xpos, double ypos)
{
    game.ProcessMouseMovement(xpos, ypos);
}


void scroll_callback(__attribute__((unused)) GLFWwindow* window, __attribute__((unused)) double xoffset, double yoffset)
{
    game.ProcessMouseScroll(yoffset);
}

void printFps(void){
    float t;
    static float t0=0,f= 0;
    f++;
    t = glfwGetTime();
    if(t - t0 > 2.000) {
        printf("%f\n", (f / (t - t0)));
        t0 = t;
        f  = 0;
    }
}


void sigint_handler(__attribute__((unused)) int sig) {
    ResourceManager::Clear();
    glfwTerminate();
    exit(1);
}