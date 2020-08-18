//Name: Shakthi Shrree Ganta Gameshwaran
//Project Name: Platformer
//Description of Project: A simple platformer game with walking AI. The story of the game is that due to global warming all lands are flooded and there are new zombie like creatures roaming outside.
//                        The player has lost their keys outside and nead to retrieve the three keys to get back home. Right and left arrow keys control the direction of movement and spacebar jumps.
//                        It's not possible to kill the zombie like creatures so the player should avoid the creatures and retrieve the keys. 

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"


#include "Entity.h"
#include "Map.h"
#include "Util.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Mainmenu.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[4];
int currentlives;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

Mix_Music* menuMusic;
Mix_Music* levelMusic;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    menuMusic = Mix_LoadMUS("dooblydoo.mp3");
    levelMusic = Mix_LoadMUS("crypto.mp3");
    Mix_PlayMusic(menuMusic, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    sceneList[0] = new Mainmenu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();

    currentlives = 3;

    SwitchToScene(sceneList[0]);

    currentScene->state.player->life = currentlives;
}

void ProcessInput() {

    currentScene->state.player->movement = glm::vec3(0);

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
                    SwitchToScene(sceneList[currentScene->state.nextScene]);
                    Mix_HaltMusic();
                    Mix_PlayMusic(levelMusic, -1);
                }
                break;
            case SDLK_SPACE:
                if (currentScene->state.player->collidedBottom && currentScene->state.level != MENU && !currentScene->state.player->won) {
                    currentScene->state.player->jump = true;
                }
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_LEFT] && !currentScene->state.player->won) {
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT] && !currentScene->state.player->won) {
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }


    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }

}


#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
void Update() {
    currentlives = currentScene->state.player->life;
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
    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player->dead && currentScene->state.player->life > 0) {
        currentScene->state.player->dead = false;
        currentScene->state.player->key = false;
        currentScene->state.key->isActive = true; 
        if (currentScene->state.level == LEVEL1) { 
            currentScene->state.player->position = glm::vec3(1.5f, -5, 0); 
            currentScene->state.enemies[0].position = glm::vec3(1, -1, 0);
        }
        else if (currentScene->state.level == LEVEL2) { currentScene->state.player->position = glm::vec3(2, -3, 0); }
        else { currentScene->state.player->position = glm::vec3(3, -5, 0); }
        float xpos = 1.0f;
        for (int i = 0; i < currentlives; i++) {
            currentScene->state.lives[i].position = glm::vec3(xpos, -0.5f, 0);
            xpos += 0.5f;
        }
        xpos = 3.3f;
        for (int i = 0; i < FAILURE_COUNT; i++) {
            currentScene->state.lose[i].position = glm::vec3(xpos, -3.5f, 0);
            xpos += 0.5f;
        }
        if (currentScene->state.level == LEVEL3) {
            xpos = 3.3f;
            for (int i = 0; i < SUCCESS_COUNT; i++) {
                currentScene->state.win[i].position = glm::vec3(xpos, -3.5f, 0);
                xpos += 0.5f;
            }
        }
    }
    if (currentScene->state.player->position.x > 20.5f) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-20.5f, 3.75, 0));
        float xpos = 16.5f;
        for (int i = 0; i < currentlives; i++) {
            currentScene->state.lives[i].position = glm::vec3(xpos, -0.5f, 0);
            xpos += 0.5f;
        }
        xpos = 18.5f;
        for (int i = 0; i < FAILURE_COUNT; i++) {
            currentScene->state.lose[i].position = glm::vec3(xpos, -3.5f, 0);
            xpos += 0.5f;
        }
        if (currentScene->state.level == LEVEL3) {
            xpos = 18.5f;
            for (int i = 0; i < SUCCESS_COUNT; i++) {
                currentScene->state.win[i].position = glm::vec3(xpos, -3.5f, 0);
                xpos += 0.5f;
            }
        }
    }
    
    else if (currentScene->state.player->position.x > 4.5f) {
        viewMatrix = glm::translate(viewMatrix,glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
        for (int i = 0; i < currentlives; i++) {
            currentScene->state.lives[i].position.x += currentScene->state.player->velocity.x * FIXED_TIMESTEP;
        }
        for (int i = 0; i < FAILURE_COUNT; i++) {
            currentScene->state.lose[i].position.x += currentScene->state.player->velocity.x * FIXED_TIMESTEP;
        }
        if (currentScene->state.level == LEVEL3) {
            for (int i = 0; i < SUCCESS_COUNT; i++) {
                currentScene->state.win[i].position.x += currentScene->state.player->velocity.x * FIXED_TIMESTEP;
            }
        }
    }
    else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-4.5f, 3.75, 0));
    }
    if (currentScene->state.player->position.y > 0) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(0, -currentScene->state.player->position.y - currentScene->state.player->position.y, 0));
    }
    if (currentScene->state.level == LEVEL3 && currentScene->state.player->won) {
        Mix_HaltMusic();
    }
    
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

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
            SwitchToScene(sceneList[currentScene->state.nextScene]);
            currentScene->state.player->life = currentlives;
        }
        Render();
    }

    Shutdown(
        
    );
    return 0;
}
