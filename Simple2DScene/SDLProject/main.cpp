#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, sunMatrix, projectionMatrix, moonMatrix, cloudMatrix, raindropMatrix, pinwheelMatrix, birdMatrix, grassMatrix, pinwheel2Matrix, pinwheel3Matrix;

float raindrop = -0.6f;
float sun = -7.0f;
float moon = -6.0f;
float bird = -6.0f;
float rotate = 0;
float lastTicks = 0.0f;

GLuint sunTextureID, moonTextureID, rainTextureID, cloudTextureID, pinwheelTextureID, birdTextureID, grassTextureID,pinTextureID;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Simple2DScene", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    sunMatrix = glm::mat4(1.0f);
    moonMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    

    glUseProgram(program.programID);

    glClearColor(0.9f,0.9f,0.2f,1.0f);

    glEnable(GL_BLEND);
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sunTextureID = LoadTexture("sun.png");
    moonTextureID = LoadTexture("moon.png");
    cloudTextureID = LoadTexture("cloud.png");
    rainTextureID = LoadTexture("raindrop.png");
    pinwheelTextureID = LoadTexture("pinwheel.png");
    birdTextureID = LoadTexture("bird.png");
    grassTextureID = LoadTexture("grass.png");
    pinTextureID = LoadTexture("pinwheel.png");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}


void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    raindrop -= 1.0f * deltaTime;
    rotate += 90.0f * deltaTime;
    sun += 1.0f * deltaTime;
    bird += 1.5 * deltaTime;

    if (bird > 8.0f) {
        bird = -6.0f;
    }
    if (raindrop < -5.0f) {
        raindrop = -0.6f;
    }
    if (sun > 7.0f) {
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        moon += 1.0f * deltaTime;
    }
    if (moon > 7.0f) {
        glClearColor(0.9f, 0.9f, 0.2f, 1.0f);
        sun = -7.0f;
        moon = -6.0f;
    }
   
    sunMatrix = glm::mat4(1.0f);
    moonMatrix = glm::mat4(1.0f);
    cloudMatrix = glm::mat4(1.0f);
    raindropMatrix = glm::mat4(1.0f);
    pinwheelMatrix = glm::mat4(1.0f);
    birdMatrix = glm::mat4(1.0f);
    grassMatrix = glm::mat4(1.0f);
    pinwheel2Matrix = glm::mat4(1.0f);
    pinwheel3Matrix = glm::mat4(1.0f);

    sunMatrix = glm::translate(sunMatrix, glm::vec3(sun, 3.0f, 0.0f));
    moonMatrix = glm::translate(moonMatrix, glm::vec3(moon, 3.0f, 0.0f));
    birdMatrix = glm::translate(birdMatrix, glm::vec3(bird, 1.5f, 0.0f));
    cloudMatrix = glm::translate(cloudMatrix, glm::vec3(0.0f, 2.5f, 0.0f));
    cloudMatrix = glm::scale(cloudMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
    pinwheelMatrix = glm::translate(pinwheelMatrix, glm::vec3(0.0f, -2.5f, 0.0f));
    pinwheelMatrix = glm::rotate(pinwheelMatrix, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    grassMatrix = glm::translate(grassMatrix, glm::vec3(0.0f, -3.0f, 0.0f));
    grassMatrix = glm::scale(grassMatrix, glm::vec3(12.0f, 2.0f, 1.0f));
    raindropMatrix = glm::translate(cloudMatrix, glm::vec3(0.0f, raindrop, 0.0f));
    raindropMatrix = glm::scale(raindropMatrix, glm::vec3(0.5f, 0.5f, 1.0f));
    pinwheel2Matrix = glm::translate(pinwheel2Matrix, glm::vec3(1.0f, -2.3f, 0.0f));
    pinwheel2Matrix = glm::rotate(pinwheel2Matrix, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    pinwheel3Matrix = glm::translate(pinwheel3Matrix, glm::vec3(-2.5f, -2.6f, 0.0f));
    pinwheel3Matrix = glm::rotate(pinwheel3Matrix, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
}
void displaySun() {
    program.SetModelMatrix(sunMatrix);
    glBindTexture(GL_TEXTURE_2D, sunTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void displayCloud() {
    program.SetModelMatrix(cloudMatrix);
    glBindTexture(GL_TEXTURE_2D, cloudTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void displayRain() {
    program.SetModelMatrix(raindropMatrix);
    glBindTexture(GL_TEXTURE_2D, rainTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void displayMoon() {
    program.SetModelMatrix(moonMatrix);
    glBindTexture(GL_TEXTURE_2D, moonTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void displaypinwheel() {
    program.SetModelMatrix(pinwheelMatrix);
    glBindTexture(GL_TEXTURE_2D, pinwheelTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void displaybird() {
    program.SetModelMatrix(birdMatrix);
    glBindTexture(GL_TEXTURE_2D, birdTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void displaygrass() {
    program.SetModelMatrix(grassMatrix);
    glBindTexture(GL_TEXTURE_2D, grassTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void displaypin2() {
    program.SetModelMatrix(pinwheel2Matrix);
    glBindTexture(GL_TEXTURE_2D, pinwheelTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void displaypin3() {
    program.SetModelMatrix(pinwheel3Matrix);
    glBindTexture(GL_TEXTURE_2D, pinwheelTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    
    displaySun();
    displayMoon();
    displayCloud();
    displaypinwheel();
    displaybird();
    displayRain();
    displaypin2();
    displaypin3();
    displaygrass();

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}
