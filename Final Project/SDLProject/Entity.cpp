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
        velocity.z = cos(glm::radians(rotation.y)) * -1.0f;
        velocity.x = sin(glm::radians(rotation.y)) * -1.0f;
    }
    if (jump) {
        jump = false;
        velocity.y += jumpPower;
    }
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    
    glm::vec3 finalPosition = position;

    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];
        if (object->entityType == FLOOR) { if (CheckCollisionY(object)) { finalPosition.y = previousPosition.y; } }
        else if(object->entityType == CRATE){
            if (CheckCollisionY(object)) {finalPosition.y = previousPosition.y; }
            if(position.y -  object->position.y < 1.5f){
                if (CheckCollisionZ(object)) { finalPosition.z = previousPosition.z; }
                if (CheckCollisionX(object)) { finalPosition.x = previousPosition.x; }

            }
        }
        //else if (object->entityType == WALL) {
            float zdist = fabs(position.z);
            float xdist = fabs(position.x);
            if (zdist >= 18.5f) { finalPosition.z = previousPosition.z; }
            if (xdist >= 18.5f) { finalPosition.x = previousPosition.x; }
        //}
    }

    position = finalPosition;
    if (entityType == CUBE) {
        rotation.y += 45.0f * deltaTime;
        rotation.z += 45.0f * deltaTime;
    }
    else if (type == ENEMY) {
        //rotation.y += 30 * deltaTime;
    }
    modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
}

void Entity::Render(ShaderProgram* program) {
    program->SetModelMatrix(modelMatrix);

    glBindTexture(GL_TEXTURE_2D, textureID);
    if (billboard) {
        DrawBillboard(program);
    }
    else {
        mesh->Render(program);
    }
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
/*
void Entity::CheckCollisionX(Entity* other, int objectCount) {
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &other[i];
        if (CheckCollision(object))
        {
            if (velocity.x > 0) {
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.y < 0) {
                velocity.x = 0;
                collidedLeft = true;
            }
        }
    }
}
void Entity::CheckCollisionY(Entity* other, int objectCount) {
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &other[i];
        if (CheckCollision(object))
        {

            if (velocity.y > 0) {
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                velocity.y = 0;
                collidedBottom = true;
            }

        }
    }

}
void Entity::CheckCollisionZ(Entity* other, int objectCount) {
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &other[i];
        if (CheckCollision(object))
        {
            if (velocity.z > 0) { 
                velocity.z = 0;
                collidedFront = true;
            }
            else if (velocity.z < 0) {   
                velocity.z = 0;
                collidedBack = true;
            }
        }
    }
}*/
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