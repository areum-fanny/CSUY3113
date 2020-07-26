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
#include "Util.h"
#include "Entity.h"
#include "Map.h"
#define SUCCESS_COUNT 7
#define FAILURE_COUNT 8

enum Level { MENU, LEVEL1, LEVEL2, LEVEL3 };

struct GameState {
	Map* map;
	Entity* player;
	Entity* enemies;
	Entity* lives;
	Entity* obstacles;
	Entity* key;
	Entity* doors;
	Entity* win;
	Entity* lose;
	Entity* gamename;
	Entity* instructions;
	Level level;
	int nextScene;
	int* failed = new int[8]{ 89,111,117,32,76,111,115,101 };      //Holds indices of "You Lose"
	int* success = new int[7]{ 89,111,117,32,87,105,110 }; //Holds indices of "You Win"

};
class Scene {
public:
	GameState state;
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ShaderProgram* program) = 0;
};

