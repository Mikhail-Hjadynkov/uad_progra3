#include "../stdafx.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <Windows.h>
#include <string.h>
#include <sstream>
using namespace std;

#include "../Include/Globals.h"
#include "../Include/CAppEmpty.h"
#include "../Include/CAppX.h"
#include "../Include/CWideStringHelper.h"
#include "../Include/CTextureManager.h"

/* */
CAppX::CAppX() :
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
CAppX::CAppX(int window_width, int window_height) :
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
CAppX::~CAppX()
{
	cout << "Destructor: ~CAppX()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	if (m_geoVAOID)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(&m_shaderID, &m_geoVAOID);
	}
	delete m_XModel;
	// =================================================
}

/* */
void CAppX::initialize()
{
	// Initialize app-specific stuff here
	// ==================================
	std::wstring wresourceFilenameVS;
	std::wstring wresourceFilenameFS;
	std::string resourceFilenameVS;
	std::string resourceFilenameFS;

	loadFile();

	char *vertexShaderToLoad;
	char *fragmentShaderToLoad;

	if (m_XModel->textureFilename != nullptr)
	{
		// Switch shaders to textured object ones
		vertexShaderToLoad = VERTEX_SHADER_TEXTURED_3D_OBJECT;
		fragmentShaderToLoad = FRAGMENT_SHADER_TEXTURED_3D_OBJECT;

		// LOAD TEXTURE AND ALSO CREATE TEXTURE OBJECT
		m_XModel->textureID = 0;
		CTextureManager TGAloader;
		if (!TGAloader.loadTexture(m_XModel->textureFilename, &m_XModel->textureID))
		{
			cout << "Error loading texture." << endl;
		}
	}
	else
	{
		vertexShaderToLoad = VERTEX_SHADER_3D_OBJECT;
		fragmentShaderToLoad = FRAGMENT_SHADER_3D_OBJECT;
	}

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

	if (m_XModel->numVertices != NULL && m_XModel->numNormals != NULL && m_XModel->numTextureCoords != NULL && m_XModel->numVertexIndices != NULL && m_XModel->numNormalIndices != NULL)
	{
		createModel();
	}
	else
	{
		MessageBox(NULL, L"There was a problem parsing the model file", L"Error loading model", MB_ICONERROR);
	}
	// ==================================
}

