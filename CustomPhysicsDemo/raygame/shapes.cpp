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

bool checkAABBCircle(glm::vec2 posA, AABB boxA, glm::vec2 posB, circle circle)
{
	//  get the disttance between the two shapes
	float distance = glm::length(posA - posB);

	//  get the sum of the radius and hypotenuse
	float sum = circle.radius + sqrt(boxA.width * boxA.width + boxA.height * boxA.height);

	//  return distance < sum
	return (posA.x + boxA.width / 2 > posB.x - circle.radius &&
			posA.x - boxA.width / 2 < posB.x + circle.radius &&
			posA.y + boxA.height / 2 > posB.y - circle.radius &&
			posA.y - boxA.height / 2 < posB.y + circle.radius && 
			distance < sum);
}

bool checkAABBCircle(glm::vec2 posA, collider boxA, glm::vec2 posB, collider circle)
{
	return checkAABBCircle(posA, boxA.boxData, posB, circle.circleData);
}

bool checkAABBAABB(glm::vec2 posA, AABB boxA, glm::vec2 posB, AABB boxB)
{
	//  make sure that the box is overlapping on its sides

	return(posA.x + boxA.width / 2 > posB.x - boxB.width / 2 && 
		   posA.x - boxA.width / 2 < posB.x + boxB.width / 2 &&
		   posA.y + boxA.height / 2 > posB.y - boxB.height / 2 && 
		   posA.y - boxA.height / 2 < posB.y + boxB.height / 2);
}

bool checkAABBAABB(glm::vec2 posA, collider boxA, glm::vec2 posB, collider boxB)
{
	return checkAABBAABB(posA, boxA.boxData, posB, boxB.boxData);
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

glm::vec2 gatherCollisionDataAABBCircle(glm::vec2 posA, AABB box, glm::vec2 posB, circle circle, float & pen)
{
	//  calculate their distance
	float dist = glm::length(posA - posB);

	//  check the penetration level and move it based on the most penetrated side
	if (posA.x + box.width / 2 - posB.x - circle.radius / 2 > posA.y + box.height / 2 - posB.y - circle.radius / 2)
	{
		float sum = box.width / 2 + circle.radius;
		pen = sum - dist;
	}
	else
	{
		float sum = box.height / 2 + circle.radius;
		pen = sum - dist;
	}

	//  get the normalized vector between the two
	return glm::normalize(posA - posB);
}

glm::vec2 gatherCollisionDataAABBCircle(glm::vec2 posA, collider box, glm::vec2 posB, collider circle, float & pen)
{
	return gatherCollisionDataAABBCircle(posA, box.boxData, posB, circle.circleData, pen);
}

glm::vec2 gatherCollisionDataAABBAABB(glm::vec2 posA, AABB boxA, glm::vec2 posB, AABB boxB, float & pen)
{
	//  calculate their distance
	float dist = glm::length(posA - posB);

	//  check the penetration level and move it based on the most penetrated side
	if (posA.x + boxA.width / 2 - posB.x - boxB.width / 2 > posA.y + boxA.height / 2 - posB.y - boxB.height / 2) 
	{
		float sum = (boxA.width + boxB.width) / 2;
		pen = sum - dist;
	}
	else 
	{
		float sum = (boxA.height + boxB.height) / 2;
		pen = sum - dist;
	}

	//  get the normalized vector between the two
	return glm::normalize(posA - posB);
}

glm::vec2 gatherCollisionDataAABBAABB(glm::vec2 posA, collider boxA, glm::vec2 posB, collider boxB, float & pen)
{
	return gatherCollisionDataAABBAABB(posA, boxA.boxData, posB, boxB.boxData, pen);
}

void resolveCollision(glm::vec2 posA, glm::vec2 velA, float massA, 
					  glm::vec2 posB, glm::vec2 velB, float massB, 
					  float elasticity, glm::vec2 colNormal, glm::vec2 * dst)
{
	//  calculate relative velocity
	glm::vec2 relvel = velA - velB;
	
	//  calculate impulse magnitude
	float impulseMag = glm::dot(-(1.0f + elasticity) * relvel, colNormal) /
					   glm::dot(colNormal, colNormal * (1/ massA + 1 / massB));

	//  write out impulses for the two objects into the given array
	dst[0] = velA + (impulseMag / massA) * colNormal;
	dst[1] = velB - (impulseMag / massB) * colNormal;
}


