#include "../stdafx.h"

#include <iostream>
#include <iomanip>
using namespace std;

#include "../Include/Globals.h"
#include "../Include/CAppEmpty.h"
#include "../Include/CShapes.h"
#include "../Include/CWideStringHelper.h"

#define PI 3.14159265358979323846f
#define D_TO_R 0.0174533f

/* */
CAppShapes::CAppShapes() :
	CApp(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
	cout << "Constructor: CShapes()" << endl;

	// Initialize class member variables here
	// ======================================
	m_shaderID = 0;
	m_geoVAOID = 0;
	m_currentDeltaTime = 0;
	m_objectRotation = 0;
	m_objectPosition.X = 0;
	m_objectPosition.Y = 0;
	m_objectPosition.Z = 0;
	m_rotationSpeed = DEFAULT_ROTATION_SPEED;
	// ======================================
}

/* */
CAppShapes::CAppShapes(int window_width, int window_height) :
	CApp(window_width, window_height)
{
	cout << "Constructor: CShapes(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	m_shaderID = 0;
	m_geoVAOID = 0;
	m_currentDeltaTime = 0;
	m_objectRotation = 0;
	m_objectPosition.X = 0;
	m_objectPosition.Y = 0;
	m_objectPosition.Z = 0;
	m_rotationSpeed = DEFAULT_ROTATION_SPEED;
	// ======================================
}

/* */
CAppShapes::~CAppShapes()
{
	cout << "Destructor: ~CShapes()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	if (m_geoVAOID)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_shaderID, &m_geoVAOID);
	}
	// =================================================
}

/* */
void CAppShapes::initialize()
{
	// Initialize app-specific stuff here
	// ==================================
	std::wstring wresourceFilenameVS;
	std::wstring wresourceFilenameFS;
	std::string resourceFilenameVS;
	std::string resourceFilenameFS;

	char *vertexShaderToLoad = VERTEX_SHADER_3D_OBJECT;
	char *fragmentShaderToLoad = FRAGMENT_SHADER_3D_OBJECT;

	if (!CWideStringHelper::GetResourceFullPath(vertexShaderToLoad, wresourceFilenameVS, resourceFilenameVS) ||
		!CWideStringHelper::GetResourceFullPath(fragmentShaderToLoad, wresourceFilenameFS, resourceFilenameFS))
	{
		cout << "ERROR: Unable to find one or more resources: " << endl;
		cout << "  " << vertexShaderToLoad << endl;
		cout << "  " << fragmentShaderToLoad << endl;

		m_initialized = false;
	}

	getOpenGLRenderer()->createShaderProgram(
		&m_shaderID,
		resourceFilenameVS.c_str(),
		resourceFilenameFS.c_str()
	);

	//createPyramid();
	//createSphere();
	createTorus();
	// ==================================
}

/* */
void CAppShapes::run()
{
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		if (getGameWindow()->create(CAPP_PROGRA3_EMPTYAPP_WINDOW_TITLE))
		{
			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			// Initialize window width/height in the renderer
			getOpenGLRenderer()->setWindowWidth(getGameWindow()->getWidth());
			getOpenGLRenderer()->setWindowHeight(getGameWindow()->getHeight());

			initialize();

			// Enter main loop
			cout << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}

/* */
void CAppShapes::update(double deltaTime)
{
	double degreesToRotate = 0.0;

	if (deltaTime <= 0.0f)
	{
		return;
	}

	// Save current delta time
	m_currentDeltaTime = deltaTime;

	// Calculate degrees to rotate
	// ----------------------------------------------------------------------------------------------------------------------------------------
	// degrees = rotation speed * delta time 
	// deltaTime is expressed in milliseconds, but our rotation speed is expressed in seconds (convert delta time from milliseconds to seconds)
	degreesToRotate = m_rotationSpeed * (deltaTime / 1000.0);
	// accumulate rotation degrees
	m_objectRotation += degreesToRotate;

	// Reset rotation if needed
	while (m_objectRotation > 360.0)
	{
		m_objectRotation -= 360.0;
	}
	if (m_objectRotation < 0.0)
	{
		m_objectRotation = 0.0;
	}
}

/* */
void CAppShapes::render()
{
	CGameMenu *menu = getMenu();

	// If menu is active, render menu
	if (menu != NULL
		&& menu->isInitialized()
		&& menu->isActive())
	{
		//...
	}
	else // Otherwise, render app-specific stuff here...
	{
		// =================================
		float color[3] = { 1.0f, 1.0f, 1.0f };
		unsigned int noTexture = 0;

		// convert total degrees rotated to radians;
		double totalDegreesRotatedRadians = m_objectRotation * 3.1459 / 180.0;

		// Get a matrix that has both the object rotation and translation
		MathHelper::Matrix4 modelMatrix = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, m_objectPosition);

		CVector3 pos2 = m_objectPosition;
		pos2 += CVector3(3.0f, 0.0f, 0.0f);
		MathHelper::Matrix4 modelMatrix2 = MathHelper::ModelMatrix((float)totalDegreesRotatedRadians, pos2);

		if (m_geoVAOID > 0 && m_numShapeFaces > 0)
		{
			getOpenGLRenderer()->renderObject(
				&m_shaderID,
				&m_geoVAOID,
				&noTexture,
				m_numShapeFaces,
				color,
				&modelMatrix,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);
		}

		// =================================
	}
}

