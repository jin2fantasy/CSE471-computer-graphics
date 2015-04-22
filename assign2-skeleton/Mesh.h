// Mesh class; used for mesh I/O
#include <fstream>
#include <assert.h>
#include "Angel.h"

#define __DEBUG__0

class Mesh {
private:
	std::ifstream input;
	std::ofstream output;
public:
	unsigned int nVertices, nFaces;
	float* vertices, *normalSmooth, *normalFlat;
	unsigned int* faces;
	float centerX = 0, centerY = 0, centerZ = 0;

	Mesh(const char* filename)
	{
		input.open(filename);
		if (!input.is_open()) assert(false);
		
		char temp[10];
		input >> temp;
		if (strcmp(temp, "OFF") == 0) {
			input >> nVertices >> nFaces >> temp;
			vertices = new float[3 * nVertices];
			normalSmooth = new float[3 * nVertices](); // initialize as 0
			normalFlat = new float[3 * nVertices];
			faces = new unsigned int[3 * nFaces];
			for (int i = 0; i < nVertices; i++) 
				input >> vertices[3 * i] >> vertices[3 * i + 1] >> vertices[3 * i + 2];
			for (int i = 0; i < nFaces; i++)
				input >> temp >> faces[3 * i] >> faces[3 * i + 1] >> faces[3 * i + 2];
		}
		for (int i = 0; i < nVertices; i++) {
			centerX += vertices[3 * i];
			centerY += vertices[3 * i + 1];
			centerZ += vertices[3 * i + 2];
		}
		centerX /= nVertices;
		centerY /= nVertices;
		centerZ /= nVertices;

		input.close();

#ifdef __DEBUG__
		input.open("normal_smooth_data.txt");
		if (!input.is_open()) {
			calculateVertexNormal();
			output.open("normal_smooth_data.txt");
			for (int i = 0; i < nVertices; i++)
				output << normalSmooth[3 * i] << " " << normalSmooth[3 * i + 1] << " " << normalSmooth[3 * i + 2] << std::endl;
			output.close();
			output.open("normal_flat_data.txt");
			for (int i = 0; i < nVertices; i++)
				output << normalFlat[3 * i] << " " << normalFlat[3 * i + 1] << " " << normalFlat[3 * i + 2] << std::endl;
			output.close();
		}
		else {
			for (int i = 0; i < nVertices; i++)
				input >> normalSmooth[3 * i] >> normalSmooth[3 * i + 1] >> normalSmooth[3 * i + 2];
			input.close();
			input.open("normal_flat_data.txt");
			for (int i=0; i<nVertices;i++)
				input >> normalFlat[3 * i] >> normalFlat[3 * i + 1] >> normalFlat[3 * i + 2];
			input.close();
		}
#else
		calculateVertexNormal();
#endif
	}

	~Mesh()
	{
		delete vertices;
		delete faces;
		delete normalFlat;
		delete normalSmooth;
	}

	void calculateVertexNormal();
};

void Mesh::calculateVertexNormal()
{
	vec3* faceNormals;
	faceNormals = new vec3[nFaces];
	for (int i = 0; i < nFaces; i++) {
		vec3 u(vertices[faces[3 * i + 1] * 3] - vertices[faces[3 * i] * 3],
			vertices[faces[3 * i + 1] * 3 + 1] - vertices[faces[3 * i] * 3 + 1],
			vertices[faces[3 * i + 1] * 3 + 2] - vertices[faces[3 * i] * 3 + 2]);
		vec3 v(vertices[faces[3 * i + 2] * 3] - vertices[faces[3 * i + 1] * 3],
			vertices[faces[3 * i + 2] * 3 + 1] - vertices[faces[3 * i + 1] * 3 + 1],
			vertices[faces[3 * i + 2] * 3 + 2] - vertices[faces[3 * i + 1] * 3 + 2]);
		faceNormals[i] = normalize(cross(u, v));
	}

	for (int i = 0; i < nFaces; i++) {
		normalFlat[faces[3 * i] * 3] = normalFlat[faces[3 * i + 1] * 3] = normalFlat[faces[3 * i + 2] * 3] = faceNormals[i].x;
		normalFlat[faces[3 * i] * 3 + 1] = normalFlat[faces[3 * i + 1] * 3 + 1] = normalFlat[faces[3 * i + 2] * 3 + 1] = faceNormals[i].y;
		normalFlat[faces[3 * i] * 3 + 2] = normalFlat[faces[3 * i + 1] * 3 + 2] = normalFlat[faces[3 * i + 2] * 3 + 2] = faceNormals[i].z;
	}

	for (int i = 0; i < 3 * nFaces; i++) {
		normalSmooth[3 * faces[i]] += faceNormals[i / 3].x;
		normalSmooth[3 * faces[i] + 1] += faceNormals[i / 3].y;
		normalSmooth[3 * faces[i] + 2] += faceNormals[i / 3].z;
		}
	for (int i = 0; i < nVertices; i++) {
		float mag = std::sqrt(normalSmooth[3 * i] * normalSmooth[3 * i]
			+ normalSmooth[3 * i + 1] * normalSmooth[3 * i + 1]
			+ normalSmooth[3 * i + 2] * normalSmooth[3 * i + 2]);
		normalSmooth[3 * i] /= mag;
		normalSmooth[3 * i + 1] /= mag;
		normalSmooth[3 * i + 2] /= mag;
	}

	delete faceNormals;
}