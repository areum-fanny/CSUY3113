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
#define PLATFORM_COUNT 27
#include "stb_image.h"

#include "Entity.h"
#define ENEMY_COUNT 1
#define FAILURE_COUNT 8
#define SUCCESS_COUNT 7
#define BULLET_COUNT 10
struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* bullets;
    Entity* enemies;
    Entity* failure;                   
    Entity* success;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
int* failed = new int[8]{ 89,111,117,32,76,111,115,101 };      //Holds indices of "Mission Failed :("
int* success = new int[7]{ 89,111,117,32,87,105,110 }; //Holds indices of "Mission Successful :)"
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
    displayWindow = SDL_CreateWindow("Textured!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    state.player->position = glm::vec3(-3,-2,0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->velocity = glm::vec3(0);
    state.player->speed = 2.0f;
    state.player->height = 0.7f;
    state.player->width = 0.4f;
    state.player->textureID = LoadTexture("character.png");
    state.player->type = PLAYER;
    state.player->animRight = new int[9]{ 8, 24, 25, 26, 27, 28, 29, 30, 31 };
    state.player->animLeft = new int[9]{ 0,16, 17, 18, 19 , 20, 21, 22 ,23 };
    //int* idle_right = new int[5]{ 8,9,10,11,12 };
    //int* idle_left = new int[5]{ 0,1,2,3,4 };
    

    state.player->direction = RIGHT;
    state.player->state = IDLE;

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 9;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 8;
    state.player->animRows = 8;

    state.player->jumpPower = 5.0f;

    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID = LoadTexture("ground.png");
    float x = -5.5f;
    float y = -3.25f;
    for (int i = 0; i < 10; i++) {
        x += 1.0f;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(x, y, 0);
        state.platforms[i].type = GROUND;
        state.platforms[i].width = 1.0f;
    }
    GLuint rightspikeID = LoadTexture("right_spike.png");
    for (int i = 10; i < 17; i++) {
        y += 1.0f;
        state.platforms[i].textureID = rightspikeID;
        state.platforms[i].position = glm::vec3(x, y, 0);
        state.platforms[i].type = SPIKE;
        state.platforms[i].width = 0.2f;
        
    }
    x = -4.5f;
    GLuint leftspikeID = LoadTexture("left_spike.png");
    for (int i = 17; i < 24; i++) {
        state.platforms[i].textureID = leftspikeID;
        state.platforms[i].position = glm::vec3(x, y, 0);
        y -= 1.0f;
        state.platforms[i].type = SPIKE;
        state.platforms[i].width = 0.2f;
    }
    
    state.platforms[24].textureID = platformTextureID;
    state.platforms[25].textureID = platformTextureID;
    state.platforms[24].position = glm::vec3(-1, -1.25f, 0);
    state.platforms[25].position = glm::vec3(0, -1.25f, 0);
    state.platforms[24].type = GROUND;
    state.platforms[25].type = GROUND;
    state.platforms[24].width = 1.0f;
    state.platforms[25].width = 1.0f;

    GLuint keyTextureID = LoadTexture("key.png");
    state.platforms[26].textureID = keyTextureID;
    state.platforms[26].position = glm::vec3(3.5f, -2.25f, 0);
    state.platforms[26].type = KEY;
    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0, NULL, 0);
        
    }


    //Initialise Enemies

    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = LoadTexture("ctg.png");
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(3.5, -2.25f, 0);
    state.enemies[0].width = 0.5f;
    state.enemies[0].type = ENEMY;
    state.enemies[0].aiType = WALKER;
    state.enemies[0].aiState = WALKING;
    state.enemies[0].speed = 0.5f;

    float xpos = -1.6f;
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
    xpos = -0.8f;
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

    //Initialise bullets

    state.bullets = new Entity[BULLET_COUNT];
    GLuint rightbulletTextureID = LoadTexture("bullet_right.png");
    GLuint leftbulletTextureID = LoadTexture("bullet_left.png");
    for (int i = 0; i < BULLET_COUNT; i++) {
        state.bullets[i].isActive = false;
        state.bullets[i].state = IDLE;
        state.bullets[i].type = BULLET;
        
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

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:
                if (state.player->collidedBottom) { 
                    state.player->jump = true;
                }
                else {
                    state.player->jump = false;
                }
                
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);


    if (keys[SDL_SCANCODE_F]) {
        state.player->state = SHOOT;
        state.player->animFrames = 6;
        if (state.player->direction == RIGHT) {
            state.player->animIndices = state.player->shootright;
            for (int i = 0; i < BULLET_COUNT; i++) {
                if (state.bullets[i].state == IDLE) {
                    state.bullets[i].isActive = true;
                    state.bullets[i].position = state.player->position + glm::vec3(1, 0, 0);
                    state.bullets[i].movement.x = 1.0f;
                    state.bullets[i].state = SHOOT;
                    state.bullets[i].textureID = LoadTexture("bullet_right.png");
                    //state.bullets[i].speed = 1.0f;
                }
            }

        }
        else {
            state.player->animIndices = state.player->shootleft;
            for (int i = 0; i < BULLET_COUNT; i++) {
                if (state.bullets[i].state == IDLE) {
                    state.bullets[i].isActive = true;
                    state.bullets[i].position = state.player->position - glm::vec3(1, 0, 0);
                    state.bullets[i].movement.x = -1.0f;
                    state.bullets[i].state = SHOOT;
                    state.bullets[i].textureID = LoadTexture("bullet_left.png");
                    //state.bullets[i].speed = 1.0f;
                }
            }
        }
    }
    else if (keys[SDL_SCANCODE_LEFT] && state.player->state != DEATH){
        state.player->state = IDLE;
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
        state.player->animFrames = 9;
        state.player->direction = LEFT;
    }
    else if (keys[SDL_SCANCODE_RIGHT] && state.player->state != DEATH) {
        state.player->state = IDLE;
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
        state.player->direction = RIGHT;
        state.player->animFrames = 9;
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
        state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        state.player->Update(FIXED_TIMESTEP, state.enemies, ENEMY_COUNT);
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        }
        for (int i = 0; i < BULLET_COUNT; i++) {
            state.bullets[i].Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
            state.bullets[i].Update(FIXED_TIMESTEP, state.enemies, ENEMY_COUNT);
        }
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;

}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);


    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    state.player->Render(&program);
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }
    for (int i = 0; i < BULLET_COUNT; i++) {
        state.bullets[i].Render(&program);
    }
    if (state.player->state == WON) {                 
        for (int i = 0; i < SUCCESS_COUNT; i++) {
            state.success[i].Render(&program);
        }
    }

    if (state.player->state == DEATH) {
        for (int i = 0; i < 8; i++) {
            state.player->animIndex = i;
            if (state.player->direction == RIGHT) {
                state.player->animIndices = state.player->death_right;
            }
            else {
                state.player->animIndices = state.player->death_left;
            }
            state.player->Render(&program);
        }
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