/* */
void CAppShapes::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void CAppShapes::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}

void CAppShapes::createPyramid()
{
	m_numShapeFaces = 6;

	m_loaded = false;

	float h = 2.5f;
	float halfZ = 1.5f;
	float halfX = 2.0f;

	float v1v3[3], v1v2[3], normal[3];

	//Vertices
	float vData[15] = {
		0.0, h, 0.0,
		-halfX, 0.0, halfZ,
		halfX, 0.0, halfZ,
		-halfX, 0.0, -halfZ,
		halfX, 0.0, -halfZ
	};

	//Vertex Indexes
	unsigned short vIndexes[18] = {
		0,1,2,
		0,2,4,
		0,4,3,
		0,3,1,
		1,4,2,
		4,3,1
	};

	//Normals
	float nData[18] = {
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0
	};

	//Normal Indexes
	unsigned short nIndexes[18] = {
		0,0,0,
		1,1,1,
		2,2,2,
		3,3,3,
		4,4,4,
		5,5,5
	};

	//Loading Mesh
	m_loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_shaderID, 
		&m_geoVAOID, 
		vData, //Vertices
		5, //Vertex Numbers
		nData, //Normals
		6, //Normal Numbers
		vData, //UV Coords
		5, //UV Coord Numbers
		vIndexes, //Vertex Indexes
		6, //Triangle Numbers
		nIndexes, //Normal Indexes
		6, //Normal Numbers
		vIndexes, //UV Coord Indexes
		6 //UV Coord Numbers
	);

	if (!m_loaded)
	{
		m_geoVAOID = 0;
	}
}

