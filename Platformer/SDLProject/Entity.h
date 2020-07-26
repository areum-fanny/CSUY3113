#pragma once 
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL_mixer.h>
#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"
enum EntityType {PLAYER, KEY, DOOR, WATER, ENEMY};
enum AIType {WALKER, WALKER2};
enum AIState { WALKING, IDLE, ATTACK };
enum Direction { RIGHT, LEFT };
enum EnemyLevel { ROUND1,ROUND2, ROUND3 };
class Entity {
public:
    EntityType type;
    AIType aiType;
    AIState aiState;
    EnemyLevel enemylevel;
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    Direction direction;
    float width = 1.0f;
    float height = 1.0f;

    float speed;
    int life = 3;
    GLuint textureID;

    glm::mat4 modelMatrix;

    int* animRight = NULL;
    int* animLeft = NULL;
    int* animUp = NULL;
    int* animDown = NULL;

    bool jump = false; 
    float jumpPower = 0;

    bool isActive = true;
    bool key = false; 
    bool won = false; 
    bool dead = false; 
    int* animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;

    bool collidedTop;
    bool collidedBottom;
    bool collidedRight;
    bool collidedLeft;
    bool endmusic = true;
    Mix_Chunk* jumpMusic;
    Mix_Chunk* keySound;
    Mix_Chunk* gameoverMusic;
    Mix_Chunk* successMusic;
    Mix_Chunk* loseMusic;
    Mix_Chunk* doorMusic;
    
    Entity();

    void Update(float deltaTime, Entity* player, Entity* objects, int objectFCount, Map* map);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckCollisionsX(Map* map);
    void CheckCollisionsY(Map* map);
    void AI();
    void AIWalker();
    void AIWalker2();
};
