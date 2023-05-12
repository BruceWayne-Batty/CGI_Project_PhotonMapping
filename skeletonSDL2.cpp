//DH2323 skeleton code, Lab2 (SDL2 version)
#include <iostream>
#include <glm/glm.hpp>
#include "SDL2auxiliary.h"
#include "TestModel.h"
#include "PhotonTracing.h"
#include "kDtree.h"

using namespace std;
using glm::vec3;
using glm::ivec2;
using glm::mat3;

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

// ----------------------------------------------------------------------------
// FUNCTIONS

void Update(void);
void Draw(void);
void PhotonShader(const vec3& photon, ivec2& pixel);

int main( int argc, char* argv[] )
{
	sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
	t = SDL_GetTicks();	// Set start value for timer.
	LoadTestModel(triangles);

	photonTracer.SetScene(triangles);
	photonTracer.SetPhotons(photons);
	photonTracer.Emit();
	photons = photonTracer.GetPhotons();

	for (const auto& element : photons)
	{
		std::cout << "(" << element.direction.x << ", " << element.direction.y << ", " << element.direction.z << ")" << std::endl;
	}

	///* Test for kDtree. */
	//Photon test(vec3(0, 0, 0), vec3(1, 1, 1));
	///* Generate photonMap. */
	//kdt::KDTree<Photon> photonMap(photons);
	///* Nearest neighbour. */
	//int idx = photonMap.nnSearch(test);
	//cout << "Nearest neighbour is (" << photons[idx].position.x << ", " << photons[idx].position.y << ", " << photons[idx].position.z << ")" << endl;
	///* K nearest neighbour. */
	//int k = 5;
	//vector<int> knn = photonMap.knnSearch(test, k);
	//for (int i = 0; i < knn.size(); ++i)
	//{
	//	cout << "The " << i << " neighbour is (" << photons[knn[i]].position.x << ", " << photons[knn[i]].position.y << ", " << photons[knn[i]].position.z << ")" << endl;
	//}

	while (!sdlAux->quitEvent())
	{
		Update();
		Draw();
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

// VertexShader from lab3.
void PhotonShader(const vec3& photon, ivec2& pixel)
{
	vec3 pp = R * (photon - cameraPos);
	pixel.x = focalLength * pp.x / pp.z + SCREEN_WIDTH / 2;
	pixel.y = focalLength * pp.y / pp.z + SCREEN_HEIGHT / 2;
}