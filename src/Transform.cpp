#include "Transform.h"
#include "Manager.h"
#include "qmath.h"

#define IX(i, j) ((i)+(N + 2)*(j))


void Transform::init(Manager *manager) {
	M = manager;
	values = (uint*)M->PW->hImage.bits();
	for (int i = 0; i < size; i++) {
		u[i] = 0.f;
		u_prev[i] = 0.f;
		v[i] = 0.f;
		v_prev[i] = 0.f;
	}

}


int Transform::getValue(int x, int y) {
	static int size = M->ter->getTSize();
	return values[(x & (size - 1)) + (y&(size - 1)) * M->ter->getTSize()] & 0xFFFF;
}

void Transform::setValue(int x, int y, uint val) {
	static int size = M->ter->getTSize();
	if (val > 0xFFFF) val = 0xFFFF;
	values[(x & (size - 1)) + (y&(size - 1)) * M->ter->getTSize()] = 0xFF000000 | val;
}

void Transform::increaseValue(int x, int y, uint val) {
	uint temp = getValue(x, y);

	if (val > 0xFFFF) val = 0xFFFF;

	if (val + temp > 0xFFFF) setValue(x, y, 0xFFFF);
	else setValue(x, y, val + temp);
}

void Transform::decreaseValue(int x, int y, uint val) {
	uint temp = getValue(x, y);

	if (val > 0xFFFF) val = 0xFFFF;

	if (temp - val > 0xFFFF) setValue(x, y, 0x0);
	else setValue(x, y, temp - val);
}

int Transform::cellIndex(int x, int y) {
	if (x > 1024) x = 1024;
	else if (x < 0) x = 0;
	if (y > 1024) y = 1024;
	else if (y < 0) y = 0;

	return (x + y* 1025);
}

void Transform::sampleSquare(int x, int y, int size, float offset)
{

	// a     b 
	//
	//    x
	//
	// c     d

	float a = diamond[cellIndex(x - size, y - size)];
	float b = diamond[cellIndex(x - size, y + size)];
	float c = diamond[cellIndex(x + size, y - size)];
	float d = diamond[cellIndex(x + size, y + size)];

	float avg = (a + b + c + d) / 4.0;
	float result = avg + offset;
	
	//printf("X,Y: %d, %d\n", x, y);
	//printf("offset: %f\n", offset);
	//printf("avg: %f\n", avg);
	//printf("result: %f\n", result);
	//Sleep(500);
	//printf("a: %f\n", a);
	//printf("b: %f\n", b);
	//printf("c: %f\n", c);
	//printf("d: %f\n", d);
	//printf("offset: %f\n", offset);
	//printf("Result: %f\n", result);
	//if (result > 1.f) result = 1.f;
	//if (diamond[cellIndex(x, y)] <= 0)
	diamond[cellIndex(x, y)] = result;
}

void Transform::sampleDiamond(int x, int y, int size, float offset)
{
	//printf("VAL: %X\n", value);

	//   c
	//
	//a  x  b
	//
	//   d
	float avg;
	float result;

	if (x == 0) {
		float a = diamond[cellIndex(x, y - size)];
		float b = diamond[cellIndex(x + size, y)];
		float c = diamond[cellIndex(x, (y + size))];
		avg = (a + b + c) / 3.0f;
		result = avg + offset;
	}else if (x == 1024) {
		float a = diamond[cellIndex(x, y - size)];
		float c = diamond[cellIndex(x, (y + size))];
		float d = diamond[cellIndex((x - size), y)];
		avg = (a + c + d) / 3.0f;
		result = avg + offset;
	}else if (y == 0) {
		float b = diamond[cellIndex(x + size, y)];
		float c = diamond[cellIndex(x, (y + size))];
		float d = diamond[cellIndex((x - size), y)];
		avg = (b + c + d) / 3.0f;
		result = avg + offset;
	}else if (y == 1024) {
		float a = diamond[cellIndex(x, y - size)];
		float b = diamond[cellIndex(x + size, y)];
		float d = diamond[cellIndex((x - size), y)];
		avg = (a + b + d) / 3.0f;
		result = avg + offset;
	}
	else {
		float a = diamond[cellIndex(x, y - size)];
		float b = diamond[cellIndex(x + size, y)];
		float c = diamond[cellIndex(x, (y + size))];
		float d = diamond[cellIndex((x - size), y)];
		avg = (a + b + c + d) / 4.0f;
		result = avg + offset;
	}
	//if (result > 1.f) result = 1.f;
	//if (diamond[cellIndex(x, y)] <= 0)
		diamond[cellIndex(x, y)] = result;
}

