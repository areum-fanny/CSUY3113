//Name: Shakthi Shrree Ganta Gameshwaran
//Course: Intro to Game Engineering
//Name of Project: Lunar Lander
//Description of Project: A spaceship has lost its route and is heading towards the city. There is a lake in the city and if the spaceship doesn't land in the lake it will
//                        crash into the surrounding buildings. The goal of the game is to land the spaceship in the water. 
//                        The player controls the spaceship with left and right arrow keys to move left and right. The game begins as soon as the screen loads. 


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

#include "Entity.h"
#define ROCKS_COUNT 31
#define FAILURE_COUNT 17
#define SUCCESS_COUNT 21

struct GameState {
    Entity* player;                     //The player/spaceship 
    Entity* rocks;                      //The buildings/obstacles
    Entity* failure;                    //Final mission failed display entity
    Entity* success;                    //Final mission successful display entity
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

int* failed = new int[17]{ 77,105,115,115,105,111,110,32,70,97,105,108,101,100,32,58,40 };      //Holds indices of "Mission Failed :("
int* success = new int[21]{ 77,105,115,115,105,111,110,32,83,117,99,99,101,115,115,102,117,108,32,58,41 }; //Holds indices of "Mission Successful :)"

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
    displayWindow = SDL_CreateWindow("Lunar Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->position = glm::vec3(0,3.75,0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -0.02f, 0);
    state.player->speed = 0.1f;
    state.player->textureID = LoadTexture("spaceship.png");

    state.player->animIndices = new int[1]{0};
    state.player->animIndex = 0;
    state.player->animCols = 1;
    state.player->animRows = 1;
    state.player->height = 0.7f;
    state.player->width = 0.8f;

    //Initialize Rocks
    float xpos = -5.5f;
    float ypos = -3.25f;
    state.rocks = new Entity[ROCKS_COUNT];
    GLuint rocksTextureID = LoadTexture("rocks.png");
    for (int i = 0; i < 2;i++) {
        xpos += 1.0f;
        state.rocks[i].textureID = rocksTextureID;
        state.rocks[i].position = glm::vec3(xpos, ypos, 0);
        
    }
    //Initialize water
    GLuint waterTextureID = LoadTexture("water.png");
    for (int i = 2; i < 4; i++) {
        xpos += 1.0f;
        state.rocks[i].textureID = waterTextureID;
        state.rocks[i].position = glm::vec3(xpos, ypos, 0);
        state.rocks[i].isdestination = true;
    }
    //Initialize rocks
    for (int i = 4; i < 10; i++) {
        xpos += 1.0f;
        state.rocks[i].textureID = rocksTextureID;
        state.rocks[i].position = glm::vec3(xpos, ypos, 0);
    }
    for (int i = 10; i < 17; i++) {
        ypos += 1.0f;
        state.rocks[i].textureID = rocksTextureID;
        state.rocks[i].position = glm::vec3(xpos, ypos, 0);
    }
    xpos = -4.5f;
    ypos = -3.25f;
    for (int i = 17; i < 24; i++) {
        ypos += 1.0f;
        state.rocks[i].textureID = rocksTextureID;
        state.rocks[i].position = glm::vec3(xpos, ypos, 0);
    }

    ypos = -0.75f;
    for (int i = 24; i < 27; i++) {
        xpos += 1.0f;
        state.rocks[i].textureID = rocksTextureID;
        state.rocks[i].position = glm::vec3(xpos, ypos, 0);
        
    }
    xpos = 4.5f;
    ypos = 1.75f;
    for (int i = 27; i < ROCKS_COUNT; i++) {
        xpos -= 1.0f;
        state.rocks[i].textureID = rocksTextureID;
        state.rocks[i].position = glm::vec3(xpos, ypos, 0);
    }
    for (int i = 0; i < ROCKS_COUNT; i++) {
        state.rocks[i].Update(0,NULL,0);
    }
    //Initialise game result

    xpos = -3.0f;
    //Failure
    state.failure = new Entity[FAILURE_COUNT];
    GLuint fontTextureID = LoadTexture("font1.png");
    for (int i = 0;i < FAILURE_COUNT;i++) {
        state.failure[i].textureID = fontTextureID;
        state.failure[i].animCols = 16;
        state.failure[i].animRows = 16;
        state.failure[i].animIndex = 0;
        state.failure[i].animIndices = new int[1]{ failed[i] };
        state.failure[i].position = glm::vec3(xpos, 0.0f, 0.0f);
        xpos += 0.4f;
    }
    
    for (int i = 0; i < FAILURE_COUNT; i++) {
        state.failure[i].Update(0, NULL, 0);
    }

    //Success
    state.success = new Entity[SUCCESS_COUNT];
    xpos = -3.8f;
    for (int i = 0; i < SUCCESS_COUNT;i++) {
        state.success[i].textureID = fontTextureID;
        state.success[i].animCols = 16;
        state.success[i].animRows = 16;
        state.success[i].animIndex = 0;
        state.success[i].animIndices = new int[1]{ success[i] };
        state.success[i].position = glm::vec3(xpos, 0.0f, 0.0f);
        xpos += 0.4f;
    }
    for (int i = 0; i < SUCCESS_COUNT; i++) {
        state.success[i].Update(0, NULL, 0);
    }
}

void ProcessInput() {

    state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT]) {
        state.player->movement.x = -0.1f;
        
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        state.player->movement.x = 0.1f;

    }

    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->Update(FIXED_TIMESTEP, state.rocks,ROCKS_COUNT);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < ROCKS_COUNT; i++){
        state.rocks[i].Render(&program);
    }

    state.player->Render(&program);
    if (state.player->reacheddestination) {                 //Checks if spaceship safely reached water
        for (int i = 0; i < SUCCESS_COUNT; i++) {
            state.success[i].Render(&program);
        }
    }
    if (state.player->collidedwithrock) {                   //Checks if spaceship collided with the buildings/obstacles
        for (int i = 0; i < FAILURE_COUNT; i++) {
            state.failure[i].Render(&program);
        }
    }
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
