#include "Mainmenu.h"

#define GAME_NAME_COUNT 13
#define INSTRUCTION_COUNT 20

int* gamename_data = new int[GAME_NAME_COUNT] {51,68,32,80, 76, 65, 84, 70, 79, 82, 77, 69, 82};
int* instruction_data = new int[INSTRUCTION_COUNT] {80, 114, 101, 115, 115, 32, 69, 110, 116, 101, 114, 32, 84, 111, 32, 83, 116, 97, 114, 116};

void Mainmenu::Initialize() {
    state.level = MENU;
    state.nextScene = -1;
    state.player = new Entity();

    glClearColor(0.2f, 0.8f, 0.4f, 1.0f);

    //Game Name
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    state.gamename = new Entity[GAME_NAME_COUNT];

    float x = -3;

    for (int i = 0; i < GAME_NAME_COUNT; i++) {
        state.gamename[i].textureID = fontTextureID;
        state.gamename[i].position = glm::vec3(x, 1.0f, -8);
        state.gamename[i].animCols = 16;
        state.gamename[i].animRows = 16;
        state.gamename[i].animIndex = 0;
        state.gamename[i].animFrames = 1;
        state.gamename[i].animIndices = new int[1]{ gamename_data[i] };
        x += 0.45f;
        if (i > 3) x += 0.1f;
    }


    //Instructions
    x = -4.5f;

    state.instructions = new Entity[INSTRUCTION_COUNT];

    for (int i = 0; i < INSTRUCTION_COUNT; i++) {
        state.instructions[i].textureID = fontTextureID;
        state.instructions[i].position = glm::vec3(x,-1, -8);
        state.instructions[i].animCols = 16;
        state.instructions[i].animRows = 16;
        state.instructions[i].animIndex = 0;
        state.instructions[i].animFrames = 1;
        state.instructions[i].animIndices = new int[1]{ instruction_data[i] };
        x += 0.5f;
    }


}
void Mainmenu::Update(float deltaTime) {
    for (int i = 0; i < GAME_NAME_COUNT; i++) {
        state.gamename[i].Update(0,state.player,NULL,0);
    }
    for (int i = 0; i < INSTRUCTION_COUNT; i++) {
        state.instructions[i].Update(0, state.player, NULL, 0);
    }
}
void Mainmenu::Render(ShaderProgram* program) {
    
    for (int i = 0; i < GAME_NAME_COUNT; i++) {
        state.gamename[i].Render(program);
    }

    for (int i = 0; i < INSTRUCTION_COUNT; i++) {
        state.instructions[i].Render(program);
    }

}
