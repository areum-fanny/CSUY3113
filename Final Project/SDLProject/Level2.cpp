#include "Level2.h"

void Level2::Initialize() {

    state.level = LEVEL2;
    state.nextScene = -1;
    glClearColor(0.8f, 0.2f, 0.2f, 1.0f);

    state.player = new Entity();
    //Failure
    float x = -1.7f;
    state.lose = new Entity[FAILURE_COUNT];
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    for (int i = 0;i < FAILURE_COUNT;i++) {
        state.lose[i].textureID = fontTextureID;
        state.lose[i].animCols = 16;
        state.lose[i].animRows = 16;
        state.lose[i].animIndex = 0;
        state.lose[i].animIndices = new int[1]{ state.failed[i] };
        state.lose[i].position = glm::vec3(x, 0, -8);
        x += 0.5f;
    }
    //Success
    state.win = new Entity[SUCCESS_COUNT];
    x = -1.5f;
    for (int i = 0; i < SUCCESS_COUNT;i++) {
        state.win[i].textureID = fontTextureID;
        state.win[i].animCols = 16;
        state.win[i].animRows = 16;
        state.win[i].animIndex = 0;
        state.win[i].animIndices = new int[1]{ state.success[i] };
        state.win[i].position = glm::vec3(x, 0, -8);
        x += 0.5f;
    }
    for (int i = 0; i < SUCCESS_COUNT; i++) {
        state.win[i].Update(0, state.player, state.win, SUCCESS_COUNT);
    }
    for (int i = 0; i < FAILURE_COUNT; i++) {
        state.lose[i].Update(0,state.player, state.lose, SUCCESS_COUNT);
    }

}
void Level2::Update(float deltaTime) {
}
void Level2::Render(ShaderProgram* program) {
    if (state.player->won) {
        for (int i = 0; i < SUCCESS_COUNT; i++) {
            state.win[i].Render(program);
        }
        
    }
    else {
        for (int i = 0; i < FAILURE_COUNT; i++) {
            state.lose[i].Render(program);
        }
    }  
}