void Transform::divide(int size) {
	float scale = 1.f;

	size -= 1;
	while (size>=2) {
		int step = size / 2;
		for (int y = step; y < 1024; y+=size) {
			for (int x = step; x < 1024; x += size) {
				sampleSquare(x, y, step, randomFloat(0, scale*2) - scale);
			}
		}

		for (int y = 0; y <= 1024; y += size) {
			for (int x = step; x < 1024; x += size) {
				sampleDiamond(x, y, step, randomFloat(0,scale*2) - scale);
			}
		}

		for (int y = step; y < 1024; y += size) {
			for (int x = 0; x <= 1024; x += size) {
				sampleDiamond(x, y, step, randomFloat(0,scale*2) - scale);
			}
		}
		printf("scale: %f\n", scale);
		scale /= 2;
		size /= 2;
	}
	/*
	if (size == 1025) {
		sampleDiamond(512, 0, half, rand() / (RAND_MAX + 1.) * scale);
		sampleDiamond(512, 1024, half, rand() / (RAND_MAX + 1.) * scale);
		sampleDiamond(0, 512, half, rand() / (RAND_MAX + 1.) * scale);
		sampleDiamond(1024, 512, half, rand() / (RAND_MAX + 1.) * scale);
	}
	else {
		for (int y = 0; y <= 1024; y += half) {
			for (int x = (y + (int)half) % size; x <= 1024; x += size) {
				//printf("X %d, %d \n", x, y);
				//Sleep(1000);
				sampleDiamond(x, y, half, rand() / (RAND_MAX + 1.) * scale);
			}
		}
	}
	*/
	//printf("END: Size: %d, Half: %d\n", size, half);
	//for (int i = 0; i < M->ter->getTSize()*M->ter->getTSize(); i++) {
	//	values[i] = 0xFF000000 | diamond[i + i / 1024];
	//}
	//M->refreshHmap = true;
	//Sleep(2000);
	//divide(size / 2);
}

void Transform::diamondSquare(int size)
{
	float max = 0.5f;
	for (int i = 0; i < size*size; i++) {
		diamond[i] = 0.f;
		if (i % 1025 == 0 || (i) % (1025) == 1024 || i < 1025 || i > 1025 * 1024) diamond[i] = 0.0f;
	}
	diamond[0] = max;
	diamond[cellIndex(0,1024)] = max;
	diamond[cellIndex(1024, 1024)] = max;
	diamond[cellIndex(1024, 0)] = max;

	divide(size);
	values = (uint*)M->PW->hImage.bits();

	printf("MID: %f\n", diamond[cellIndex(512, 512)]);
	float maxValue = 1.f;
	float minValue = -1.f;
	for (int i = 0; i < 1025 * 1025; i++) {
		if (diamond[i] < minValue)minValue = diamond[i];
	}
	//minValue = qAbs(minValue);
	//for (int i = 0; i < 1025 * 1025; i++) {
	//	diamond[i] += minValue;
	//}
	for (int i = 0; i < 1025 * 1025; i++) {
		if (diamond[i] > maxValue) maxValue = diamond[i];
	}
	//for (int x = 0; x < M->ter->getTSize(); x++) {
	//	for (int y = 0; y < M->ter->getTSize(); y++) {
	//		//printf("VALUES: %X\n", diamond[cellIndex(x, y)]);
	//		setValue(x, y, (uint)((diamond[cellIndex(x, y)]/maxValue)*(uint)(0xFFFF)));
	//		
	//	}
	//}
	for (int i = 0; i < 1024 * 1024; i++) {
		float val = diamond[i + i / 1024];
		if(val <0 ) values[i] = 0xFFF00000 | (0xFFFF - (uint)(diamond[i + i/1024]*-1 * 0xFFFF));
		else if (val >=0) values[i] = 0xFF000000 | (uint)(diamond[i + i / 1024]/maxValue * 0xFFFF);
	}
	printf("MID: %X\n", values[512 + 1024*512]);
	//printf("MID2: %d\n", cellIndex(512, 512));
}

