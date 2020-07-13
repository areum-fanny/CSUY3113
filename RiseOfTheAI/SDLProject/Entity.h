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


enum EntityType { PLAYER, GROUND, ENEMY, OBSTACLE, KEY, DOOR, BULLET };
enum Direction { RIGHT, LEFT };
enum State { IDLE, WALK, DEAD, JUMP, SHOOT };
enum EnemyType { WALKER, RAISER, SHOOTER };

class Entity {
    public:

        EntityType type;
        Direction direction;
        State state;
        EnemyType enemyType;
        glm::vec3 position;
        glm::vec3 movement;
        glm::vec3 gravity;
        glm::vec3 velocity;

        float speed;

        GLuint textureID;

        glm::mat4 modelMatrix;

        int* animRight = NULL;
        int* animLeft = NULL;
        int* idleRight = new int[1]{8};
        int* idleLeft = new int[1]{0};
        int* shootRight = new int[6]{ 8,40,41,42,43,44 };
        int* shootLeft = new int[6]{ 0,32,33,34,35,36 };
        int* death_right = new int[8]{ 56,57,58,59,60,61,62,63 };
        int* death_left = new int[8]{ 48,49,50,51,52,53,54,55 };

        int* animIndices = NULL;
        int animFrames = 0;
        int animIndex = 0;
        float animTime = 0;
        int animCols = 0;
        int animRows = 0;

        float jumpPower = 0;
        float height = 1.0f;
        float width = 1.0f;

        bool collidedLeft = false;
        bool collidedRight = false;
        bool collidedTop = false;
        bool collidedBottom = false;
        bool isActive = true;
        bool key = false;
        bool wonGame = false;
        Entity();
        void Update(float deltaTime, Entity* platforms, int platformcount);
        void Render(ShaderProgram* program);
        void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);
        bool CheckCollision(Entity* other);
        void CheckCollisionsY(Entity* objects, int objectCount);
        void CheckCollisionsX(Entity* objects, int objectCount);
        void AI();
        void AIWalker();
        void AIRaiser();
    };