/* */
void CAppX::run()
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
void CAppX::update(double deltaTime)
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
void CAppX::render()
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

		if (m_geoVAOID > 0 && m_XModel->numVertexIndices > 0)
		{
			//getOpenGLRenderer()->renderWireframeObject(
			//	&m_shaderID,
			//	&m_geoVAOID,
			//	m_XModel->numVertexIndices,
			//	color,
			//	&modelMatrix
			//);
			getOpenGLRenderer()->renderObject(
				&m_shaderID,
				&m_geoVAOID,
				&noTexture,
				m_XModel->numVertexIndices,
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
void CAppX::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void CAppX::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}

void CAppX::loadFile()
{
	string lineBuffer;
	int lineNumber = 0;
	bool fileSelected = false;

	std::wstring fileDirectory = L"";
	fileDirectory.resize(MAX_PATH);

	OPENFILENAME file;
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = NULL;
	file.lpstrFilter = L" X Files\0*.x\0";
	file.lpstrFile = &fileDirectory[0];
	file.nMaxFile = MAX_PATH;
	file.lpstrTitle = L"Choose a model to load";
	file.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	while (fileSelected == false)
	{
		if (0 != GetOpenFileName(&file))
		{
			fstream meshData;
			meshData.open(fileDirectory);
			wcout << "Summarizing " << fileDirectory << endl;

			while (!meshData.eof())
			{
				getline(meshData, lineBuffer);
				lineNumber++;

				//cout << "Line " << lineNumber << " (" << lineBuffer.size() << " chars)" << ": " << lineBuffer << endl;

				if (lineBuffer.size() > 0)
				{
					parseBuffer(lineBuffer);
				}
			}
			meshData.close();
			fileSelected = true;
		}
		else
		{
			MessageBox(NULL, L"A compatible model must be selected", L"No model chosen", MB_ICONWARNING);
		}
	}
}

void CAppX::parseBuffer(string & line)
{
	char separators[] = ",; {}[]\<>\""; //Symbols that mustn't be read from file
	char* lastToken = NULL;
	char* token = "null"; //Stores each parsed value or data from the line
	char* unparsedData = NULL; //Stores the rest of the line as data becomes tokenized
	
	token = strtok_s((char*)line.c_str(), separators, &unparsedData); //Establishing "unparsedData" as context
	
	while (token != NULL)
	{	
		if (token != NULL)
		{
			//Mesh Organizer
			switch (m_currentData)
			{
			case TEXTURE:
				m_XModel->textureFilename = new char[MAX_PATH];
				strcpy(m_XModel->textureFilename, token);
				cout << "Saved '" << m_XModel->textureFilename << "' as texture filename" << endl;
				m_iterator = 0;
				break;

			case VERTICES:
				if (m_XModel->numVertices == NULL && lastToken == NULL)
				{
					m_XModel->numVertices = atoi(token);
					cout << m_XModel->numVertices << " vertices detected in file." << endl;
					m_XModel->vertices = new float[m_XModel->numVertices * 3];
					m_iterator = 0;
				}
				else if (m_iterator < (m_XModel->numVertices * 3))
				{
					m_XModel->vertices[m_iterator] = atof(token);
					//cout << "Vertex Component " << m_iterator << "/" << m_component3D << ": " << m_XModel->vertices[m_iterator] << endl;
					m_iterator++;
					m_component3D++;
				}
				else if (m_XModel->numVertices != NULL && m_iterator >= m_XModel->numVertices * 3)
				{
					m_currentData = VERTEX_INDICES;
					m_XModel->numVertexIndices = atoi(token);
					cout << m_XModel->numVertexIndices << " faces detected in file." << endl;
					m_XModel->vertexIndices = new unsigned short[m_XModel->numVertexIndices * 3];
					m_iterator = 0;
				}
				break;

			case VERTEX_INDICES:
				if (lastToken != NULL)
				{
					m_XModel->vertexIndices[m_iterator] = (unsigned short)strtoul(token, NULL, 0);
					//cout << "Face Component " << m_iterator << "/" << m_component3D << ": " << m_XModel->vertexIndices[m_iterator] << endl;
					m_iterator++;
					m_component3D++;
				}
				break;

			case NORMALS:
				if (m_XModel->numNormals == NULL && lastToken == NULL)
				{
					m_XModel->numNormals = atoi(token);
					cout << m_XModel->numNormals << " normals detected in file." << endl;
					m_XModel->normals = new float[m_XModel->numNormals * 3];
					m_iterator = 0;
				}
				else if (m_iterator < (m_XModel->numNormals * 3))
				{
					m_XModel->normals[m_iterator] = atof(token);
					//cout << "Normal Component " << m_iterator << "/" << m_component3D << ": " << m_XModel->normals[m_iterator] << endl;
					m_iterator++;
					m_component3D++;
				}
				else if (m_iterator >= m_XModel->numNormals * 3)
				{
					m_currentData = NORMAL_INDICES;
					m_XModel->numNormalIndices = atoi(token);
					cout << m_XModel->numNormalIndices << " faces detected in file." << endl;
					m_XModel->normalIndices = new unsigned short[m_XModel->numNormalIndices * 3];
					m_iterator = 0;
				}
				break;
			
			case NORMAL_INDICES:
				if (lastToken != NULL)
				{
					m_XModel->normalIndices[m_iterator] = (unsigned short)strtoul(token, NULL, 0);;
					//cout << "Normal Index Component " << m_iterator << "/" << m_component3D << ": " << m_XModel->normalIndices[m_iterator] << endl;
					m_iterator++;
					m_component3D++;
				}
				break;

			case COORDS:
				if (m_XModel->numTextureCoords == NULL && lastToken == NULL)
				{
					m_XModel->numTextureCoords = atoi(token);
					cout << m_XModel->numTextureCoords << " texture coordinates detected in file." << endl;
					m_XModel->textureCoords = new float[m_XModel->numTextureCoords * 3];
					m_iterator = 0;
				}
				else if (m_iterator < (m_XModel->numTextureCoords * 2))
				{
					m_XModel->textureCoords[m_iterator] = atof(token);
					//cout << "Coordinate Component " << m_iterator << "/" << m_component2D << ": " << m_XModel->textureCoords[m_iterator] << endl;
					m_iterator++;
					m_component2D++;
				}
				break;

			default:
				break;
			}

			//Keyword Gateways
			if (0 == strcmp(token, "TextureFilename") && lastToken == NULL)
			{
				m_currentData = TEXTURE;
				cout << "Texture Detected" << endl;
			}
			else if (0 == strcmp(token, "Mesh") && lastToken == NULL)
			{
				m_currentData = VERTICES;
				cout << "Vertices Detected" << endl;
			}
			else if (0 == strcmp(token, "MeshNormals") && lastToken == NULL)
			{
				m_currentData = NORMALS;
				cout << "Normals Detected" << endl;
			}
			else if (0 == strcmp(token, "MeshTextureCoords") && lastToken == NULL)
			{
				m_currentData = COORDS;
				cout << "Texture Coordinates Detected" << endl;
			}
		}

		lastToken = token;
		token = strtok_s(NULL, separators, &unparsedData);
	}
	m_component3D = 120;
	m_component2D = 117;
}

void CAppX::createModel()
{
	//for (int i = 0; i < (m_XModel->numVertices); i++)
	//{
	//	cout << "Vertex " << m_XModel->vertices[(i * 3) + 0] << ", " << m_XModel->vertices[(i * 3) + 1] << ", " << m_XModel->vertices[(i * 3) + 2] << endl;
	//}
	//for (int i = 0; i < (m_XModel->numVertexIndices); i++)
	//{
	//	cout << "Face " << m_XModel->vertexIndices[(i * 3) + 0] << ", " << m_XModel->vertexIndices[(i * 3) + 1] << ", " << m_XModel->vertexIndices[(i * 3) + 2] << endl;
	//}
	//for (int i = 0; i < (m_XModel->numNormals); i++)
	//{
	//	cout << "Normal " << m_XModel->normals[(i * 3) + 0] << ", " << m_XModel->normals[(i * 3) + 1] << ", " << m_XModel->normals[(i * 3) + 2] << endl;
	//}
	//for (int i = 0; i < (m_XModel->numNormalIndices); i++)
	//{
	//	cout << "Normal Index " << m_XModel->normalIndices[(i * 3) + 0] << ", " << m_XModel->normalIndices[(i * 3) + 1] << ", " << m_XModel->normalIndices[(i * 3) + 2] << endl;
	//}
	//for (int i = 0; i < (m_XModel->numTextureCoords); i++)
	//{
	//	cout << "Coord " << m_XModel->textureCoords[(i * 2) + 0] << ", " << m_XModel->textureCoords[(i * 2) + 1] << endl;
	//}

	m_loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_shaderID,
		&m_geoVAOID,
		m_XModel->vertices, //Vertices
		m_XModel->numVertices, //Vertex Total
		m_XModel->normals, //Normals
		m_XModel->numNormals, //Normal Total
		m_XModel->textureCoords, //UV Coords
		m_XModel->numTextureCoords, //UV Coord Total
		m_XModel->vertexIndices, //Triangles
		m_XModel->numVertexIndices, //Triangle Total
		m_XModel->normalIndices, //Normal Indexes
		m_XModel->numNormalIndices, //Normal Numbers
		m_XModel->vertexIndices, //UV Coord Indexes
		m_XModel->numVertexIndices //UV Coord Numbers
	);

	if (!m_loaded)
	{
		m_geoVAOID = 0;
	}
}