void CAppShapes::createSphere()
{
	int stacks = 10 ;
	int slices = 10;
	int currentVertex = 0;
	int currentFace = 0;
	int currentSlice = 0;

	m_loaded = false;

	m_numShapeFaces = ((stacks * 2) * (slices)) - (slices * 2);
	m_numShapeVertices = (stacks * (slices - 1)) + 2;

	cout << "Shape Faces: " << m_numShapeFaces << endl;
	cout << "Shape Vertices: " << m_numShapeVertices << endl;
	cout << "Calculated from " << stacks << " stacks and " << slices << " slices." << endl;

	float *vData = new float[m_numShapeVertices * 3];
	unsigned short *vIndex = new unsigned short[m_numShapeFaces * 3];

	for (int t = 1; t < stacks; t++) // stacks are ELEVATION so they count theta
	{
		for (int p = 0; p < slices; p++) // slices are SEPARATION so they count phi
		{
			float theta = ((float)(t) / stacks) * PI;
			float phi = ((float)(p) / slices) * 2 * PI; // azimuth goes around 0 .. 2*PI

			vData[(currentVertex * 3) + 0] = sinf(theta) * cosf(phi);	//x
			vData[(currentVertex * 3) + 1] = cosf(theta);				//y
			vData[(currentVertex * 3) + 2] = -sinf(theta) * sinf(phi);	//z
			cout << "Vertex " << currentVertex << " <" << vData[(currentVertex * 3) + 0] << "," << vData[(currentVertex * 3) + 1] << "," << vData[(currentVertex * 3) + 2] << ">" << endl;
			currentVertex++;
		}
	}

	//Top Vertex
	vData[(currentVertex * 3) + 0] = 0;
	vData[(currentVertex * 3) + 1] = 1;
	vData[(currentVertex * 3) + 2] = 0;
	cout << "Vertex " << currentVertex + 1 << " <" << vData[(currentVertex * 3) + 0] << "," << vData[(currentVertex * 3) + 1] << "," << vData[(currentVertex * 3) + 2] << ">" << endl;
	currentVertex++;

	//Bottom Vertex
	vData[(currentVertex * 3) + 0] = 0;
	vData[(currentVertex * 3) + 1] = -1;
	vData[(currentVertex * 3) + 2] = 0;
	cout << "Vertex " << currentVertex + 1 << " <" << vData[(currentVertex * 3) + 0] << "," << vData[(currentVertex * 3) + 1] << "," << vData[(currentVertex * 3) + 2] << ">" << endl;
	currentVertex++;

	//Triangulation
	for (int i = 0; i < m_numShapeVertices; i++)
	{
		if (i >= 0 && i < slices) //Orders top faces
		{
			if (i == slices - 1)
			{
				vIndex[(currentFace * 3) + 0] = 0;
				vIndex[(currentFace * 3) + 1] = i;
				vIndex[(currentFace * 3) + 2] = m_numShapeVertices - 2;

				cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
				currentFace++;
			}
			else
			{
				vIndex[(currentFace * 3) + 0] = i + 1;
				vIndex[(currentFace * 3) + 1] = i;
				vIndex[(currentFace * 3) + 2] = m_numShapeVertices - 2;

				cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
				currentFace++;
			}
		}

		else if (i >= stacks && i < m_numShapeVertices - 2) //Orders mid faces
		{
			if (currentSlice == slices - 1) //Last Vertex in Slice
			{
				vIndex[(currentFace * 3) + 0] = i;
				vIndex[(currentFace * 3) + 1] = i - (slices);
				vIndex[(currentFace * 3) + 2] = i - (slices + (slices - 1));

				cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
				currentFace++;

				vIndex[(currentFace * 3) + 0] = i;
				vIndex[(currentFace * 3) + 1] = i - 1;
				vIndex[(currentFace * 3) + 2] = i - slices;

				cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
				currentFace++;

				currentSlice = 0;
			}
			else if (currentSlice == 0) //First Vertex in Slice
			{
				vIndex[(currentFace * 3) + 0] = i;
				vIndex[(currentFace * 3) + 1] = i - (slices);
				vIndex[(currentFace * 3) + 2] = i - (slices - 1);

				cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
				currentFace++;

				vIndex[(currentFace * 3) + 0] = i;
				vIndex[(currentFace * 3) + 1] = i + (slices - 1);
				vIndex[(currentFace * 3) + 2] = i - slices;

				cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
				currentFace++;

				currentSlice++;
			}
			else //Vertex Between Slices
			{
				vIndex[(currentFace * 3) + 0] = i;
				vIndex[(currentFace * 3) + 1] = i - (slices);
				vIndex[(currentFace * 3) + 2] = i - (slices - 1);

				cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
				currentFace++;

				vIndex[(currentFace * 3) + 0] = i;
				vIndex[(currentFace * 3) + 1] = i - 1;
				vIndex[(currentFace * 3) + 2] = i - slices;

				cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
				currentFace++;

				currentSlice++;
			}
		}

		else //Orders bottom faces
		{
			for (int j = (m_numShapeVertices-1) - (slices + 1); j < m_numShapeVertices - 2; j++)
			{
				if (j < m_numShapeVertices - 3)
				{
					vIndex[(currentFace * 3) + 0] = m_numShapeVertices - 1;
					vIndex[(currentFace * 3) + 1] = j;
					vIndex[(currentFace * 3) + 2] = j+1;

					cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
					currentFace++;
				}
				else
				{
					vIndex[(currentFace * 3) + 0] = m_numShapeVertices - 1;
					vIndex[(currentFace * 3) + 1] = j;
					vIndex[(currentFace * 3) + 2] = j- (slices - 1);

					cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
					currentFace++;
				}
			}
			i = m_numShapeVertices;
		}
		//phi2  phi1
		// |     |
		// 2-----1 -- theta1
		// | \   |
		// |   \ |
		// 3-----4 -- theta2
	}

	//Normals
	float *nData = new float [m_numShapeFaces * 3];

	for (int i = 0; i < m_numShapeFaces * 3; i++)
	{
		nData[i] = 0.0f;
	}

	//Normal Indexes
	unsigned short *nIndex = new unsigned short[m_numShapeFaces * 3];

	for (int i = 0; i < m_numShapeFaces; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			nIndex[(i * 3) + j] = i;
		}
	}

	//Loading Mesh
	m_loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_shaderID,
		&m_geoVAOID,
		vData, //Vertices
		m_numShapeVertices, //Vertex Numbers
		nData, //Normals
		m_numShapeFaces, //Normal Numbers
		vData, //UV Coords
		m_numShapeVertices, //UV Coord Numbers
		vIndex, //Vertex Indexes
		m_numShapeFaces, //Triangle Numbers
		nIndex, //Normal Indexes
		m_numShapeFaces, //Normal Numbers
		vIndex, //UV Coord Indexes
		m_numShapeFaces //UV Coord Numbers
	);

	if (!m_loaded)
	{
		m_geoVAOID = 0;
	}
}

