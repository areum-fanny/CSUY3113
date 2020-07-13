#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    gravity = glm::vec3(0,-9.81f,0);
    velocity = glm::vec3(0);
    speed = 0;
    modelMatrix = glm::mat4(1.0f);
}

void Entity::Update(float deltaTime, Entity* platforms, int platformcount)
{
    if (state == DEAD || isActive == false || wonGame) return;
    collidedLeft = false;
    collidedRight = false;
    collidedTop = false;
    collidedBottom = false;

    if (animIndices != NULL) {
        if (glm::length(movement) != 0 || state == SHOOT) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    if (state == SHOOT) {
                        if (direction == RIGHT) {
                            animIndices = idleRight;
                            animFrames = 1;
                            state = IDLE;
                        }
                        else {
                            animIndices = idleLeft;
                            animFrames = 1;
                            state = IDLE;
                        }
                    }
                     animIndex = 0;
                }
            }
        }
        else {
            if (state == WALK) {
                if (direction == RIGHT) {
                    animIndices = idleRight;
                }
                else {
                    animIndices = idleLeft;
                }
            }  
            else if (state == JUMP) {
                velocity.y += jumpPower;
            }
            state = IDLE;
            animFrames = 1;
            animIndex = 0;
        }
    }
    
    velocity.x = movement.x * speed; 
    velocity += gravity * deltaTime;

    position.y += velocity.y * deltaTime; // Move on Y
    CheckCollisionsY(platforms, platformcount);// Fix if needed
    position.x += velocity.x * deltaTime; // Move on X
    CheckCollisionsX(platforms, platformcount);// Fix if needed

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
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

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
bool Entity::CheckCollision(Entity* other) {
    if (other->state == DEAD || other->isActive == false || isActive == false) return false;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    //if (other->type == OBSTACLE && xdist < -1.0f && ydist < -1.0f) return true;
    if (xdist < 0 && ydist < 0) return true;

    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];
        if (CheckCollision(object))
        {

            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
            }
            if (object->type == OBSTACLE && type == PLAYER) {
                state = DEAD;
            }
            else if (object->type == PLAYER) {
                object->state = DEAD;
            }
            else if (object->type == KEY && type == PLAYER) {
                key = true;
                object->isActive = false;
            }
            else if (object->type == DOOR && type == PLAYER) {
                if (key) {
                    wonGame = true;
                }
            }
            else if (type == ENEMY && object->type == BULLET) {
                object->isActive = false;
                object->state = IDLE;
                state = DEAD;
                isActive = false;
            }
            else if (type == BULLET && object->type == PLAYER) {
                object->state = DEAD;
                state = IDLE;
                isActive = false;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];
        if (CheckCollision(object))
        {
            if (object->type == KEY && type == ENEMY) return;
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }
                
            if (object->type == OBSTACLE && type == PLAYER) {
               state = DEAD;
            }
            else if (object->type == ENEMY && type == PLAYER) {
                state = DEAD;
            }
            else if (type == ENEMY && object->type == OBSTACLE) {
                AI();
            }
            else if (object->type == PLAYER) {
                object->state = DEAD;
            }
            else if (object->type == KEY && type == PLAYER) {
                key = true;
                object->isActive = false;
            }
            else if (object->type == DOOR && type == PLAYER) {
                if (key) {
                    wonGame = true;
                }
            }
            else if ((object->type == OBSTACLE || object->type == GROUND || object->type == KEY || object->type == DOOR) && type == BULLET) {
                isActive = false;
                state = IDLE;
            }
            else if (type == ENEMY && object->type == BULLET) {
                object->isActive = false;
                object->state = IDLE;
                state = DEAD;
                isActive = false;
            }
            else if (type == BULLET && object->type == PLAYER) {
                object->state = DEAD;
                state = IDLE;
                isActive = false;
            }
        }
    }
}

void Entity::AI() {
    switch (enemyType) {
        case WALKER:
            AIWalker();
            break;
        case RAISER:
            AIRaiser();
            break;
    }
}
void Entity::AIWalker() {
    if (collidedRight) {
        direction = LEFT;
        animIndices = animLeft;
        movement.x = -1.0f;
    }
    else if(collidedLeft){
        direction = RIGHT;
        animIndices = animRight;
        movement.x = 1.0f;
    }
}

void Entity::AIRaiser() {
    if (collidedRight) {
        direction = LEFT;
        animIndices = animLeft;
        movement.x = -1.0f;
    }
    else if (collidedLeft) {
        direction = RIGHT;
        animIndices = animRight;
        movement.x = 1.0f;
    }
}