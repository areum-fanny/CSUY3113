#include "Level1.h"

#define OBJECT_COUNT 1001


void Level1::Initialize() {

    state.level = LEVEL1;
    state.nextScene = -1;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 4.5f, 18.5f);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 1.0f;
    state.player->jump = false;
    state.player->jumpPower = 6.5f;
    state.player->height = 3.0f;
    state.player->width = 0.5f;
    state.player->depth = 0.5f;

    state.objects = new Entity[OBJECT_COUNT];

    Mesh* cubeMesh = new Mesh();
    cubeMesh->LoadOBJ("cube.obj", 40);

    GLuint floorTextureID = Util::LoadTexture("grass1.jpg");
    state.objects[0].textureID = floorTextureID;
    state.objects[0].mesh = cubeMesh;
    state.objects[0].rotation = glm::vec3(0, 0, 0);
    state.objects[0].position = glm::vec3(0, -0.5f, 0);
    state.objects[0].entityType = FLOOR;
    state.objects[0].acceleration = glm::vec3(0, 0, 0);
    state.objects[0].scale = glm::vec3(40, 0.5f, 40);
    state.objects[0].height = 0.5f; 
    state.objects[0].width = 40.0f;
    state.objects[0].depth = 40.0f;


    GLuint wallTextureID = Util::LoadTexture("nightsky2.jfif");

    Mesh* wallMesh = new Mesh();
    wallMesh->LoadOBJ("cube.obj", 20);
    for (int i = 1; i < 5; i++) {
        state.objects[i].textureID = wallTextureID;
        state.objects[i].mesh = wallMesh;
        state.objects[i].rotation = glm::vec3(0, 0, 0);
        state.objects[i].entityType = WALL;
        state.objects[i].height = 20;
        state.objects[i].width = 0.5f;
        state.objects[i].depth = 40;
        if (i % 2 == 0) {
            state.objects[i].scale = glm::vec3(0.5f, 20, 40);
            state.objects[i].position = glm::vec3(40.5f, 0.5f, 0);
        }
        else {

            state.objects[i].scale = glm::vec3(0.5f, 20, 40);
            state.objects[i].position = glm::vec3(-40.5f, 0.5f, 0);
        }
    }
    state.objects[3].position = glm::vec3(0, 0.5f, 40.5f);
    state.objects[3].scale = glm::vec3(40, 20, 0.5f);
    state.objects[3].width = 40;
    state.objects[3].depth = 0.5f;
    state.objects[4].position = glm::vec3(0, 0.5f, -40.5f);
    state.objects[4].scale = glm::vec3(40, 20, 0.5f);
    state.objects[4].width = 40;
    state.objects[4].depth = 0.5f;

    GLuint crateTextureID = Util::LoadTexture("hedge_ivy.png");
    Mesh* crateMesh = new Mesh();
    crateMesh->LoadOBJ("cube.obj", 1);

    
    float x = -19.5f;
    float z = 19;
    for (int i = 5; i < 24; i++) {
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        x += 1.0f; 
    }
    x = 1.5f;
    z = 19;
    for (int i = 24; i < 43; i++) {
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }
    x = -19.5f;
    z = 18;
    for (int i = 43; i < 81; i++) {
        if (i == 62) x = 1.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }
    x = -19.5f;
    z = 17;
    for (int i = 81; i < 89; i++) {
        if (i == 87) x = 18.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }
    x = -19.5f;
    z = 16;
    for (int i = 89; i < 97; i++) {
        if (i == 95) x = 18.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }
    x = -19.5f;
    z = 15;
    for (int i = 97; i < 133; i++) {
        if (i == 103) x = -11.5f;
        if (i == 120) x = 7.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }
    x = -19.5f;
    z = 14;
    for (int i = 133; i < 167; i++) {
        if (i == 139) x = -11.5f;
        if (i == 153) x = 4.5f;
        if (i == 154) x = 7.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }

    x = -19.5f;
    z = 13;
    for (int i = 167; i < 201; i++) {
        if (i == 173) x = -11.5f;
        if (i == 187) x = 4.5f;
        if (i == 188) x = 7.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 12;
    for (int i = 201; i < 213; i++) {
        if (i == 207) x = -0.5f;
        if (i == 210) x = 17.5f;

        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 11;
    for (int i = 213; i < 225; i++) {
        if (i == 219) x = -0.5f;
        if (i == 222) x = 17.5f;

        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 10;
    for (int i = 225; i <257; i++) {
        if (i == 237) x = -5.5f;
        if (i == 240) x = -0.5f;
        if (i == 246) x = 7.5f;
        if (i == 254) x = 17.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 9;
    for (int i = 257; i < 287; i++) {
        if (i == 259) x = -15.5f;
        if (i == 267) x = -5.5f;
        if (i == 270) x = -0.5f;
        if (i == 276) x = 7.5f;
        if (i == 284) x = 17.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 8;
    for (int i = 287; i < 315; i++) {
        if (i == 289) x = -15.5f;
        if (i == 297) x = -5.5f;
        if (i == 300) x = -0.5f;
        if (i == 306) x = 9.5f;
        if (i == 312) x = 17.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 7;
    for (int i = 315; i < 337; i++) {
        if (i == 317) x = -5.5f;
        if (i == 328) x = 9.5f;
        if (i == 334) x = 17.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 6;
    for (int i = 337; i < 359; i++) {
        if (i == 339) x = -5.5f;
        if (i == 356) x = 17.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 5;
    for (int i = 359; i < 387; i++) {
        if (i == 363) x = -13.5f;
        if (i == 369) x = -5.5f;
        if (i == 377) x = 4.5f;
        if (i == 384) x = 17.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    
    x = -19.5f;
    z = 4;
    for (int i = 387; i < 416; i++) {
        if (i == 391) x = -13.5f;
        if (i == 397) x = -2.5f;
        if (i == 402) x = 4.5f;
        if (i == 409) x = 13.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 3;
    for (int i = 416; i < 440; i++) {
        if (i == 420) x = -13.5f;
        if (i == 426) x = -2.5f;
        if (i == 431) x = 4.5f;
        if (i == 433) x = 13.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 2;
    for (int i = 440; i < 469; i++) {
        if (i == 444) x = -13.5f;
        if (i == 460) x = 4.5f;
        if (i == 462) x = 13.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 1;
    for (int i = 469; i < 503; i++) {
        if (i == 473) x = -13.5f;
        if (i == 489) x = 4.5f;
        if (i == 496) x = 13.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 0;
    for (int i = 503; i < 537; i++) {
        if (i == 507) x = -13.5f;
        if (i == 523) x = 4.5f;
        if (i == 530) x = 13.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = 20;
    for (int i = 957; i < 997 ; i++) {
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }
    x = -19.5f;
    z = -20;
    for (int i = 924; i < 957; i++) {
        if (i == 954) x = 17.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }
    x = -19.5f;
    z = -19;
    for (int i = 891; i < 924; i++) {
        if (i == 921) x = 17.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }
    x = -19.5f;
    z = -18;
    for (int i = 857; i < 891; i++) {
        if (i == 860) x = -12.5f;
        if (i == 886) x = 15.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }
    x = -19.5f;
    z = -17;
    for (int i = 851; i < 857; i++) {
        if (i == 854) x = -12.5f;
        if (i == 855) x = 15.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }
    x = -19.5f;
    z = -16;
    for (int i = 843; i < 851; i++) {
        if (i == 846) x = -14.5f;
        if (i == 849) x = 15.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }
    x = -19.5f;
    z = -15;
    for (int i = 811; i < 843; i++) {
        if (i == 814) x = -14.5f;
        if (i == 817) x = -9.5f;
        if (i == 833) x = 8.5;
        if (i == 842) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }
    x = -19.5f;
    z = -14;
    for (int i = 791; i < 811; i++) {
        if (i == 793) x = -4.5f;
        if (i == 804) x = 8.5f;
        if (i == 810) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;

    }

    x = -19.5f;
    z = -13;
    for (int i = 771; i < 791; i++) {
        if (i == 773) x = -4.5f;
        if (i == 784) x = 8.5f;
        if (i == 790) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = -12;
    for (int i = 752; i < 771; i++) {
        if (i == 760) x = -9.5f;
        if (i == 763) x = 2.5f;
        if (i == 764) x = 11.5f;
        if (i == 770) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = -11;
    for (int i = 740; i < 752; i++) {
        if (i == 743) x = -9.5f;
        if (i == 746) x = 2.5f;
        if (i == 747) x = 11.5f;
        if (i == 750) x = 16.5f;
        if (i == 751) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    
    x = -19.5f;
    z = -10;
    for (int i = 716; i < 740; i++) {
        if (i == 719) x = -9.5f;
        if (i == 721) x = -4.5f;
        if (i == 729) x = 5.5f;
        if (i == 738) x = 16.5f;
        if (i == 739) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = -9;
    for (int i = 687; i < 716; i++) {
        if (i == 690) x = -14.5f;
        if (i == 697) x = -4.5f;
        if (i == 705) x = 5.5f;
        if (i == 714) x = 16.5f;
        if (i == 715) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = -8;
    for (int i = 664; i < 687; i++) {
        if (i == 667) x = -14.5f;
        if (i == 685) x = 16.5f;
        if (i == 686) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = -7;
    for (int i = 657; i < 664; i++) {
        if (i == 660) x = 1.5f;
        if (i == 662) x = 16.5f;
        if (i == 663) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = -6;
    for (int i = 637; i < 657; i++) {
        if (i == 640) x = 1.5f;
        if (i == 656) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = -5;
    for (int i = 609; i < 637; i++) {
        if (i == 628) x = 1.5f;
        if (i == 631) x = 8.5f;
        if (i == 634) x = 15.5f;
        if (i == 636) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = -4;
    for (int i = 583; i < 609; i++) {
        if (i == 587) x = -13.5f;
        if (i == 600) x = 1.5f;
        if (i == 603) x = 8.5f;
        if (i == 606) x = 15.5f;
        if (i == 608) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = -3;
    for (int i = 553; i < 583; i++) {
        if (i == 557) x = -13.5f;
        if (i == 570) x = 1.5f;
        if (i == 575) x = 8.5f;
        if (i == 578) x = 13.5f;
        if (i == 582) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = -2;
    for (int i = 545; i < 553; i++) {
        if (i == 549) x = 8.5f;
        if (i == 552) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    x = -19.5f;
    z = -1;
    for (int i = 537; i < 545; i++) {
        if (i == 541) x = 8.5f;
        if (i == 544) x = 19.5f;
        state.objects[i].textureID = crateTextureID;
        state.objects[i].mesh = crateMesh;
        state.objects[i].position = glm::vec3(x, 0.5f, z);
        state.objects[i].entityType = HEDGE;
        //state.objects[i].scale = glm::vec3(1, 2, 1);
        x += 1.0f;
    }
    
    for (int i = 5; i < 997; i++) {
        state.objects[i].scale = glm::vec3(1, 3, 1);
        state.objects[i].height = 5;
    }
    Mesh* doorMesh = new Mesh();
    doorMesh->LoadOBJ("cube.obj", 1);
    GLuint doorTextureID = Util::LoadTexture("door.jfif");
    state.objects[997].textureID = doorTextureID;
    state.objects[997].position = glm::vec3(13.5f, 0.5f, -18.5f);
    state.objects[997].entityType = DOOR;
    state.objects[997].mesh = doorMesh;
    state.objects[998].textureID = doorTextureID;
    state.objects[998].position = glm::vec3(14.5f, 0.5f, -18.5f);
    state.objects[998].entityType = DOOR;
    state.objects[998].mesh = doorMesh;
    state.objects[999].textureID = doorTextureID;
    state.objects[999].position = glm::vec3(13.5f, 1.5f, -18.5f);
    state.objects[999].entityType = DOOR;
    state.objects[999].mesh = doorMesh;
    state.objects[1000].textureID = doorTextureID;
    state.objects[1000].position = glm::vec3(14.5f, 1.5f, -18.5f);
    state.objects[1000].entityType = DOOR;
    state.objects[1000].mesh = doorMesh;
    //ENEMY

    GLuint enemyTextureID = Util::LoadTexture("ghost.png");

    state.enemy1 = new Entity();
    state.enemy1->billboard = true;
    state.enemy1->textureID = enemyTextureID;
    state.enemy1->position = glm::vec3(14, 0.5f, 16.5f);
    state.enemy1->rotation = glm::vec3(0, 0, 0);
    state.enemy1->acceleration = glm::vec3(0, 0, 0);
    state.enemy1->entityType = ENEMY;
    state.enemy1->enemyType = ENEMY1;
    state.enemy1->velocity = glm::vec3(1, 0, 0);
    state.enemy1->speed = 1.0f;

    state.enemy2 = new Entity();
    state.enemy2->billboard = true;
    state.enemy2->textureID = enemyTextureID;
    state.enemy2->position = glm::vec3(-13.5f, 0.5f, -1.5f);
    state.enemy2->rotation = glm::vec3(0, 0, 0);
    state.enemy2->acceleration = glm::vec3(0, 0, 0);
    state.enemy2->entityType = ENEMY;
    state.enemy2->enemyType = ENEMY2;
    state.enemy2->velocity = glm::vec3(1, 0, 0);
    state.enemy2->speed = 1.0f;
    
    state.enemy3 = new Entity();
    state.enemy3->billboard = true;
    state.enemy3->textureID = enemyTextureID;
    state.enemy3->position = glm::vec3(-10, 0.5f, -16.5f);
    state.enemy3->rotation = glm::vec3(0, 0, 0);
    state.enemy3->acceleration = glm::vec3(0, 0, 0);
    state.enemy3->entityType = ENEMY;
    state.enemy3->enemyType = ENEMY3;
    state.enemy3->velocity = glm::vec3(1, 0, 0);
    state.enemy3->speed = 1.0f;

    state.enemy4 = new Entity();
    state.enemy4->billboard = true;
    state.enemy4->textureID = enemyTextureID;
    state.enemy4->position = glm::vec3(18, 0.5f, -1.5f);
    state.enemy4->rotation = glm::vec3(0, 0, 0);
    state.enemy4->acceleration = glm::vec3(0, 0, 0);
    state.enemy4->entityType = ENEMY;
    state.enemy4->enemyType = ENEMY4;
    state.enemy4->velocity = glm::vec3(0, 0, 1);
    state.enemy4->speed = 1.0f;

    state.enemy5 = new Entity();
    state.enemy5->billboard = true;
    state.enemy5->textureID = enemyTextureID;
    state.enemy5->position = glm::vec3(-7, 0.5f, 6.5f);
    state.enemy5->rotation = glm::vec3(0, 0, 0);
    state.enemy5->acceleration = glm::vec3(0, 0, 0);
    state.enemy5->entityType = ENEMY;
    state.enemy5->enemyType = ENEMY5;
    state.enemy5->velocity = glm::vec3(1, 0, 0);
    state.enemy5->speed = 1.0f;

    //KEY

    GLuint keyTextureID = Util::LoadTexture("key.png");
    state.key = new Entity();
    state.key->textureID = keyTextureID;
    state.key->position = glm::vec3(18, 0.5f, -16.5f);
    state.key->billboard = true;
    state.key->entityType = KEY;


    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Update(0, state.player, state.objects, OBJECT_COUNT);
    }
    
}
void Level1::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
    if (state.player->won) {
        state.nextScene = 2;
    }
    state.enemy1->Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
    state.enemy2->Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
    state.enemy3->Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
    state.enemy4->Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
    state.enemy5->Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
    state.key->Update(deltaTime, state.player, state.objects, OBJECT_COUNT);
}
void Level1::Render(ShaderProgram* program) {
    state.player->Render(program);
   
    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].Render(program);
    }
    state.enemy1->Render(program);
    state.enemy2->Render(program);
    state.enemy3->Render(program);
    state.enemy4->Render(program);
    state.enemy5->Render(program);
    state.key->Render(program);
}
