#pragma once

#include <xstring>
#include <string>

#include "Globals.h"
#include "CApp.h"

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

// --------------------------------------------------------------------------------------------------------------------------------------
// Class that inherits from Base class CApp
// Base class CApp has members for: CGameWindow, CGameMenu, and COpenGLRenderer, which we can access through the public/protected methods
// --------------------------------------------------------------------------------------------------------------------------------------
class CAppX : public CApp
{
private:

	// ---------------------------------------------------
	// Private member variables specific to CAppEmpty only
	// ---------------------------------------------------
	//
	// ---------------------------------------------------

protected:

	// Method to initialize the menu 
	// (not needed in CAppEmpty, so return false)
	bool initializeMenu() { return false; }

public:
	// Constructors and destructor
	CAppX();
	CAppX(int window_width, int window_height);
	~CAppX();

	// --------------------------------------------------------------------------------------------------------------------------
	// Inherited methods from CApp
	// Initialize(), update(), run(), and render() are PURE VIRTUAL methods, so they need to be implemented in this DERIVED class
	// --------------------------------------------------------------------------------------------------------------------------

	// Method to initialize any objects for this class
	void initialize();

	// Method to update any objecs based on time elapsed since last frame
	void update(double deltaTime);

	// Method to run the app
	void run();

	// Method to render the app
	void render();


	// -----------------------------------------------------------------------------------------------------------------------
	// Other inherited methods from CApp. These are optional to override.
	// Only provide an implementation in this DERIVD class if you need them to do something different than the base class CApp
	// -----------------------------------------------------------------------------------------------------------------------

	// Executes the action corresponding to the selected menu item
	void executeMenuAction();

	// Called when mouse moves within the window
	void onMouseMove(float deltaX, float deltaY);

	// -----------------------------------------
	// Public methods specific to CAppEmpty only
	// -----------------------------------------
	//
	// -----------------------------------------

	//Loads data from file to buffer
	void loadFile();

	//Parses buffer and sends data to renderer
	void parseBuffer(string & line);

	//Shows 3D model on screen
	void createModel();

private:

	// ------------------------------------------
	// Private methods specific to CAppEmpty only
	// ------------------------------------------

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

	// Current object rotation, expressed in degrees
	double m_objectRotation;

	// Current object position
	CVector3 m_objectPosition;

	//  Object rotation speed (degrees per second)
	double m_rotationSpeed;

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

	// ------------------------------------------
};