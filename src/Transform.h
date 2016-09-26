#pragma once
#include "Manager.h"
#include "random"
#include <stack>

class Manager;
class Transform : QObject{

public:
	void init(Manager *manager);
	void inline sampleSquare(int x, int y, int size, float offset);
	void inline sampleDiamond(int x, int y, int size, float offset);
	void diamondSquare(int size);
	void fluid();
	int inline getValue(int x, int y);
	void inline setValue(int x, int y, uint val);
	int cellIndex(int x, int y);
	void inline divide(int size);
	void increaseValue(int x, int y, uint val);
	void decreaseValue(int x, int y, uint val);
	uint randomUInt(uint min, uint max);
	float randomFloat(float min, float max);
	static const int size = ((1024+2) * (1024+2));
	float u[size], v[size], u_prev[size], v_prev[size];
	float dens[size], dens_prev[size];

private:
	QElapsedTimer eTimer;
	Manager *M;
	uint *values;
	float diamond[1025 * 1025];
	std::stack<std::pair<int, int>> pointStack;

	void inline add_source(int N, float * x, float * s, float dt);
	void inline diffuse(int N, int b, float * x, float * x0, float diff, float dt);
	void inline advect(int N, int b, float * d, float * d0, float * u, float *v, float dt);
	void inline dens_step(int N, float * x, float * x0, float * u, float * v, float diff, float dt);
	void inline project(int N, float * u, float * v, float * p, float * div);
	void inline vel_step(int N, float * u, float * v, float * u0, float * v0, float visc, float dt);
	void inline set_bnd(int N, int b, float * x);
	//std::random_device r;
	//std::uniform_int_distribution<uint> dis;
};