float Transform::randomFloat(float min, float max) {
	static std::random_device rd;     // only used once to initialise (seed) engine
	static std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_real_distribution<float> uni(min, max); // guaranteed unbiased

	return uni(rng);





	std::uniform_int_distribution<uint> unif(min, max);
	static std::mt19937 re(1293847);
	//re.seed(rand());
	return unif(re);
}

uint Transform::randomUInt(uint min, uint max) {
	static std::random_device rd;     // only used once to initialise (seed) engine
	static std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<uint> uni(min, max); // guaranteed unbiased

	return uni(rng);





	std::uniform_int_distribution<uint> unif(min, max);
	static std::mt19937 re(1293847);
	//re.seed(rand());
	return unif(re);
}

void Transform::fluid() {
	int N = M->ter->getTSize();
	float visc = 0.1f;
	float diff = 1.f;
	float dt = 50.f;
	static bool init = true;

	//vel_step(N, u, v, u_prev, v_prev, visc, dt);
	dens_step(N, dens, dens_prev, u, v, diff, dt);
	for (int i = 1; i < M->ter->getTSize() * M->ter->getTSize(); i++) {
		//printf("VALUE1 %f\n", dens[i]);
		//printf("VALUE2 %X\n", (uint)(dens[i] * 0xFFFF));
		//printf("VALUE3 %X\n", 0xFF000000 | (uint)(dens[i] * 0xFFFF));
		values[i] = 0xFF000000 | (uint)(dens[i] * 0xFFFF);
	}
	M->refreshHmap = true;
}

void Transform::add_source(int N, float * x, float * s, float dt)
{

	int i, size = (N + 2)*(N + 2);
	for (i = 0; i < size; i++) x[i] += dt*s[i];
}

void Transform::diffuse(int N, int b, float * x, float * x0, float diff, float dt)
{
	int i, j, k;
	float a = dt*diff*N*N;
	for (k = 0; k < 20; k++) {
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				x[IX(i, j)] = (x0[IX(i, j)] + a*(x[IX(i - 1, j)] + x[IX(i + 1, j)] +
					x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
			}
		}
		set_bnd(N, b, x);
	}
}

void Transform::advect(int N, int b, float * d, float * d0, float * u, float *v, float dt)
{
	int i, j, i0, j0, i1, j1;
	float x, y, s0, t0, s1, t1, dt0;
	dt0 = dt*N;
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			x = i - dt0*u[IX(i, j)];
			y = j - dt0*v[IX(i, j)];
			if (x < 0.5) x = 0.5;
			if (x > N + 0.5)x = N + 0.5;
			i0 = (int)x;
			i1 = i0 + 1;

			if (y < 0.5)y = 0.5;
			if (y > N + 0.5) y = N + 0.5;
			j0 = (int)y;
			j1 = j0 + 1;
			s1 = x - i0;
			s0 = 1 - s1;
			t1 = y - j0;
			t0 = 1 - t1;
			d[IX(i, j)] = s0*(t0*d0[IX(i0, j0)] + t1*d0[IX(i0, j1)]) + s1*(t0*d0[IX(i1, j0)] + t1*d0[IX(i1, j1)]);
		}
	}
	set_bnd(N, b, d);
}

void Transform::dens_step(int N, float * x, float * x0, float * u, float * v, float diff, float dt)
{
	//eTimer.start();
	//printf("START: %d\n", eTimer.elapsed());
	add_source(N, x, x0, dt);
	//printf("After Add Source: %d\n", eTimer.elapsed());
	std::swap(x0, x);
	//printf("After Add Swap: %d\n", eTimer.elapsed());
	diffuse(N, 0, x, x0, diff, dt);
	//printf("After Add Diffuse: %d\n", eTimer.elapsed());
	std::swap(x0, x); advect(N, 0, x, x0, u, v, dt);
	//printf("After Advect: %d\n", eTimer.elapsed());
}

