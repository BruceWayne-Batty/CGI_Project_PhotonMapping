#ifndef TEST_MODEL_CORNEL_BOX_H
#define TEST_MODEL_CORNEL_BOX_H

// Defines a simple test model: The Cornel Box

#include <glm/glm.hpp>
#include <vector>

using namespace std;
using glm::vec3;

extern const float MAX_VALUE;

// Used to describe a photon:
class Photon
{
public:
	static const int DIM = 3;

	glm::vec3 position;
	glm::vec3 direction;

	Photon(glm::vec3 pos, glm::vec3 dir);

	float operator[](int index) const;
};

// Used to describe an intersection:
class Intersection
{
public:
	glm::vec3 position;
	float distance;
	int triangleIndex;

	Intersection(glm::vec3 pos, float dis, int triIndex);
};

// Used to describe a triangular surface:
class Triangle
{
public:
	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 normal;
	glm::vec3 color;

	Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 color);

	void ComputeNormal();
};

// From lab2.
bool ClosestIntersection(
	vec3 start,
	vec3 dir,
	const vector<Triangle>& triangles,
	Intersection& closestIntersection
);

// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
void LoadTestModel(std::vector<Triangle>& triangles);

// A function for rotating vec around axis by theta.
vec3 Rotate(vec3 vec, vec3 axis, float theta);

#endif