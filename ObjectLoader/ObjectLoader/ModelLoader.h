#include <xstring>
#include <string>
#pragma once

#ifdef DLL_CLASS_EXPORTS

#else
#define DLL_CLASS_EXPORTS __declspec(dllimport)

#endif // DLL_CLASS_EXPORTS

enum DATA_SPECIFIER
{
	TEXTURE = 1,
	VERTICES,
	VERTEX_INDICES,
	NORMALS,
	NORMAL_INDICES,
	COORDS,
};

struct xMesh
{
	int numVertices;
	int numNormals;
	int numTextureCoords;
	int numVertexIndices;
	int numNormalIndices;
	char* textureFilename = nullptr;

	unsigned int textureID;

	float* vertices = nullptr;
	float* normals = nullptr;
	float* textureCoords = nullptr;
	unsigned short* vertexIndices = nullptr;
	unsigned short* normalIndices = nullptr;
};

class DLL_CLASS_EXPORTS CLoadX
{
public:
	CLoadX() {}
	~CLoadX() {}

	//Loads data from file to buffer
	void loadFile();

	//Parses buffer and sends data to renderer
	void parseBuffer(std::string & line, xMesh** xmesh);

private:

	//Structure used to compress mesh data
	xMesh* m_XModel = new xMesh();

	//Helps to open gateways and classify data
	int m_currentData = NULL;

	//Takes control of allocating data in its respective place
	int m_iterator = NULL;

	//Takes control of enlisting data in console (2D)
	char m_component3D = 120;

	//Takes control of enlisting data in console (2D)
	char m_component2D = 117;
};