void Transform::project(int N, float * u, float * v, float * p, float * div)
{
	int i, j, k;
	float h;
	h = 1.0 / N;
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			div[IX(i, j)] = -0.5*h*(u[IX(i + 1, j)] -
				u[IX(i - 1, j)] + v[IX(i, j + 1)] - v[IX(i, j - 1)]);
			p[IX(i, j)] = 0;
		}
	}
	set_bnd(N, 0, div);
	set_bnd(N, 0, p);
	for (k = 0; k < 20; k++) {
		for (i = 1; i <= N; i++) {
			for (j = 1; j <= N; j++) {
				p[IX(i, j)] = (div[IX(i, j)] + p[IX(i
					- 1, j)] + p[IX(i + 1, j)] +
					p[IX(i, j - 1)] + p[IX(i, j + 1)]) / 4;
			}
		}
		set_bnd(N, 0, p);
	}
	for (i = 1; i <= N; i++) {
		for (j = 1; j <= N; j++) {
			u[IX(i, j)] -= 0.5*(p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
			v[IX(i, j)] -= 0.5*(p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
		}
	}
	set_bnd(N, 1, u); set_bnd(N, 2, v);
}

void Transform::vel_step(int N, float * u, float * v, float * u0, float * v0, float visc, float dt)
{
	eTimer.start();
	printf("START: %d\n", eTimer.elapsed());
	add_source(N, u, u0, dt); add_source(N, v, v0, dt);
	printf("After Add Source: %d\n", eTimer.elapsed());
	std::swap(u0, u); diffuse(N, 1, u, u0, visc, dt);
	std::swap(v0, v); diffuse(N, 2, v, v0, visc, dt);
	printf("After Swap: %d\n", eTimer.elapsed());
	project(N, u, v, u0, v0);	
	printf("Aftr Project: %d\n", eTimer.elapsed());
	std::swap(u0, u);
	std::swap(v0, v);
	printf("After Swap: %d\n", eTimer.elapsed());
	advect(N, 1, u, u0, u0, v0, dt); advect(N, 2, v, v0, u0, v0, dt);
	printf("After Advect: %d\n", eTimer.elapsed());
	project(N, u, v, u0, v0);
	printf("After Project: %d\n", eTimer.elapsed());
}

void Transform::set_bnd(int N, int b, float * x)
{
	int i;
	for (i = 1; i <= N; i++) {
		x[IX(0, i)] = b == 1 ? -(x[IX(1, i)]) : x[IX(1, i)];
		x[IX(N + 1, i)] = b == 1 ? (-x[IX(N, i)]) : x[IX(N, i)];
		x[IX(i, 0)] = b == 2 ? (-x[IX(i, 1)]) : x[IX(i, 1)];
		x[IX(i, N + 1)] = b == 2 ? (-x[IX(i, N)]) : x[IX(i, N)];
	}
	x[IX(0, 0)] = 0.5*(x[IX(1, 0)] + x[IX(0, 1)]);
	x[IX(0, N + 1)] = 0.5*(x[IX(1, N + 1)] + x[IX(0, N)]);
	x[IX(N + 1, 0)] = 0.5*(x[IX(N, 0)] + x[IX(N + 1, 1)]);
	x[IX(N + 1, N + 1)] = 0.5*(x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}


using namespace std;

class OlsenNoise2D
{

public:
	int * olsennoise(int x, int y, int width, int height);

private:
	int hashrandom(std::vector<long long int> elements);
	long long hash(long long v);

};

int * OlsenNoise2D::olsennoise(int x, int y, int width, int height)
{
	int maxiterations = 7;
	int cx, cy;
	int cxh, cyh;
	int cwidth, cheight;
	int xoff, yoff;
	int nwidth, nheight;
	int nx, ny;
	int nxh, nyh;
	int m = 0;
	int n = 0;
	int fieldwidth = 0;
	int fieldheight = 0;

	int * field = NULL;

	for (int iteration = 0; iteration < maxiterations; iteration++)
	{
		nx = x;
		ny = y;

		nxh = x + width;
		nyh = y + height;

		for (int i = 1, n = maxiterations - iteration; i < n; i++)
		{
			nx = (nx / 2) - 1;
			ny = (ny / 2) - 1;
			nxh = 1 - (-nxh / 2);
			nyh = 1 - (-nyh / 2);
		}

		xoff = -2 * ((nx / 2)) + nx + 1;
		yoff = -2 * ((ny / 2)) + ny + 1;

		cx = (nx / 2) - 1;
		cy = (ny / 2) - 1;
		cxh = 1 - (-nxh / 2);
		cyh = 1 - (-nyh / 2);

		nwidth = nxh - nx;
		nheight = nyh - ny;

		cwidth = cxh - cx;
		cheight = cyh - cy;

		/// rest
		fieldwidth = cwidth;
		fieldheight = cheight;

		/// Only happens once
		if (field == NULL)
		{
			/// allocate memory
			field = new int[cheight * cwidth];

			/// blank value
			for (int x = 0; x < cwidth; x++)
			{
				for (int y = 0; y < cheight; y++)
				{
					field[x + (y*cwidth)] = 0;
				}
			}
		}

		/// First loop
		for (int j = 0, m = cwidth; j < m; j++)
		{
			for (int k = 0, n = cheight; k < n; k++)
			{
				field[j + (k*m)] += (hashrandom({ cx + j, cy + k, iteration }) & (1 << (7 - iteration)));
			}
		}

		/// Up sampled
		int * upsampled = new int[(fieldwidth * 2)*(fieldheight * 2)];
		long int upsampledsize = (fieldwidth * 2)*(fieldheight * 2);

		for (int j = 0, m = fieldwidth * 2; j < m; j++)
		{
			for (int k = 0, n = fieldheight * 2; k < n; k++)
			{
				upsampled[j + (k*m)] = field[(j / 2) + ((k / 2)*fieldwidth)];
			}
		}

		/// Allocate upsampled to field
		delete[] field;
		field = upsampled;

		/// Copy new fieldwieght and fieldheight
		fieldwidth = fieldwidth * 2;
		fieldheight = fieldheight * 2;

		/// Blur field
		int * blurfield = new int[(fieldwidth - 2)*(fieldheight - 2)];
		long int blurfieldsize = (fieldwidth - 2)*(fieldheight - 2);

		for (int j = 0, m = fieldwidth - 2; j < m; j++)
		{
			for (int k = 0, n = fieldheight - 2; k < n; k++)
			{
				for (int h = 0; h < 9; h++)
				{
					blurfield[j + (k*m)] += field[(j + (h % 3)) + ((k + (h / 3))*fieldwidth)];
				}
				blurfield[j + (k*m)] /= 9;
			}
		}

		/// Allocate blurfield to field
		delete[] field;
		field = blurfield;

		/// Copy new fieldwieght and fieldheight
		fieldwidth = fieldwidth - 2;
		fieldheight = fieldheight - 2;

		/// Trim field
		int * trimfield = new int[nwidth*nheight];
		long int trimfieldsize = nwidth*nheight;

		for (int j = 0, m = nwidth; j < m; j++)
		{
			for (int k = 0, n = nheight; k < n; k++)
			{
				trimfield[j + (k*m)] = field[(j + xoff) + ((k + yoff)*fieldwidth)];
			}
		}

		/// Allocate trimfield to field
		delete[] field;
		field = trimfield;

	}

	return field;
}

int OlsenNoise2D::hashrandom(std::vector<long long int> elements)
{
	long long hashcalc = 0;


	for (int i = 0; i < elements.size(); i++)
	{
		hashcalc ^= elements[i];
		hashcalc = hash(hashcalc);
	}
	return (int)hashcalc;
};

long long OlsenNoise2D::hash(long long v)
{
	long long hash = v;
	long long h = hash;

	switch ((int)hash & 3)
	{
	case 3:
		hash += h;
		hash ^= hash << 32;
		hash ^= h << 36;
		hash += hash >> 22;
		break;
	case 2:
		hash += h;
		hash ^= hash << 22;
		hash += hash >> 34;
		break;
	case 1:
		hash += h;
		hash ^= hash << 20;
		hash += hash >> 2;
	}
	hash ^= hash << 6;
	hash += hash >> 10;
	hash ^= hash << 8;
	hash += hash >> 34;
	hash ^= hash << 50;
	hash += hash >> 12;
	return hash;
};