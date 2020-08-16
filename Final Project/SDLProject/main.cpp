#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 2

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Mesh.h"
#define OBJECT_COUNT 8
#define ENEMY_COUNT 10
SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

struct GameState {
    Entity* player;
    Entity* objects;
    Entity* enemies;
};

GameState state;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("3D!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 1280, 720);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(45.0f), 1.777f, 0.1f, 100.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(program.programID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0,3.5f, 0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 1.0f;
    state.player->jump = false;
    state.player->jumpPower = 10.0f;
    state.player->height = 3.0f;
    

    state.objects = new Entity[OBJECT_COUNT];

    Mesh* cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj",40);

    GLuint floorTextureID = Util::LoadTexture("floor.jpg");
    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].rotation = glm::vec3(0, 0, 0);
    state.objects[0].position = glm::vec3(0, -0.5f, 0);
    state.objects[0].entityType = FLOOR;
    state.objects[0].acceleration = glm::vec3(0, 0, 0);
    state.objects[0].scale = glm::vec3(40, 0.5f, 40);
    state.objects[0].height = 0.5f;
    state.objects[0].width = 40.0f;
    state.objects[0].depth = 40.0f;

    GLuint crateTextureID = Util::LoadTexture("crate1_diffuse.png");
    Mesh* crateMesh = new Mesh();
    crateMesh->LoadOBJ("cube.obj", 1);
    state.objects[1].textureID = crateTextureID;
    state.objects[1].mesh = crateMesh;
    state.objects[1].position = glm::vec3(0, 0.5, -3);
    state.objects[1].entityType = CRATE;
    state.objects[2].textureID = crateTextureID;
    state.objects[2].mesh = crateMesh;
    state.objects[2].position = glm::vec3(0, 0.5, -4);
    state.objects[2].entityType = CRATE;
    state.objects[3].textureID = crateTextureID;
    state.objects[3].mesh = crateMesh;
    state.objects[3].position = glm::vec3(0, 1.5, -3);
    state.objects[3].entityType = CRATE;


    GLuint wallTextureID = Util::LoadTexture("floor1.jpg");

    Mesh* wallMesh = new Mesh();
    wallMesh->LoadOBJ("cube.obj", 20);
    for (int i = 4; i < 8; i++) {
        state.objects[i].textureID = wallTextureID;
        state.objects[i].mesh = wallMesh;
        state.objects[i].rotation = glm::vec3(0, 0, 0);
        state.objects[i].entityType = CRATE;    
        state.objects[i].height = 20;
        state.objects[i].width = 0.5f;
        state.objects[i].depth = 40;
        if (i % 2 == 0) { 
            state.objects[i].scale = glm::vec3(0.5f, 20, 40);
            state.objects[i].position = glm::vec3(40.5f, 0.5f, 0);
        }
        else {
            
            state.objects[i].scale = glm::vec3(0.5f, 20, 40);
            state.objects[i].position = glm::vec3(-40.5f, 0.5f, 0);
        }
    }
    state.objects[6].position = glm::vec3(0, 0.5f, 40.5f);
    state.objects[6].scale = glm::vec3(40, 20, 0.5f);
    state.objects[6].width = 40;
    state.objects[6].depth = 0.5f;
    state.objects[7].position = glm::vec3(0, 0.5f, -40.5f);
    state.objects[7].scale = glm::vec3(40, 20, 0.5f);
    state.objects[7].width = 40;
    state.objects[7].depth = 0.5f;


    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].billboard = true;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].position = glm::vec3(rand() % 20 - 10, 0.5f, rand() % 20 - 10);
        state.enemies[i].rotation = glm::vec3(0, 0, 0);
        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
        state.enemies[i].entityType = CRATE;
        state.enemies[i].type = ENEMY;
    }
}


void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                if (state.player->collidedBottom) {
                    state.player->jump = true;
                    break;
                }

            }
            break;
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_A]) {
        state.player->rotation.y += 1.0f;
    }
    else if (keys[SDL_SCANCODE_D]) {
        state.player->rotation.y -= 1.0f;
    }
    state.player->velocity.x = 0;
    state.player->velocity.z = 0;
    if (keys[SDL_SCANCODE_W]) {
        state.player->velocity.z = cos(glm::radians(state.player->rotation.y)) * -10.0f;
        state.player->velocity.x = sin(glm::radians(state.player->rotation.y)) * -10.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        state.player->velocity.z = cos(glm::radians(state.player->rotation.y)) * 10.0f;
        state.player->velocity.x = sin(glm::radians(state.player->rotation.y)) * 10.0f;
    }
    if (keys[SDL_SCANCODE_UP]) {
        state.player->rotation.x += 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        state.player->rotation.x -= 1.0f;
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
        state.player->Update(FIXED_TIMESTEP,state.player,state.objects,OBJECT_COUNT);
        for (int i = 0; i < OBJECT_COUNT; i++) {
            state.objects[i].Update(FIXED_TIMESTEP, state.player, state.objects, OBJECT_COUNT);
        }
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.enemies, ENEMY_COUNT);
        }
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix,glm::radians(state.player->rotation.y), glm::vec3(0, -1.0f, 0));
    viewMatrix = glm::translate(viewMatrix, -state.player->position);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Render(&program);
    }
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
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
