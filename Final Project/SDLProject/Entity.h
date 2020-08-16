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
enum EntityType { FLOOR, PLAYER, PLATFORM,  CUBE, SHIP, CRATE, WALL};
enum Type { ENEMY };
class Entity {
public:
    EntityType entityType;
    Type type;
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

    glm::mat4 modelMatrix;
    Mesh* mesh;

    Entity();
    bool CheckCollision(Entity* other);
    void CheckCollisionX(Entity* other, int objectCount);
    void CheckCollisionY(Entity* other, int objectCount);
    void CheckCollisionZ(Entity* other, int objectCount);
    void Update(float deltaTime, Entity* player, Entity* objects, int objectCount);
    void Render(ShaderProgram* program);
    void DrawBillboard(ShaderProgram* program);
    bool CheckCollisionX(Entity* object);
    bool CheckCollisionY(Entity* object);
    bool CheckCollisionZ(Entity* object);
};



