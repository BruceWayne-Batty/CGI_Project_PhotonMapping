//DH2323 skeleton code, Lab2 (SDL2 version)
#include <iostream>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include "SDL2auxiliary.h"
#include "TestModel.h"
#include "PhotonTracing.h"
#include "kDtree.h"

using namespace std;
using glm::vec3;
using glm::ivec2;
using glm::mat3;
using glm::normalize;

// ----------------------------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
SDL2Aux *sdlAux;
int t;
float focalLength = 500.0f;
vec3 cameraPos(0.0f, 0.0f, -3.001f);
vector<Triangle> triangles;
PhotonTracing photonTracer;
vector<Photon> photons;
extern const float MAX_VALUE;
vec3 c1(1.0f, 0.0f, 0.0f);
vec3 c2(0.0f, 1.0f, 0.0f);
vec3 c3(0.0f, 0.0f, 1.0f);
mat3 R(c1, c2, c3);
vec3 lightColor = 14.f * vec3(1, 1, 1);
vec3 indirectLight = 0.5f * vec3(1, 1, 1);
vec3 photonPower = (14.f / 1000) * vec3(1, 1, 1);
const vec3 lightPos(0, -0.5, -0.7);
// ----------------------------------------------------------------------------
// STRUCTURES
struct m_Intersection
{
	vec3 position;
	float distance;
	int triangleIndex;
};
// ----------------------------------------------------------------------------
// FUNCTIONS

void Update(void);
void Draw(void);
void PhotonShader(const vec3& photon, ivec2& pixel);
void Draw_New(kdt::KDTree<Photon> photonMap);
vec3 ScreenToWorld(const ivec2& pixel);
vec3 m_DirectLight(const m_Intersection& i);
bool m_ClosestIntersection(
	vec3 start,
	vec3 dir,
	const vector<Triangle>& triangles,
	m_Intersection& closestIntersection);



int main( int argc, char* argv[] )
{
	sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
	t = SDL_GetTicks();	// Set start value for timer.
	LoadTestModel(triangles);

	photonTracer.SetScene(triangles);
	photonTracer.SetPhotons(photons);
	photonTracer.Emit();
	photons = photonTracer.GetPhotons();
	photonPower = 1.0f / photons.size() * lightColor;
	cout << "Number of photons emitted and stored: " << photons.size() << std::endl;


	//for (const auto& element : photons)
	//{
	//	std::cout << "(" << element.position.x << ", " << element.position.y << ", " << element.position.z << ")" << std::endl;
	//}


	/* Test for kDtree. */
	Photon test(vec3(0, 0, 0), vec3(1, 1, 1));
	/* Save photons vector to photonMap. */
	kdt::KDTree<Photon> photonMap(photons);
	/* Nearest neighbour. */
	int idx = photonMap.nnSearch(test);
	//cout << "Nearest neighbour is (" << photons[idx].position.x << ", " << photons[idx].position.y << ", " << photons[idx].position.z << ")" << endl;
	/* K nearest neighbour. */
	int k = 5;
	vector<int> knn = photonMap.radiusSearch(test, 0.1);
	
	//for (int i = 0; i < knn.size(); ++i)
	//{
	//	cout << "The " << i << " neighbour is (" << photons[knn[i]].position.x << ", " << photons[knn[i]].position.y << ", " << photons[knn[i]].position.z << ")" << endl;
	//}
	

	//Draw();
	Draw_New(photonMap);
	vec3 topLeftWorld = ScreenToWorld(ivec2(0, 0));
	vec3 bottomRightWorld = ScreenToWorld(ivec2(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1));

	
	while (!sdlAux->quitEvent())
	{
		//Update();
		//Draw();
		//Draw_New(photonMap);
		//Draw_PhotonPositions(photons);
	}
	
	
	sdlAux->saveBMP("screenshot.bmp");
	return 0;
}

void Update(void)
{
	// Compute frame time:
	int t2 = SDL_GetTicks();
	float dt = float(t2-t);
	t = t2;
	// cout << "Render time: " << dt << " ms." << endl;
}

void Draw()
{
	sdlAux->clearPixels();

	for (const auto& element : photons)
	{
		ivec2 projPos;
		PhotonShader(element.position, projPos);
		vec3 color(1, 1, 1);
		sdlAux->putPixel(projPos.x, projPos.y, color);
	}

	sdlAux->render();
}
vec3 ScreenToWorld(const ivec2& pixel)
{
	// Convert pixel coordinates to NDC (Normalized Device Coordinates) [-1, 1]
	float ndcX = (pixel.x / float(SCREEN_WIDTH)) * 2.0f - 1.0f;
	float ndcY = 1.0f - (pixel.y / float(SCREEN_HEIGHT)) * 2.0f;

	// Convert NDC to world coordinates
	vec3 rayDir = normalize(vec3(ndcX, ndcY, 1.0f)); // Assuming a simple perspective projection
	vec3 worldPos = cameraPos + rayDir * focalLength; // Calculate the world position

	return worldPos;
}

