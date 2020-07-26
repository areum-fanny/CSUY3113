#include "Level2.h"
#define LEVEL2_WIDTH 26
#define LEVEL2_HEIGHT 8
#define LEVEL2_ENEMY_COUNT 1
#define LEVEL2_WATER_COUNT 14

unsigned int level2_data[] =
{
21,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,21,
21,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,21,
21,12,12,12,12,12,12,12,12,12,12,14,14,14,12,12,12,12,14,14,14,12,12,12,12,21,
21,12,12,12,12,14,14,14,14,14,12,12,12,12,12,12,12,14,17,17,17,12,12,12,12,21,
21,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,21,
21,12,12,14,14,12,12,12,12,12,12,12,12,14,14,14,12,12,12,12,12,12,12,12,12,21,
21,14,14,17,17,12,12,12,12,12,12,12,12,17,17,17,12,12,12,12,12,12,14,14,14,21,
21,17,17,17,17,12,12,12,12,12,12,12,12,17,17,17,12,12,12,12,12,12,17,17,17,21
};

void Level2::Initialize() {
 
    state.level = LEVEL2;
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 14, 7);
    // Initialize Player
    state.player = new Entity();
    state.player->type = PLAYER;
    state.player->position = glm::vec3(2, -3, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 1.8f;
    state.player->height = 0.8f;
    state.player->width = 0.6f;
    state.player->textureID = Util::LoadTexture("george_0.png");
    state.player->jumpPower = 6.8f;

    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    //Lives
    float x = 1.0f;
    state.lives = new Entity[3];
    GLuint lifeTextureID =  Util::LoadTexture("Life.png");
    for (int i = 0; i < 3; i++) {
        state.lives[i].textureID = lifeTextureID;
        state.lives[i].position = glm::vec3(x, -0.5, 0);
        x += 0.5f;
    }
    
    //Key
    state.key = new Entity();
    GLuint keyTextureID = Util::LoadTexture("key.png");
    state.key->textureID = keyTextureID;
    state.key->position = glm::vec3(12, -1, 0);
    state.key->type = KEY;
    state.key->width = 0.5f;
    state.key->height = 0.5f;
    state.key->Update(0, NULL, NULL, 0, state.map);
    //Door
    state.doors = new Entity[2];
    state.doors[0].textureID = Util::LoadTexture("doortop.png");
    state.doors[0].position = glm::vec3(24, -4, 0);
    state.doors[1].textureID = Util::LoadTexture("door.png");
    state.doors[1].position = glm::vec3(24, -5, 0);

    for (int i = 0; i < 2; i++) {
        state.doors[i].type = DOOR;
        state.doors[i].Update(0, NULL, NULL, 0, state.map);
    }
    //Water
    x = 5.0f;
    state.obstacles = new Entity[LEVEL2_WATER_COUNT];
    GLuint waterTextureID = Util::LoadTexture("water.png");
    for (int i = 0; i < LEVEL2_WATER_COUNT;i++) {
        if (i == 8) x = 16;
        state.obstacles[i].textureID = waterTextureID;
        state.obstacles[i].position = glm::vec3(x, -7, 0);
        state.obstacles[i].type = WATER;
        x += 1.0f;
        
    }
    for (int i = 0; i < LEVEL2_WATER_COUNT;i++) {
        state.obstacles[i].Update(0, NULL, NULL, 0, state.map);
    }

    //Enemies
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    state.enemies[0].textureID = Util::LoadTexture("zombies-male.png");
    state.enemies[0].position = glm::vec3(6, -1, 0);
    state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies[0].animLeft = new int[4]{ 13,12,13,14 };
    state.enemies[0].animRight = new int[4]{ 25,24,25,26 };
    state.enemies[0].animCols = 12;
    state.enemies[0].animRows = 8;
    state.enemies[0].animFrames = 4;
    state.enemies[0].animIndices = state.enemies[0].animRight;
    state.enemies[0].animIndex = 0;
    state.enemies[0].speed = 0.2f;
    state.enemies[0].type = ENEMY;
    state.enemies[0].aiType = WALKER;
    state.enemies[0].aiState = WALKING;
    state.enemies[0].speed = 1.0f;
    state.enemies[0].direction = RIGHT;
    state.enemies[0].width = 0.6f;
    state.enemies[0].enemylevel = ROUND2;

    //Failure
    float xpos = 3.3f;
    state.lose = new Entity[FAILURE_COUNT];
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    for (int i = 0;i < FAILURE_COUNT;i++) {
        state.lose[i].textureID = fontTextureID;
        state.lose[i].animCols = 16;
        state.lose[i].animRows = 16;
        state.lose[i].animIndex = 0;
        state.lose[i].animIndices = new int[1]{state.failed[i]};
        state.lose[i].position = glm::vec3(xpos, -3.5f, 0.0f);
        xpos += 0.5f;
    }
   
   

}
void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    if (state.player->won) {
        state.nextScene = 3;
    }
    for (int i = 0; i < LEVEL2_WATER_COUNT;i++) {
        if(state.player->dead == false) state.obstacles[i].Update(deltaTime, &state.obstacles[i], state.player,1, state.map);
    }
    for (int i = 0; i < LEVEL2_ENEMY_COUNT;i++) {
        state.enemies[i].Update(deltaTime, &state.enemies[i], state.obstacles, LEVEL2_WATER_COUNT, state.map);
    }
    state.key->Update(deltaTime, state.key, state.player, 1, state.map);
    state.doors[0].Update(deltaTime, &state.doors[0], state.player, 1, state.map);
    state.doors[1].Update(deltaTime, &state.doors[1], state.player, 1, state.map);
    for (int i = 0; i < 3; i++) { state.lives[i].Update(0, &state.lives[i], NULL, 0, state.map); }
    for (int i = 0; i < FAILURE_COUNT; i++) { state.lose[i].Update(0, &state.lose[i], NULL, 0, state.map); }
    
}
void Level2::Render(ShaderProgram* program) {
    
    state.player->Render(program);
    state.map->Render(program);
    state.enemies[0].Render(program);
    for (int i = 0; i < state.player->life; i++) {
        state.lives[i].Render(program);
    }
    for (int i = 0; i < 2; i++) {
        state.doors[i].Render(program);
    }
    for (int i = 0; i < LEVEL2_WATER_COUNT;i++) {
        state.obstacles[i].Render(program);
    }
    if (state.player->life == 0) {
        if (state.player->endmusic) {
            Mix_PlayChannel(-1, state.player->gameoverMusic, 0);
            state.player->endmusic = false;
        }
        for (int i = 0; i < FAILURE_COUNT; i++) {
            state.lose[i].Render(program);
        }
   }
    state.key->Render(program);
}