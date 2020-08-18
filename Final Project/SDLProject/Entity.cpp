#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    modelMatrix = glm::mat4(1.0f);
    rotation = glm::vec3(0);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    speed = 0.0f;
    billboard = false;
    width = 1.0f;
    height = 1.0f;
    depth = 1.0f;
    animIndices = NULL;
    enemyCollision = Mix_LoadWAV("enemyCollision.wav");
    keySound = Mix_LoadWAV("keys.wav");
}

void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount)
{
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    collidedFront = false;
    collidedBack = false;

    glm::vec3 previousPosition = position;

    if (billboard) {
        float directionX = position.x - player->position.x;
        float directionZ = position.z - player->position.z;
        rotation.y = glm::degrees(atan2f(directionX, directionZ));
    }
    if (animIndices != NULL) {
       animTime += deltaTime;
       if (animTime >= 0.25f){
           animTime = 0.0f;
           animIndex++;
           if (animIndex >= animFrames){
               animIndex = 0;
           }
       }
    }
    if (jump) {
        jump = false;
        velocity.y += jumpPower;
    }
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    
    glm::vec3 finalPosition = position;
    
    for (int i = 0; i < objectCount; i++)    {
            Entity* object = &objects[i];
            if (object->entityType == FLOOR) { if (CheckCollisionY(object)) { finalPosition.y = previousPosition.y; } }
            else if (object->entityType == HEDGE) {
                if (entityType == PLAYER) {
                    if (CheckCollisionY(object)) { finalPosition.y = previousPosition.y; }
                    if (position.y - object->position.y < 3) {
                        if (CheckCollisionZ(object)) { finalPosition.z = previousPosition.z; }
                        if (CheckCollisionX(object)) { finalPosition.x = previousPosition.x; }

                    }
                }
            }
            else if (object->entityType == DOOR) {
                if (CheckCollision(object)) {
                    //finalPosition = previousPosition;
                    if (key) {
                        won = true;
                    }
                    else {
                        finalPosition = previousPosition;
                    }
                }
            }
            float zdist = fabs(position.z);
            float xdist = fabs(position.x);
            if (zdist >= 18.5f) { finalPosition.z = previousPosition.z; }
            if (xdist >= 18.5f) { finalPosition.x = previousPosition.x; }
    }
    
    if (entityType == ENEMY){
        if (CheckCollision(player)) {
            if (player->life != 0) Mix_PlayChannel(-1, enemyCollision, 0);
            player->life -= 1;
            player->position = glm::vec3(0, 4.5f, 18.5f);
            player->rotation = glm::vec3(0, 0, 0);
        }
    }
    if (entityType == KEY) {
        if (CheckCollision(player)) {
            Mix_PlayChannel(-1, keySound, 0);
            player->key = true;
            isActive = false;
        }
    }
    position = finalPosition;
    if (entityType == ENEMY) AI();
    modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
}
void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) {
    if (isActive == false) return;
    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);

    if (billboard) {
        DrawBillboard(program);
        return;
    }
    else if (mesh != NULL) {
        mesh->Render(program);
        return;
    }
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::DrawBillboard(ShaderProgram* program) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

bool Entity::CheckCollision(Entity* other)
{
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    float zdist = fabs(position.z - other->position.z) - ((depth + other->depth) / 2.0f);
    if (xdist < 0 && ydist < 0 && zdist < 0) return true;
    return false;
}

bool Entity::CheckCollisionX(Entity* object) {

    if (CheckCollision(object)) {
       if (velocity.x > 0) {
            collidedRight = true;
       }
       else if (velocity.x < 0) {
            collidedLeft = true;
       }
       velocity.x = 0;
       return true;
    }
    return false;
}
bool Entity::CheckCollisionY(Entity* object) {

    if (CheckCollision(object)) {
        if (velocity.y > 0) {
            collidedTop = true;
        }
        else if (velocity.y < 0) {
            collidedBottom = true;
        }
        velocity.y = 0;
        return true;
    }
    return false;
}
bool Entity::CheckCollisionZ(Entity* object) {

     if (CheckCollision(object)){
        if (velocity.z > 0) {
            collidedFront = true;
        }
        else if (velocity.z < 0) {
            collidedBack = true;
        }
        velocity.z = 0;
        return true;
     }
     return false;
}

void Entity::AI() {
    if (enemyType == ENEMY1) {
        if (position.x > 15) velocity.x = -1.0f; if (position.x < -13) velocity.x = 1.0f;
    }
    else if (enemyType == ENEMY2)
    {
        if (position.x > 6) velocity.x = -1.0f; if (position.x < -13) velocity.x = 1.0f;
    }
    else if (enemyType == ENEMY3) {
        if (position.x > 14) velocity.x = -1.0f; if (position.x < -10) velocity.x = 1.0f;
    }
    else if (enemyType == ENEMY4){
        if (position.z > -2.5f) velocity.z = -1.0f; if (position.z < -16) velocity.z = 1.0f;
    }
    else if (enemyType == ENEMY5) {
        if (position.x > -6) velocity.x = -1.0f; if (position.x < -15) velocity.x = 1.0f;
    }
}