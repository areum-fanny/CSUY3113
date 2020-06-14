
//Intro to Game Programming
//Name: Shakthi Shrre Ganta Gameshwaran
//Project Name: Pong
//Project Description: Game of Pong. To start the game press spacebar. Player 1 is on left-hand side and can use the keys 'W' and 'S' to move up and down.
//                     Player 2 is on the right-hand side and can use arrow Up key and arrow Down key to move up and down. 


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

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, projectionMatrix, player1Matrix, player2Matrix, pongMatrix;


glm::vec3 player1_position = glm::vec3(-4.75, 0, 0);
glm::vec3 player1_movement = glm::vec3(0, 0, 0);

glm::vec3 player2_position = glm::vec3(4.75, 0, 0);
glm::vec3 player2_movement = glm::vec3(0, 0, 0);

glm::vec3 pong_position = glm::vec3(0, 0, 0);
glm::vec3 pong_movement = glm::vec3(0, 0, 0);


float player_speed = 1.0f;
float pong_speed = 2.0f;
float pong_direction = 0; //Up or Down (Up = 1 & Down = -1)
float pong_direction2 = 0;  //To Left or To Right (Left = -1 & Right = 1)
float game_status = 0; //Game status is 0 if game hasn't started. If game is running game_status is 1. If game has ended game_status = 2;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void ProcessInput() {

    player1_movement = glm::vec3(0);
    player2_movement = glm::vec3(0);
    pong_movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_SPACE:                        //The game starts when the space bar is pressed down. 
                if (game_status == 0) {
                    game_status = 1;
                    pong_direction = 1;
                    pong_direction2 = 1;
                }
                break;
            }
            break;
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_UP]) {
        player2_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        player2_movement.y = -1.0f;
    }
    if (keys[SDL_SCANCODE_W]) {
        player1_movement.y = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        player1_movement.y = -1.0f;
    }
    if (glm::length(pong_movement) > 1.0f) {
        pong_movement = glm::normalize(pong_movement);
    }
}
float lastTicks = 0.0f;
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    if (game_status == 1) {
        player1_position += player1_movement * player_speed * deltaTime;
        player2_position += player2_movement * player_speed * deltaTime;

        if (player1_position.y > 0.75f) {
            player1_position.y = 0.75f;
        }
        if (player1_position.y < -0.75f) {
            player1_position.y = -0.75f;
        }
        if (player2_position.y > 0.75f) {
            player2_position.y = 0.75f;
        }
        if (player2_position.y < -0.75f) {
            player2_position.y = -0.75f;
        }

        if (pong_direction == 1) {  //Moving in the positive direction of Y axis
            pong_movement.y = 1.0f;
        }
        else {
            pong_movement.y = -1.0f; //Moving in the negative direction of y axis

        }
        if (pong_direction2 == 1) { //Moving in the positive direction of x axis
            pong_movement.x = 1.0f;
        }
        else {
            pong_movement.x = -1.0f; //Moving in the negative direction of x axis
        }
        pong_position += pong_movement * pong_speed * deltaTime;

        if (pong_position.y > 3.50f) {
            pong_direction = -1.0f;
        }
        if (pong_position.y < -3.50f) {
            pong_direction = 1.0f;
        }
        float player1y = player1_position.y * 3.75f;
        float player2y = player2_position.y * 3.75f;
        float pongplayer2xdist = fabs(player2_position.x - pong_position.x) - ((0.5f + 0.5f) / 2.0f);
        float pongplayer1xdist = fabs(player1_position.x - pong_position.x) - ((0.5f + 0.5f) / 2.0f);
        float pongplayer1ydist = fabs(player1y - pong_position.y) - ((0.5f + 1.875f) / 2.0f);
        float pongplayer2ydist = fabs(player2y - pong_position.y) - ((0.5f + 1.875f) / 2.0f);

        if (pong_position.x > 4.75f || pong_position.x < -4.75f) {    //If pong touches either side then the game ends
            game_status = 2;
        }
        if (pongplayer1xdist <= 0 && pongplayer1ydist <= 0) {
            pong_direction2 = 1.0f;
        }
        if (pongplayer2xdist <= 0 && pongplayer2ydist <= 0) {
            pong_direction2 = -1.0f;
        }

    }

    player1Matrix = glm::mat4(1.0f);
    player1Matrix = glm::scale(player1Matrix, glm::vec3(1.0f, 3.75f, 1.0f));
    player1Matrix = glm::translate(player1Matrix, player1_position);

    player2Matrix = glm::mat4(1.0f);
    player2Matrix = glm::scale(player2Matrix, glm::vec3(1.0f, 3.75f, 1.0f));
    player2Matrix = glm::translate(player2Matrix, player2_position);

    pongMatrix = glm::mat4(1.0f);
    pongMatrix = glm::scale(pongMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
    pongMatrix = glm::translate(pongMatrix, pong_position);

}
void displayplayer1() {
    program.SetModelMatrix(player1Matrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void displayplayer2() {
    program.SetModelMatrix(player2Matrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void displaypong() {
    program.SetModelMatrix(pongMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);


    float vertices[] = { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);


    displayplayer1();
    displayplayer2();
    displaypong();


    glDisableVertexAttribArray(program.positionAttribute);
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}

