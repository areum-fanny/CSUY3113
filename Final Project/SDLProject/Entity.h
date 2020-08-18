#pragma once
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
#include "Mesh.h"
#include <SDL_mixer.h>
enum EntityType { FLOOR, PLAYER, HEDGE, ENEMY, KEY, WALL, DOOR};
enum EnemyType { ENEMY1, ENEMY2, ENEMY3, ENEMY4, ENEMY5 };

class Entity {
public:
    EntityType entityType;
    EnemyType enemyType;
    glm::vec3 rotation;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 scale;
    bool jump;
    float jumpPower;
    float speed;

    bool billboard;
    float width;
    float height;
    float depth;
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
    bool collidedFront;
    bool collidedBack;
    GLuint textureID;
    int* animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;
    glm::mat4 modelMatrix;
    Mesh* mesh;
    int life = 3;
    bool won = false; 
    Mix_Chunk* enemyCollision;
    Mix_Chunk* keySound;
    bool endMusic = true;
    bool key = false;
    bool isActive = true;
    Entity();
    bool CheckCollision(Entity* other);
    void Update(float deltaTime, Entity* player, Entity* objects, int objectCount);
    void Render(ShaderProgram* program);
    void DrawBillboard(ShaderProgram* program);
    bool CheckCollisionX(Entity* object);
    bool CheckCollisionY(Entity* object);
    bool CheckCollisionZ(Entity* object);
    void AI();
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);

};



