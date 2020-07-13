//INTRO TO GAME PROGRAMMING
//Name: Shakthi Shrree Ganta Gameshwaran
//Project Name: Rise of the AI
//Description of Project: It is a simple game with three different AI zombies. There is a virus turning people into zombies and you (the player) are tasked to bring the antedote which is behind the door. Press F to shoot at the enemies and spacebar to jump. Press right and left arrow keys to move right and left.
//                        Obtain the key to the door and reach the door to win the game. 




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


#define PLATFORM_COUNT 36
#define FAILURE_COUNT 8
#define SUCCESS_COUNT 7
#define ENEMY_COUNT 3
#define BULLET_COUNT 2
#define ENEMY_BULLET_COUNT 3

struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* failure;
    Entity* success;
    Entity* enemies;
    Entity* bullets;
    Entity* enemyBullet;
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
    displayWindow = SDL_CreateWindow("Zombie Apocalypse", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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
    state.player->position = glm::vec3(-4, -2, 0);
    state.player->speed = 1.0f;
    state.player->type = PLAYER;
    state.player->gravity = glm::vec3(0, -9.81f, 0);
    state.player->textureID = LoadTexture("character.png");

    state.player->animRight = new int[8]{ 24, 25, 26, 27, 28, 29, 30, 31 };
    state.player->animLeft = new int[8]{ 16, 17, 18, 19 , 20, 21, 22 ,23 };

    state.player->direction = RIGHT;
    state.player->animIndices = state.player->idleRight;
    state.player->animFrames = 8;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 8;
    state.player->animRows = 8;

    state.player->state = IDLE;
    state.player->jumpPower = 7.1f;

    state.player->height = 0.71f;
    state.player->width = 0.4f;

    //Initialise platform

    state.platforms = new Entity[PLATFORM_COUNT];
    GLuint platformTextureID = LoadTexture("ground.png");
    float x = -5.5f;
    float y = -3.25f;
    for (int i = 0; i < 10; i++) {
        x += 1.0f;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(x, y, 0);
        state.platforms[i].type = GROUND;
    }
    GLuint rightspikeID = LoadTexture("right_spike.png");
    for (int i = 10; i < 17; i++) {
        y += 1.0f;
        state.platforms[i].textureID = rightspikeID;
        state.platforms[i].position = glm::vec3(x, y, 0);
        state.platforms[i].type = OBSTACLE;
        state.platforms[i].height = 0.71f;
        state.platforms[i].width = 0.00f;
    }
    x = -4.5f;
    GLuint leftspikeID = LoadTexture("left_spike.png");
    for (int i = 17; i < 24; i++) {
        state.platforms[i].textureID = leftspikeID;
        state.platforms[i].position = glm::vec3(x, y, 0);
        y -= 1.0f;
        state.platforms[i].type = OBSTACLE;
        state.platforms[i].height = 0.71f;
        state.platforms[i].width = 0.00f;
    }
    state.platforms[24].textureID = platformTextureID;
    state.platforms[25].textureID = platformTextureID;
    state.platforms[24].position = glm::vec3(0, -1.25f, 0);
    state.platforms[25].position = glm::vec3(1, -1.25f, 0);
    state.platforms[24].type = GROUND;
    state.platforms[25].type = GROUND;
    state.platforms[24].width = 1.0f;
    state.platforms[25].width = 1.0f;
    state.platforms[24].height = 0.9f;
    state.platforms[25].height = 0.9f;

    GLuint keyTextureID = LoadTexture("key.png");
    state.platforms[26].textureID = keyTextureID;
    state.platforms[26].position = glm::vec3(3.5f, -2.25f, 0);
    state.platforms[26].type = KEY;
    state.platforms[26].state = IDLE;
    state.platforms[26].width = 0.5f;
    state.platforms[26].height = 0.5f;

    x = 1.0f;
    y = 1.25f;
    for (int i = 27; i < 30; i++) {
        x += 1.0f;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(x, y, 0);
        state.platforms[i].type = GROUND;
    }
    state.platforms[30].textureID = LoadTexture("door.png");
    state.platforms[30].position = glm::vec3(x, 2.25f, 0);
    state.platforms[30].type = DOOR;
    
    state.platforms[31].textureID = LoadTexture("doortop.png");
    state.platforms[31].position = glm::vec3(x, 3.25f, 0);
    state.platforms[31].type = DOOR;

    y = 0.25f;
    x = -1.0f;
    for (int i = 32; i < 35; i++) {
        x -= 1.0f;
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].position = glm::vec3(x, y, 0);
        state.platforms[i].type = GROUND;
    }
    state.platforms[35].textureID = platformTextureID;
    state.platforms[35].position = glm::vec3(0,2.25f , 0);
    state.platforms[35].type = GROUND;

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0, NULL, 0);
    }
    
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
    xpos = -1.2f;
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

    //Initialise Enemies
    
    state.enemies = new Entity[ENEMY_COUNT];
    GLuint enemyTextureID = LoadTexture("zombies-male.png");
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].animCols = 12;
        state.enemies[i].animRows = 8;
        state.enemies[i].animFrames = 4;
        state.enemies[0].animLeft = new int[4]{ 13,12,13,14 };
        state.enemies[0].animRight = new int[4]{ 25,24,25,26 };
        state.enemies[1].animLeft = new int[4]{ 16,15,16,17 };
        state.enemies[1].animRight = new int[4]{ 28,27,28,29 };
        state.enemies[i].direction = LEFT;
        state.enemies[i].animIndices = state.enemies[i].animLeft;
        state.enemies[i].animIndex = 0;
        state.enemies[i].speed = 0.2f;
        state.enemies[i].type = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].width = 0.6f;
        state.enemies[i].gravity = glm::vec3(0, -9.81f, 0);
        
    }
    state.enemies[0].position = glm::vec3(2, -3.0f, 0);
    state.enemies[1].position = glm::vec3(1, -0.25f, 0);
    state.enemies[0].enemyType = WALKER;
    state.enemies[1].enemyType = RAISER;
    state.enemies[0].state = WALK;
    state.enemies[1].state = IDLE;
    state.enemies[1].isActive = false;
    state.enemies[0].movement.x = -1.0f;
    state.enemies[2].position = glm::vec3(-4, 1.25f, 0);
    state.enemies[2].movement = glm::vec3(0);
    state.enemies[2].animFrames = 1;
    state.enemies[2].animIndex = 0;
    state.enemies[2].animIndices = new int[1]{ 73 };
    state.enemies[2].enemyType = SHOOTER;
    state.enemies[2].state = IDLE;
    //state.enemies[2].height = 0.8f;
    //Initialise bullets

    state.bullets = new Entity[BULLET_COUNT];
  
    for (int i = 0; i < BULLET_COUNT; i++) {
        state.bullets[i].isActive = false;
        state.bullets[i].state = IDLE;
        state.bullets[i].type = BULLET;
        state.bullets[i].speed = 1.5f;
        state.bullets[i].gravity = glm::vec3(0,-0.01,0);
        state.bullets[i].Update(0, NULL, 0);
    }

    //Initialise Enemybullets
    state.enemyBullet = new Entity[ENEMY_BULLET_COUNT];
    GLuint enemyBulletTextureID = LoadTexture("bullet_green.png");
    for (int i = 0; i < ENEMY_BULLET_COUNT;i++) {
        state.enemyBullet[i].isActive = false;
        state.enemyBullet[i].state = IDLE;
        state.enemyBullet[i].type = BULLET;
        state.enemyBullet[i].speed = 1.5f;
        state.enemyBullet[i].gravity = glm::vec3(0, -0.01, 0);
        state.enemyBullet[i].Update(0, NULL, 0);
        state.enemyBullet[i].movement.x = 1.0f;
        state.enemyBullet[i].textureID = enemyBulletTextureID;
        state.enemyBullet[i].height = 0.2f;
        state.enemyBullet[i].width = 0.2f;
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
                    state.player->state = JUMP;
                }
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_F] && state.player->state != DEAD) {
        state.player->state = SHOOT;
        state.player->animFrames = 6;
        if (state.player->direction == RIGHT) {
            state.player->animIndices = state.player->shootRight;
            for (int i = 0; i < BULLET_COUNT; i++) {
                if (state.bullets[i].isActive == false) {
                    state.bullets[i].isActive = true;
                    state.bullets[i].position = state.player->position + glm::vec3(0.5f, 0, 0);
                    state.bullets[i].movement.x = 1.0f;
                    state.bullets[i].state = SHOOT;
                    state.bullets[i].textureID = LoadTexture("bullet_right.png");
                }
            }

        }
        else {
            state.player->animIndices = state.player->shootLeft;
            for (int i = 0; i < BULLET_COUNT; i++) {
                if (state.bullets[i].isActive == false) {
                    state.bullets[i].isActive = true;
                    state.bullets[i].position = state.player->position - glm::vec3(0.5f, 0, 0);
                    state.bullets[i].movement.x = -1.0f;
                    state.bullets[i].state = SHOOT;
                    state.bullets[i].textureID = LoadTexture("bullet_left.png");
                }
            }
        }

    }
    else if (keys[SDL_SCANCODE_LEFT] && state.player->state != DEAD) {
        state.player->movement.x = -1.0f;
        state.player->animIndices = state.player->animLeft;
        state.player->direction = LEFT;
        state.player->state = WALK;
        state.player->animFrames = 8;
    }
    else if (keys[SDL_SCANCODE_RIGHT] && state.player->state != DEAD) {
        state.player->movement.x = 1.0f;
        state.player->animIndices = state.player->animRight;
        state.player->direction = RIGHT;
        state.player->state = WALK;
        state.player->animFrames = 8;
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
        if (fabs(state.player->position.x - state.enemies[1].position.x) <= 1.0f && state.enemies[1].isActive == false && state.enemies[1].state != DEAD && fabs(state.player->position.y - state.enemies[1].position.y) <= 0.5f) {
            state.enemies[1].isActive = true;
        }
        float xpos = state.player->position.x - state.enemies[1].position.x;
        if (xpos > 0) {
            state.enemies[1].movement.x = 0.5f;
            state.enemies[1].animIndices = state.enemies[1].animRight;
        }
        else {
            state.enemies[1].movement.x = -0.5f;
            state.enemies[1].animIndices = state.enemies[1].animLeft;
        }
        if (fabs(state.player->position.x - state.enemies[2].position.x) <= 3.0f && fabs(state.player->position.y - state.enemies[2].position.y) <= 2.0f && state.enemies[2].state != DEAD) {
            for (int i = 0; i < ENEMY_BULLET_COUNT; i++) {
                if (state.enemyBullet[i].isActive == false) {
                    state.enemyBullet[i].isActive = true;
                    state.enemyBullet[i].position = state.enemies[2].position + glm::vec3(0.5f, 0, 0);
                    state.enemyBullet[i].state = SHOOT;
                }
            }
        }
        state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        //state.player->Update(FIXED_TIMESTEP, state.enemies, ENEMY_COUNT);
        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, 1);
            state.enemies[i].Update(FIXED_TIMESTEP, state.bullets, BULLET_COUNT);
        }
        for (int i = 0; i < BULLET_COUNT; i++) {
            state.bullets[i].Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
        }
        for (int i = 0; i < ENEMY_BULLET_COUNT; i++) {
            state.enemyBullet[i].Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
            state.enemyBullet[i].Update(FIXED_TIMESTEP, state.player,1);
        }
        deltaTime -= FIXED_TIMESTEP;
    }
    accumulator = deltaTime;

}



void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    state.player->Render(&program);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].Render(&program);
    }

    if (state.player->wonGame) {
        for (int i = 0; i < SUCCESS_COUNT; i++) {
            state.success[i].Render(&program);
        }
    }

    for (int i = 0; i < BULLET_COUNT; i++) {
        state.bullets[i].Render(&program);
    }
    for (int i = 0; i < ENEMY_BULLET_COUNT; i++) {
        state.enemyBullet[i].Render(&program);
    }
    if (state.player->state == DEAD) {
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
