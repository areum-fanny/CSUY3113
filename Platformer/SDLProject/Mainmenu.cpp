#include "Mainmenu.h"

#define GAME_NAME_COUNT 10
#define INSTRUCTION_COUNT 20
#define MENU_WIDTH 26
#define MENU_HEIGHT 8
float red = 0;
float blue = 0;
float green = 0;
bool ascending = true;
unsigned int menu_data[] =
{
21,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,21,
21,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,21,
21,12,12,12,12,14,14,14,14,14,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,21,
21,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,21,
21,12,12,12,12,12,12,12,12,12,12,12,14,14,14,14,12,12,14,14,12,12,12,12,12,21,
21,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,21,
21,12,12,14,14,14,14,12,12,14,14,12,12,12,12,12,12,12,12,12,12,12,14,14,14,21,
21,4,4,17,17,17,17,4,4,17,17,4,4,4,4,4,4,4,4,4,4,4,17,17,17,21
};
int* gamename_data = new int[GAME_NAME_COUNT] {80,76,65,84,70,79,82,77,69,82};
int* instruction_data = new int[INSTRUCTION_COUNT] {80,114,101,115,115,32,69,110,116,101,114,32,84,111,32,83,116,97,114,116};

void Mainmenu::Initialize() {
    state.level = MENU;
    state.nextScene = -1;
    state.player = new Entity();
    GLuint mapTextureID = Util::LoadTexture("tilesheet.png");
    state.map = new Map(MENU_WIDTH, MENU_HEIGHT, menu_data, mapTextureID, 1.0f, 14, 7);
    glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
    //Game Name
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    state.gamename = new Entity[GAME_NAME_COUNT];
    
    float x = 2.0f;

    for (int i = 0; i < GAME_NAME_COUNT; i++) {
        state.gamename[i].textureID = fontTextureID;
        state.gamename[i].position = glm::vec3(x, -2.5f, 0);
        state.gamename[i].animCols = 16;
        state.gamename[i].animRows = 16;
        state.gamename[i].animIndex = 0;
        state.gamename[i].animIndices = new int[1]{ gamename_data[i] };
        x += 0.5f;
        if (i > 3) x += 0.1f;
    }
    

    //Instructions
    x = 1.0f;

    state.instructions = new Entity[INSTRUCTION_COUNT];

    for (int i = 0; i < INSTRUCTION_COUNT; i++) {
        state.instructions[i].textureID = fontTextureID;
        state.instructions[i].position = glm::vec3(x, -4.0f, 0);
        state.instructions[i].animCols = 16;
        state.instructions[i].animRows = 16;
        state.instructions[i].animIndex = 0;
        state.instructions[i].animIndices = new int[1]{ instruction_data[i] };
        x += 0.4f;
    }
    
    
}
void Mainmenu::Update(float deltaTime) {
    for (int i = 0; i < GAME_NAME_COUNT; i++) {
        state.gamename[i].Update(0, &state.gamename[i], NULL, 0, state.map);
    }
    for (int i = 0; i < INSTRUCTION_COUNT; i++) {
        state.instructions[i].Update(0, &state.instructions[i], NULL, 0, state.map);
    }
}
void Mainmenu::Render(ShaderProgram* program) {
    state.map->Render(program);
    for (int i = 0; i < GAME_NAME_COUNT; i++) {
        state.gamename[i].Render(program);
    }

   
    for (int i = 0; i < INSTRUCTION_COUNT; i++) {
        state.instructions[i].Render(program);
    }
   
}
