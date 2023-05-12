#include "PhotonTracing.h"
#include "TestModel.h"
#include <cstdlib>
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#define ran() (float(rand() % RAND_MAX) / RAND_MAX) // Generate a random number between [0, 1].

using namespace std;
using glm::vec3;
using glm::mat3;

const int MAX_PHOTONTRACING_DEPTH = 10;
extern const float MAX_VALUE;
const int EMIT_PHOTONS = 5000;
const float EPS = 1e-6;
const vec3 PointLightPos(0.0f, -0.9f, 0.0f); // The point light source is near the center of the roof.
const float PI = 3.14159265358979323846;


bool PhotonTracing::Reflection(Photon photon, int depth, int triIndex)
{
	float prob = 2 * ran() - 1;
	if (prob <= 0) // 50% probability
	{
		return false;
	}
	vec3 normal = triangles[triIndex].normal;
	vec3 reflection = photon.direction - vec3(2.0f) * normal * glm::dot(photon.direction, normal);
	reflection = glm::normalize(reflection);
	photon.direction = reflection;

	Tracing(photon, depth + 1);
	return true;
}

bool PhotonTracing::Diffusion(Photon photon, int depth, int triIndex)
{
	float prob = 2 * ran() - 1;
	if (prob <= 0) // 50% probability
	{
		return false;
	}
	/* Diffusion leads to a trace in random direction, but angle between normal < 90 degrees.
	   To calculate duffusion direction, first let normal rotate around the vector perpendicular to normal by theta,
	   then rotate the obtained vector around the normal by phi. */
	vec3 normal = triangles[triIndex].normal;
	vec3 perpToNormal = glm::cross(normal, glm::vec3(1.0f, 0.0f, 0.0f));
	float theta = acos(sqrt(ran())); // [0, pi/2]
	float phi = ran() * 2 * PI; // [0, 2pi]
	vec3 temp = Rotate(normal, perpToNormal, theta);
	vec3 diffusion = Rotate(temp, normal, phi);
	photon.direction = diffusion;

	Tracing(photon, depth + 1);
	return true;
}

/* Refraction is not done yet. Current scene assumes that all surfaces are diffuse surfaces. */
bool PhotonTracing::Refraction(Photon photon, int depth, int triIndex)
{
	float prob = 2 * ran() - 1;
	if (prob <= 0) // 50% probability
	{
		return false;
	}
	vec3 normal = triangles[triIndex].normal;
	float cosI = - glm::dot(normal, photon.direction);
	// float cosT2 refers to the ratio of refractive index 
	/*............*/

	Tracing(photon, depth + 1);
	return true;
}

void PhotonTracing::Tracing(Photon photon, int depth)
{
	if (depth > MAX_PHOTONTRACING_DEPTH) return;

	Intersection closestIntersection(vec3(0.0f, 0.0f, 0.0f), MAX_VALUE, 0);
	bool flag = ClosestIntersection(photon.position, photon.direction, triangles, closestIntersection);
	
	if (flag == true)
	{
		// Currently, assume all surfaces are diffuse surfaces, thus saving all photons.
		photon.position = closestIntersection.position;
		// When saved in the photons vector, the photon contains the position of where it is now and the direction of where it comes from.
		photons.push_back(photon);
		
		if (Reflection(photon, depth, closestIntersection.triangleIndex) == false);
		if (Diffusion(photon, depth, closestIntersection.triangleIndex) == false);
	}
}

void PhotonTracing::SetScene(std::vector<Triangle> input)
{
	triangles = input;
}

void PhotonTracing::SetPhotons(std::vector<Photon> input)
{
	photons = input;
}

std::vector<Photon> PhotonTracing::GetPhotons()
{
	return photons;
}

void PhotonTracing::Emit()
{
	std::cout << "hello" << std::endl;
	for (int i = 0; i < EMIT_PHOTONS; ++i)
	{
		vec3 direction;
		do
		{
			direction.x = 2 * ran() - 1;
			direction.y = 2 * ran() - 1;
			direction.z = 2 * ran() - 1;
		} while (direction.x * direction.x + direction.y * direction.y + direction.z * direction.z > 1 || direction.x * direction.x + direction.y * direction.y + direction.z * direction.z < EPS);
		direction = glm::normalize(direction);

		// Create a random photon.
		Photon photon(PointLightPos, direction);
		// When doing Tracing, the photon contains the position of where it comes from and the direction of where it goes to.
		Tracing(photon, 1);
	}
}
