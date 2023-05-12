#ifndef PHOTONTRACING
#define PHOTONTRACING

#include "TestModel.h"

extern const int MAX_PHOTONS;
extern const int MAX_PHOTONTRACING_DEPTH;
extern const float MAX_VALUE;

class PhotonTracing
{
private:
	std::vector<Triangle> triangles;
	std::vector<Photon> photons;

	bool Reflection(Photon photon, int depth, int triIndex);
	bool Diffusion(Photon photon, int depth, int triIndex);
	bool Refraction(Photon photon, int depth, int triIndex);
	void Tracing(Photon photon, int depth);

public:
	void SetScene(std::vector<Triangle> input);
	void SetPhotons(std::vector<Photon> input);
	std::vector<Photon> GetPhotons();
	void Emit();
};

#endif // !PHOTONTRACING
