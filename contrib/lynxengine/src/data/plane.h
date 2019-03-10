#ifndef plane_h
#define plane_h

#include <shared.h>

std::vector<float> vertices = std::vector<float>({
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f
});

std::vector<float> normals = std::vector<float>({
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
});

std::vector<float> uvcoords = std::vector<float>({
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,

	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
});

#endif