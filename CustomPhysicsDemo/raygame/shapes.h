#pragma once

#include <cstdint>

#include "glm/vec2.hpp"

struct circle
{
	float radius;
};

struct AABB 
{
	float width;
	float height;
};

enum class shapeType : uint8_t
{
	NONE    = 0,
	CIRCLE  = 1 << 0,
	AABB    = 2 << 0 
};

struct collider
{
	shapeType colliderShape;
	union //  unnamed union
	{
		circle circleData;
		AABB boxData;
	};

};

bool checkCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB);
bool checkCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB);

bool checkAABBCircle(glm::vec2 posA, AABB boxA, glm::vec2 posB, circle circle);
bool checkAABBCircle(glm::vec2 posA, collider boxA, glm::vec2 posB, collider circle);

bool checkAABBAABB(glm::vec2 posA, AABB boxA, glm::vec2 posB, AABB boxB);
bool checkAABBAABB(glm::vec2 posA, collider boxA, glm::vec2 posB, collider boxB);

//  vec2 for collision normal
//  pos of shape A
//  shape of shape A
//  pos of shape B
//  shape of shape B
//  penetration distance

glm::vec2 gatherCollisionDataCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB, float& pen);
glm::vec2 gatherCollisionDataCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB, float& pen);

glm::vec2 gatherCollisionDataAABBCircle(glm::vec2 posA, AABB box, glm::vec2 posB, circle circle, float& pen);
glm::vec2 gatherCollisionDataAABBCircle(glm::vec2 posA, collider box, glm::vec2 posB, collider circle, float& pen);

glm::vec2 gatherCollisionDataAABBAABB(glm::vec2 posA, AABB boxA, glm::vec2 posB, AABB boxB, float& pen);
glm::vec2 gatherCollisionDataAABBAABB(glm::vec2 posA, collider boxA, glm::vec2 posB, collider boxB, float& pen);

//  write a wrapper that will work w/ shapes instead of directly referencing circles

void resolveCollision(glm::vec2 posA, glm::vec2 velA, float massA, 
					  glm::vec2 posB, glm::vec2 velB, float massB, 
					  float elasticity, glm::vec2 colNormal, glm::vec2 *dst);