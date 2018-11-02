#include "../Include/CAppGame.h"

CAppGame::CAppGame() :
	CApp(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
	cout << "Constructor: CAppGame()" << endl;

	// Initialize class member variables here
	// ======================================
	m_shaderID = 0;
	m_geoVAOID = 0;
	m_currentDeltaTime = 0;
	m_objectRotation = 0;
	m_objectPosition.X = 0;
	m_objectPosition.Y = 0;
	m_objectPosition.Z = 0;
	m_rotationSpeed = 0;
	// ======================================
}

CAppGame::CAppGame(int window_width, int window_height) :
	CApp(window_width, window_height)
{
	cout << "Constructor: CAppGame(int window_width, int window_height)" << endl;

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
	m_rotationSpeed = 0;
	// ======================================
	// ======================================
}

CAppGame::~CAppGame()
{
}

void CAppGame::initialize()
{
	// Initialize app-specific stuff here
	// ==================================
	std::wstring wresourceFilenameVS;
	std::wstring wresourceFilenameFS;
	std::string resourceFilenameVS;
	std::string resourceFilenameFS;

	char *vertexShaderToLoad;
	char *fragmentShaderToLoad;

	vertexShaderToLoad = VERTEX_SHADER_3D_OBJECT;
	fragmentShaderToLoad = FRAGMENT_SHADER_3D_OBJECT;

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
	// ==================================
	m_pGrid = new CHexaGrid(5, 5, 10, true);
	m_pGrid->BuildGrid();

	m_pCells = m_pGrid->ExtractGrid();

	m_vertexArray = new float[m_pCells.size() * 18];
	m_indexArray = new unsigned short[m_pCells.size() * 12];
	m_normalArray = new float[m_pCells.size() * 12];
	ZeroMemory(m_normalArray, m_pCells.size() * 12 * sizeof(float));
	m_normalIndices = new unsigned short[m_pCells.size() * 12];

	for (int e = 0; e < m_pCells.size(); e++)
	{
		m_vertexExtractor = m_pCells[e]->GetVertices();
		m_indexExtractor = m_pCells[e]->GetIndices();

		//Extracting vertices from cell vector
		for (int v = 0; v < 18; v++)
		{
			m_vertexArray[18 * e + v] = *(m_vertexExtractor + v);
		}

		//Extracting indices from cell vector
		for (int i = 0; i < 12; i++)
		{
			m_indexArray[12 * e + i] = *(m_indexExtractor + i);
		}
	}

	for (int i = 0; i < (m_pCells.size() * 4); i++)
	{
		m_normalIndices[i*3 + 0] = i;
		m_normalIndices[i*3 + 1] = i;
		m_normalIndices[i*3 + 2] = i;
	}

	create();
}

/* */
void CAppGame::run()
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
void CAppGame::update(double deltaTime)
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
void CAppGame::render()
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

		if (m_geoVAOID > 0 && m_pCells.size() > 0)
		{
			getOpenGLRenderer()->renderObject(
				&m_shaderID,
				&m_geoVAOID,
				&noTexture,
				m_pCells.size()*12,
				color,
				&modelMatrix,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);
		}

		// =================================
	}
}

void CAppGame::create()
{
	////Vertices 
	//float vData[15] = {
	//	0.0, h, 0.0,
	//	-halfX, 0.0, halfZ,
	//	halfX, 0.0, halfZ,
	//	-halfX, 0.0, -halfZ,
	//	halfX, 0.0, -halfZ
	//};
	//
	////Vertex Indexes 
	//unsigned short vIndexes[18] = {
	//	0,1,2,
	//	0,2,4,
	//	0,4,3,
	//	0,3,1,
	//	1,4,2,
	//	4,3,1
	//};
	//
	////Normals 
	//float nData[18] = {
	//	0.0, 0.0, 0.0,
	//	0.0, 0.0, 0.0,
	//	0.0, 0.0, 0.0,
	//	0.0, 0.0, 0.0,
	//	0.0, 0.0, 0.0,
	//	0.0, 0.0, 0.0
	//};
	//
	////Normal Indexes 
	//unsigned short nIndexes[18] = {
	//	0,0,0,
	//	1,1,1,
	//	2,2,2,
	//	3,3,3,
	//	4,4,4,
	//	5,5,5
	//};

	//m_loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
	//	&m_shaderID,
	//	&m_geoVAOID,
	//	vData, //Vertices 
	//	5, //Vertex Numbers 
	//	nData, //Normals 
	//	6, //Normal Numbers 
	//	vData, //UV Coords 
	//	5, //UV Coord Numbers 
	//	vIndexes, //Vertex Indexes 
	//	6, //Triangle Numbers 
	//	nIndexes, //Normal Indexes 
	//	6, //Normal Numbers 
	//	vIndexes, //UV Coord Indexes 
	//	6 //UV Coord Numbers 
	//);

	m_loaded = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_shaderID,
		&m_geoVAOID,
		m_vertexArray, //Vertices
		6 * m_pCells.size(), //Vertex Total
		m_normalArray, //Normals
		4 * m_pCells.size(), //Normal Total
		m_vertexArray, //UV Coords
		6 * m_pCells.size(), //UV Coord Total
		m_indexArray, //Triangles
		4 * m_pCells.size(), //Triangle Total
		m_normalIndices, //Normal Indexes
		4 * m_pCells.size(), //Normal Numbers
		m_indexArray, //UV Coord Indexes
		4 * m_pCells.size() //UV Coord Numbers
	);

	if (!m_loaded)
	{
		m_geoVAOID = 0;
	}
}
