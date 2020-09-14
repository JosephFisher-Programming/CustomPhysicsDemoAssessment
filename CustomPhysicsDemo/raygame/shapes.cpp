#include "shapes.h"

#include "glm/glm.hpp"

bool checkCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB) 
{
	//  get the disttance between the two circles
	float distance = glm::length(posA - posB);

	//  get the sum of the radii
	float sum = circleA.radius + circleB.radius;

	//  return distance < sum
	return distance < sum;
}

bool checkCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB)
{
	return checkCircleCircle(posA, circleA.circleData, posB, circleB.circleData);
}

glm::vec2 gatherCollisionDataCircleCircle(glm::vec2 posA, circle circleA, glm::vec2 posB, circle circleB, float& pen)
{
	//  calculate their distances
	float dist = glm::length(posA - posB);
	float sum = circleA.radius + circleB.radius;

	//  spen is equal the distance of radii sum sub distance between to two
	pen = sum - dist;

	//  collision normal is the normalized vector between their two positions
	return glm::normalize(posA - posB);
}

glm::vec2 gatherCollisionDataCircleCircle(glm::vec2 posA, collider circleA, glm::vec2 posB, collider circleB, float & pen)
{
	return gatherCollisionDataCircleCircle(posA, circleA.circleData, posB, circleB.circleData, pen);
}

void resolveCollision(glm::vec2 posA, glm::vec2 velA, float massA, 
					  glm::vec2 posB, glm::vec2 velB, float massB, 
					  float elasticity, glm::vec2 colNormal, glm::vec2 * dst)
{
	//  calculate relative velocity
	glm::vec2 relvel = velA - velB;
	
	//  calculate impulse magnitude
	float impulseMag = glm::dot(-(-1.0f + elasticity) * relvel, colNormal) /
					   glm::dot(colNormal, colNormal * (1/ massA + 1 / massB));

	//  write out impulses for the two objects into the given array
	dst[0] = velA + (impulseMag / massA) * colNormal;
	dst[1] = velB - (impulseMag / massB) * colNormal;
}