vec3 PhotonContribution(const Photon& photon, const vec3& worldPos)
{
	// Correct calculation of squared distance
	float distanceSquared = glm::dot(photon.position - worldPos, photon.position - worldPos);
	float falloff = 1.0f / (1.0f + distanceSquared); // Simple falloff function based on squared distance

	// Assuming all photons have the same "color" for simplicity
	vec3 photonColor = 14.0f * vec3(1.0f, 1.0f, 1.0f); // White light

	return photonColor * falloff; // Scale color by falloff
}

void Draw_New(kdt::KDTree<Photon> photonMap)
{
	sdlAux->clearPixels();
	vec3 dir;
	int count;
	for (int y = 0; y < SCREEN_HEIGHT; ++y)
	{
		for (int x = 0; x < SCREEN_WIDTH; ++x)
		{
			m_Intersection closestIntersection;
			vec3 color(0.0,0.0,0.0);
			vec3 pixel(x, y, 0);
			vec3 dir(x - (SCREEN_WIDTH / 2), y - (SCREEN_HEIGHT / 2), focalLength);
			
			if (m_ClosestIntersection(cameraPos, dir, triangles, closestIntersection)) {
				//cout << closestIntersection.position.x << ", " << closestIntersection.position.y << ", " << closestIntersection.position.z << endl;
				Photon hit(closestIntersection.position, vec3(1, 1, 1));
				vector<int> photon_idx = photonMap.radiusSearch(hit, 0.3);
				if (photon_idx.size() > 10) {
					int max_N = (photon_idx.size() > 100) ? 100 : photon_idx.size();
					for (int i = 0; i < max_N; i++) {
						vec3 dist = photons[photon_idx[i]].position - closestIntersection.position;
						color += 1.0f/(1.0f + glm::dot(dist, dist)) * photonPower;
					}
				}
				//color = triangles[closestIntersection.triangleIndex].color * color;
				color = triangles[closestIntersection.triangleIndex].color * (m_DirectLight(closestIntersection) + color);
			}

			sdlAux->putPixel(x, y, color);
		}
	}
	cout << "ready_to_ render" << endl;
	sdlAux->render();
}


// VertexShader from lab3.
void PhotonShader(const vec3& photon, ivec2& pixel)
{
	vec3 pp = R * (photon - cameraPos);
	pixel.x = focalLength * pp.x / pp.z + SCREEN_WIDTH / 2;
	pixel.y = focalLength * pp.y / pp.z + SCREEN_HEIGHT / 2;
}

vec3 m_DirectLight(const m_Intersection& i) {
	vec3 dl;
	vec3 dir = lightPos - i.position;
	float distance = glm::length(dir);
	vec3 r = glm::normalize(dir);
	m_Intersection Shadows;
	if (m_ClosestIntersection(lightPos, -r, triangles, Shadows) && Shadows.triangleIndex != i.triangleIndex) {
		return vec3(0, 0, 0);
	}
	vec3 n = triangles[i.triangleIndex].normal;
	vec3 P = lightColor;
	dl = P * (glm::dot(r, n) > 0 ? glm::dot(r, n) : 0) * float(1 / (4 * M_PI * distance * distance));
	return dl;
}

bool m_ClosestIntersection(
	vec3 start,
	vec3 dir,
	const vector<Triangle>& triangles,
	m_Intersection& closestIntersection) {
	bool hasBeenHitted = 0;
	for (int i = 0; i < triangles.size(); i++) {
		vec3 e1 = triangles[i].v1 - triangles[i].v0;
		vec3 e2 = triangles[i].v2 - triangles[i].v0;
		vec3 b = start - triangles[i].v0;
		mat3 A(-dir, e1, e2);
		vec3 x = glm::inverse(A) * b; //x = (tuv).T
		if (x.y >= 0 && x.z >= 0 && x.y + x.z <= 1 && x.x >= 0 && (!hasBeenHitted || x.x < closestIntersection.distance))
		{
			//cout << "hit" << endl;
			hasBeenHitted = true;
			closestIntersection.position = start + x.x * dir;
			closestIntersection.distance = x.x;
			closestIntersection.triangleIndex = i;
		}
	}
	if (hasBeenHitted) {
		return true;
	}
	return false;
}





