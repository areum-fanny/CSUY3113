#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    speed = 0;
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    modelMatrix = glm::mat4(1.0f);
}

void Entity::Update(float deltaTime, Entity* platforms, int platformcount)
{

    if (isActive == false || state == DEATH) return;

    bool collidedLeft = false;
    bool collidedRight = false;
    bool collidedTop = false;
    bool collidedBottom = false;
    if (type == ENEMY) {
        AI();
    }
    if (animIndices != NULL) {
        if (glm::length(movement) != 0 || state == SHOOT) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
                
            }
        }
        else {
            animIndex = 0;
        }
    }
    
    if (jump) {
        jump = false;
        velocity.y += jumpPower;
    }
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    //position += velocity * deltaTime;

    /*for (int i = 0; i < platformcount; i++)
    {
        Entity* platform = &platforms[i];
        if (CheckCollision(platform))
        {
            float ydist = fabs(position.y - platform->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (platform->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
            }
        }
    }*/

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


    if (isActive ==  false ) return;

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
    if (isActive == false || other->isActive == false) return false;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);
    //if (other->type == SPIKE && xdist < -0.2 && ydist < -0.2) return true;
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
            if (object->type == SPIKE && type == PLAYER) {
                state = DEATH;
            }
            else if (object->type == ENEMY && type == PLAYER) {
                state = DEATH;
            }
            else if (object->type == KEY && type == PLAYER) {
                key = true;
                object->isActive = false;
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
                float xdist = fabs(position.x - object->position.x);
                float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
                if (velocity.x > 0) {
                    position.x -= penetrationX;
                    velocity.x = 0;
                    collidedRight = true;
                    if (aiType == WALKER) {
                        direction = LEFT;
                    }
                }
                else if (velocity.x < 0) {
                    position.x += penetrationX;
                    velocity.x = 0;
                    collidedLeft = true;
                    if (aiType == WALKER) {
                        direction = RIGHT;
                    }
                }
                if (object->type == SPIKE && type == PLAYER) {
                    state = DEATH;
                }
                else if (object->type == ENEMY && type == PLAYER) {
                    state = DEATH;
                }
                else if (object->type == KEY && type == PLAYER) {
                    key = true;
                    object->isActive = false;
                }
                else if (object->type == ENEMY && type == BULLET) {
                    object->isActive = false;
                    object->state = DEATH;
                    isActive = false;
                    state = IDLE;
                }
                else if (object->type == SPIKE && type == BULLET) {
                    isActive = false;
                    state = IDLE;
                }
        }
    }
}

void Entity::AI() {
    switch (aiType) {
        case WALKER:
            AIWalker();
            break;
    }
}

void Entity::AIWalker() {
    if (direction == LEFT) {
        movement.x = -1.0f;
    }
    else {
        movement.x = 1.0f;
    }
}