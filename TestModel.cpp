#include "TestModel.h"
#include <iostream>

const float MAX_VALUE = numeric_limits<float>::max();

Photon::Photon(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = glm::normalize(dir);
}

float Photon::operator[](int index) const {
	if (index == 0) {
		return position.x;
	}
	else if (index == 1) {
		return position.y;
	}
	else if (index == 2) {
		return position.z;
	}
	else {
		std::cout << "axis is not 0 or 1 or 2" << std::endl;
		return 0.0f;
	}
}

Intersection::Intersection(glm::vec3 pos, float dis, int triIndex)
{
	position = pos;
	distance = dis;
	triangleIndex = triIndex;
}

void Triangle::ComputeNormal()
{
	glm::vec3 e1 = v1 - v0;
	glm::vec3 e2 = v2 - v0;
	normal = glm::normalize(glm::cross(e2, e1));
}

bool ClosestIntersection(
	vec3 start,
	vec3 dir,
	const vector<Triangle>& triangles,
	Intersection& closestIntersection
)
{
	bool flag = false;
	closestIntersection.distance = MAX_VALUE;
	for (size_t i = 0; i < triangles.size(); ++i)
	{
		// calculate t, u, v
		Triangle triangle = triangles[i];
		vec3 v0 = triangle.v0;
		vec3 v1 = triangle.v1;
		vec3 v2 = triangle.v2;
		vec3 e1 = v1 - v0;
		vec3 e2 = v2 - v0;
		vec3 b = start - v0;
		glm::mat3 A(-dir, e1, e2);
		vec3 x = glm::inverse(A) * b;
		float t = x[0];
		float u = x[1];
		float v = x[2];

		if (u > -0.0001 && v > -0.0001 && u + v < 1.0001 && t >= 0.0001)
		{
			flag = true;
			if (t < closestIntersection.distance)
			{
				vec3 position = start + t * dir;
				closestIntersection.position = position;
				closestIntersection.distance = t;
				closestIntersection.triangleIndex = i;
			}
		}
	}
	return flag;
}

Triangle::Triangle(glm::vec3 V0, glm::vec3 V1, glm::vec3 V2, glm::vec3 col)
{
	v0 = V0;
	v1 = V1;
	v2 = V2;
	color = col;
	ComputeNormal();
}