void CAppShapes::createTorus()
{
	//Shape Variables
	int radials = 5;
	int rings = 5;

	float radius;

	//Iterative Variables
	int currentVertex = 0;
	int currentFace = 0;
	int currentSlice = 0;
	float currentRing = 0;
	float currentRadial = 0;
	float zValue;

	m_loaded = false;

	//Class Value Assignation
	m_numShapeFaces = radials * rings * 2;
	m_numShapeVertices = radials * rings;

	cout << "Shape Faces: " << m_numShapeFaces << endl;
	cout << "Shape Vertices: " << m_numShapeVertices << endl;
	cout << "Calculated from " << radials << " radials and " << rings << " rings." << endl;

	float *vData = new float[m_numShapeVertices * 3];
	unsigned short *vIndex = new unsigned short[m_numShapeFaces * 3];

	for (int j = 1; j <= rings; j++)
	{
		for (int i = 1; i <= radials; i++)
		{
			float thetaA = ((float)(j) / radials) * PI;
			float phiA = ((float)(i) / rings) * 2 * PI;
			float thetaB = ((float)(j) / radials) * 2 * PI;

			radius = cosf(thetaB);

			vData[(currentVertex * 3) + 0] = radius * cosf(thetaB);	// x 
			vData[(currentVertex * 3) + 1] = radius * cosf(phiA);			// y
			vData[(currentVertex * 3) + 2] = radius * sinf(thetaB);   // z

			cout << fixed << "Vertex " << currentVertex + 1 << " (" << setw(10) << vData[(currentVertex * 3) + 0] << "," << setw(10) << vData[(currentVertex * 3) + 1] << "," << setw(10) << vData[(currentVertex * 3) + 2] << ")" << endl;
			currentVertex++;
		}
	}

	//for (int i = 0; i < m_numShapeVertices; i++)
	//{
	//	if (currentSlice == radials - 1) //Last Vertex in Slice
	//	{
	//		vIndex[(currentFace * 3) + 0] = i;
	//		vIndex[(currentFace * 3) + 1] = i - (slices);
	//		vIndex[(currentFace * 3) + 2] = i - (slices + (slices - 1));
	//
	//		cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
	//		currentFace++;
	//
	//		vIndex[(currentFace * 3) + 0] = i;
	//		vIndex[(currentFace * 3) + 1] = i - 1;
	//		vIndex[(currentFace * 3) + 2] = i - slices;
	//
	//		cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
	//		currentFace++;
	//
	//		currentSlice = 0;
	//	}
	//	else if (currentSlice == 0) //First Vertex in Slice
	//	{
	//		vIndex[(currentFace * 3) + 0] = i;
	//		vIndex[(currentFace * 3) + 1] = i - (slices);
	//		vIndex[(currentFace * 3) + 2] = i - (slices - 1);
	//
	//		cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
	//		currentFace++;
	//
	//		vIndex[(currentFace * 3) + 0] = i;
	//		vIndex[(currentFace * 3) + 1] = i + (slices - 1);
	//		vIndex[(currentFace * 3) + 2] = i - slices;
	//
	//		cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
	//		currentFace++;
	//
	//		currentSlice++;
	//	}
	//	else //Vertex Between Slices
	//	{
	//		vIndex[(currentFace * 3) + 0] = i;
	//		vIndex[(currentFace * 3) + 1] = i - (slices);
	//		vIndex[(currentFace * 3) + 2] = i - (slices - 1);
	//
	//		cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
	//		currentFace++;
	//
	//		vIndex[(currentFace * 3) + 0] = i;
	//		vIndex[(currentFace * 3) + 1] = i - 1;
	//		vIndex[(currentFace * 3) + 2] = i - slices;
	//
	//		cout << "Face " << currentFace << " <" << vIndex[(currentFace * 3) + 0] << "," << vIndex[(currentFace * 3) + 1] << "," << vIndex[(currentFace * 3) + 2] << ">" << endl;
	//		currentFace++;
	//
	//		currentSlice++;
	//	}
	//}
}
