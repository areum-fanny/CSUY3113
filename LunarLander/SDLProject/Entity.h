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

class Entity {
public:
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    float speed = 0;

    GLuint textureID;
    bool isdestination = false;                     //Keeps track of the water
    bool reacheddestination = false;                //Initialized to keep track of the nature of collision that is if the collision was with water/destination
    bool collidedwithrock = false;                  //Initialized to keep track of collisions with the buildings/obstacles
    glm::mat4 modelMatrix;
    int* animIndices = NULL;

    int animIndex = 0;
    int animCols = 0;
    int animRows = 0;
    float width = 1.0f;
    float height = 1.0f;

    Entity();
    bool CheckCollision(Entity* other);
    void Update(float deltaTime, Entity* rocks, int rocksCount);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
};
