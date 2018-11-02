#pragma once
#include "CApp.h"
#include "CHexaGrid.h"
#include "C3DModel.h"
#include "Globals.h"
#include "CTextureManager.h"
#include "CWideStringHelper.h"

#include <Vector>
#pragma once

class CAppGame : public CApp
{
public:
	CAppGame();
	CAppGame(int window_width, int window_height);
	~CAppGame();

	// Method to initialize any objects for this class
	void initialize();

	// Method to update any objecs based on time elapsed since last frame
	void update(double deltaTime);

	// Method to run the app
	void run();

	// Method to render the app
	void render();

	// Method to render the polygon
	void create();

protected:

	// Method to initialize the menu 
	bool initializeMenu() { return false; }

private:

	//Shader ID
	unsigned int m_shaderID;

	//Array Object ID
	unsigned int m_geoVAOID;

	//Verifies if renderer is initialized
	bool m_initialized;

	//Verifies if mesh is loaded
	bool m_loaded;

	// Current delta time (time of the current frame - time of the last frame)
	double m_currentDeltaTime;

	//  Object rotation speed (degrees per second)
	double m_rotationSpeed;

	// Current object rotation, expressed in degrees
	double m_objectRotation;

	// Current object position
	CVector3 m_objectPosition;

	//Pointer to the whole grid
	CHexaGrid * m_pGrid = nullptr;

	//Game object storage vector
	vector <CGameObject*> m_pGameObjects;

	//Mesh storage vector
	vector <C3DModel*> m_pMeshes;

	//Cell container vector
	vector <CHexaCell*> m_pCells;

	//Array that will carry all the vertex coordinates
	float* m_vertexArray = nullptr;

	//Array that will carry all the index coordinates
	unsigned short* m_indexArray = nullptr;

	//Array that will import all the extracted vertex coordinates
	float* m_vertexExtractor = new float [18];

	//Array that will import all the extracted index coordinates 
	unsigned short* m_indexExtractor = new unsigned short [12];

	float* m_normalArray = nullptr;

	unsigned short* m_normalIndices = nullptr;
};