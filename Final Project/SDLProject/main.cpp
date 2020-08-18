//Name: Shakthi Shrree Ganta Gameshwaran
//Project Name: 3D Platformer
//Description of Project: You are stuck in a maze, you need to avoid the enemies, pick up the key and reach the final destination/door. A, S, W and D are used to move around. Spacebar is used to jump.
//                        The player gets 3 lives and everytime they touch a ghost they lose a life and start the maze from the initial position. 



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
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Mainmenu.h"


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;


Scene* currentScene;
Scene* sceneList[3];

int currentlives;
bool won;
Mix_Music* menuMusic;
Mix_Music* levelMusic;
Mix_Chunk* wonGame;
Mix_Chunk* grassSound;
Mix_Chunk* jumpSound;
Mix_Chunk* lostGame;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("3D!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 1280, 720);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    menuMusic = Mix_LoadMUS("horrorBackground.mp3");
    levelMusic = Mix_LoadMUS("horrorLevel.mp3");
    wonGame = Mix_LoadWAV("wongame2.wav");
    Mix_PlayMusic(menuMusic, -1);
    grassSound = Mix_LoadWAV("grass1.wav");
    jumpSound = Mix_LoadWAV("jump.wav");
    lostGame = Mix_LoadWAV("lostGame.wav");
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
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

    sceneList[0] = new Mainmenu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();

    SwitchToScene(sceneList[0]);

    
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
            case SDLK_RETURN:
                if (currentScene->state.level == MENU) {
                    currentScene->state.nextScene = 1;
                    Mix_HaltMusic();
                    Mix_PlayMusic(levelMusic, -1);
                    
                }
                break;
            case SDLK_SPACE:
                if (currentScene->state.player->collidedBottom && currentScene->state.level != MENU) {
                    currentScene->state.player->jump = true;
                    Mix_PlayChannel(-1, jumpSound, 0);
                }
                break;
            }
            break;
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_A] && !(currentScene->state.level == MENU)) {
        currentScene->state.player->rotation.y += 1.0f;
    }
    else if (keys[SDL_SCANCODE_D] && !(currentScene->state.level == MENU)) {
        currentScene->state.player->rotation.y -= 1.0f;
    }

    currentScene->state.player->velocity.x = 0;
    currentScene->state.player->velocity.z = 0;

    if (keys[SDL_SCANCODE_W] && (currentScene->state.level == LEVEL1)) {
        currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * -3.0f;
        currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * -3.0f;
        Mix_PlayChannel(-1, grassSound, 0);
    }
    else if (keys[SDL_SCANCODE_S] && (currentScene->state.level == LEVEL1)) {
        currentScene->state.player->velocity.z = cos(glm::radians(currentScene->state.player->rotation.y)) * 3.0f;
        currentScene->state.player->velocity.x = sin(glm::radians(currentScene->state.player->rotation.y)) * 3.0f;
        Mix_PlayChannel(-1, grassSound, 0);
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
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;
    if (currentScene->state.player->life == 0) {

        currentScene->state.nextScene = 2;
    }
    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix,glm::radians(currentScene->state.player->rotation.y), glm::vec3(0, -1.0f, 0));
    viewMatrix = glm::translate(viewMatrix, -currentScene->state.player->position);
    if (currentScene->state.level == LEVEL2) {
        if (currentScene->state.player->life == 0) { 
                  
            Mix_PlayChannel(-1, lostGame, 0);
            currentScene->state.player->endMusic = false;
        }
        else {
            Mix_PlayChannel(-1, wonGame, 0);
            currentScene->state.player->endMusic = false;
        }
    }
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);
    currentScene->Render(&program);
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
        if (currentScene->state.nextScene >= 0) {
            currentlives = currentScene->state.player->life;
            won = currentScene->state.player->won;
            SwitchToScene(sceneList[currentScene->state.nextScene]);
            currentScene->state.player->life = currentlives;
            currentScene->state.player->won = won;
            
        }
        Render();
    }

    Shutdown();
    return 0;
}