void LoadTestModel(std::vector<Triangle>& triangles)
{
	using glm::vec3;

	// Defines colors:
	vec3 red(0.75f, 0.15f, 0.15f);
	vec3 yellow(0.75f, 0.75f, 0.15f);
	vec3 green(0.15f, 0.75f, 0.15f);
	vec3 cyan(0.15f, 0.75f, 0.75f);
	vec3 blue(0.15f, 0.15f, 0.75f);
	vec3 purple(0.75f, 0.15f, 0.75f);
	vec3 white(0.75f, 0.75f, 0.75f);

	triangles.clear();
	triangles.reserve(5 * 2 * 3);

	// ---------------------------------------------------------------------------
	// Room

	float L = 555;			// Length of Cornell Box side.

	vec3 A(L, 0, 0);
	vec3 B(0, 0, 0);
	vec3 C(L, 0, L);
	vec3 D(0, 0, L);

	vec3 E(L, L, 0);
	vec3 F(0, L, 0);
	vec3 G(L, L, L);
	vec3 H(0, L, L);

	// Floor:
	triangles.push_back(Triangle(C, B, A, green));
	triangles.push_back(Triangle(C, D, B, green));

	// Left wall
	triangles.push_back(Triangle(A, E, C, purple));
	triangles.push_back(Triangle(C, E, G, purple));

	// Right wall
	triangles.push_back(Triangle(F, B, D, yellow));
	triangles.push_back(Triangle(H, F, D, yellow));

	// Ceiling
	triangles.push_back(Triangle(E, F, G, cyan));
	triangles.push_back(Triangle(F, H, G, cyan));

	// Back wall
	triangles.push_back(Triangle(G, D, C, white));
	triangles.push_back(Triangle(G, H, D, white));

	// ---------------------------------------------------------------------------
	// Short block

	A = vec3(290, 0, 114);
	B = vec3(130, 0, 65);
	C = vec3(240, 0, 272);
	D = vec3(82, 0, 225);

	E = vec3(290, 165, 114);
	F = vec3(130, 165, 65);
	G = vec3(240, 165, 272);
	H = vec3(82, 165, 225);

	// Front
	triangles.push_back(Triangle(E, B, A, red));
	triangles.push_back(Triangle(E, F, B, red));

	// Front
	triangles.push_back(Triangle(F, D, B, red));
	triangles.push_back(Triangle(F, H, D, red));

	// BACK
	triangles.push_back(Triangle(H, C, D, red));
	triangles.push_back(Triangle(H, G, C, red));

	// LEFT
	triangles.push_back(Triangle(G, E, C, red));
	triangles.push_back(Triangle(E, A, C, red));

	// TOP
	triangles.push_back(Triangle(G, F, E, red));
	triangles.push_back(Triangle(G, H, F, red));

	// ---------------------------------------------------------------------------
	// Tall block

	A = vec3(423, 0, 247);
	B = vec3(265, 0, 296);
	C = vec3(472, 0, 406);
	D = vec3(314, 0, 456);

	E = vec3(423, 330, 247);
	F = vec3(265, 330, 296);
	G = vec3(472, 330, 406);
	H = vec3(314, 330, 456);

	// Front
	triangles.push_back(Triangle(E, B, A, blue));
	triangles.push_back(Triangle(E, F, B, blue));

	// Front
	triangles.push_back(Triangle(F, D, B, blue));
	triangles.push_back(Triangle(F, H, D, blue));

	// BACK
	triangles.push_back(Triangle(H, C, D, blue));
	triangles.push_back(Triangle(H, G, C, blue));

	// LEFT
	triangles.push_back(Triangle(G, E, C, blue));
	triangles.push_back(Triangle(E, A, C, blue));

	// TOP
	triangles.push_back(Triangle(G, F, E, blue));
	triangles.push_back(Triangle(G, H, F, blue));


	// ----------------------------------------------
	// Scale to the volume [-1,1]^3

	for (size_t i = 0; i < triangles.size(); ++i)
	{
		triangles[i].v0 *= 2 / L;
		triangles[i].v1 *= 2 / L;
		triangles[i].v2 *= 2 / L;

		triangles[i].v0 -= vec3(1, 1, 1);
		triangles[i].v1 -= vec3(1, 1, 1);
		triangles[i].v2 -= vec3(1, 1, 1);

		triangles[i].v0.x *= -1;
		triangles[i].v1.x *= -1;
		triangles[i].v2.x *= -1;

		triangles[i].v0.y *= -1;
		triangles[i].v1.y *= -1;
		triangles[i].v2.y *= -1;

		triangles[i].ComputeNormal();
	}
}

vec3 Rotate(vec3 vec, vec3 axis, float theta)
{
	vec3 ret;
	float cost = cos(theta);
	float sint = sin(theta);
	ret.x += vec.x * (axis.x * axis.x + (1 - axis.x * axis.x) * cost);
	ret.x += vec.y * (axis.x * axis.y * (1 - cost) - axis.z * sint);
	ret.x += vec.z * (axis.x * axis.z * (1 - cost) + axis.y * sint);
	ret.y += vec.x * (axis.y * axis.x * (1 - cost) + axis.z * sint);
	ret.y += vec.y * (axis.y * axis.y + (1 - axis.y * axis.y) * cost);
	ret.y += vec.z * (axis.y * axis.z * (1 - cost) - axis.x * sint);
	ret.z += vec.x * (axis.z * axis.x * (1 - cost) - axis.y * sint);
	ret.z += vec.y * (axis.z * axis.y * (1 - cost) + axis.x * sint);
	ret.z += vec.z * (axis.z * axis.z + (1 - axis.z * axis.z) * cost);
	return ret;
}