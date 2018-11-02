//#include "stdafx.h"
#include <Windows.h>
#include <string.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <errno.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <sstream>


#define DLL_CLASS_EXPORTS __declspec(dllexport)

#include "ModelLoader.h"


void CLoadX::loadFile()
{
	std::string lineBuffer;
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
			std::fstream meshData;
			meshData.open(fileDirectory);
			std::wcout << "Summarizing " << fileDirectory << std::endl;

			while (!meshData.eof())
			{
				getline(meshData, lineBuffer);
				lineNumber++;

				//cout << "Line " << lineNumber << " (" << lineBuffer.size() << " chars)" << ": " << lineBuffer << endl;

				if (lineBuffer.size() > 0)
				{
					parseBuffer(lineBuffer, &m_XModel);
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

void CLoadX::parseBuffer(std::string & line, xMesh** xmesh)
{
	char separators[] = ",; {}[]\<>\""; //Symbols that mustn't be read from file
	char* lastToken = NULL;
	char* token = nullptr; //Stores each parsed value or data from the line
	token = new char[MAX_PATH];
	strcpy_s(token, MAX_PATH, (const char*)"null");
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
				((xMesh*)(*(xmesh)))->textureFilename = new char[MAX_PATH];
				strcpy_s(((xMesh*)(*(xmesh)))->textureFilename, MAX_PATH, token);
				std::cout << "Saved '" << ((xMesh*)(*(xmesh)))->textureFilename << "' as texture filename" << std::endl;
				m_iterator = 0;
				break;

			case VERTICES:
				if (((xMesh*)(*(xmesh)))->numVertices == NULL && lastToken == NULL)
				{
					((xMesh*)(*(xmesh)))->numVertices = atoi(token);
					std::cout << ((xMesh*)(*(xmesh)))->numVertices << " vertices detected in file." << std::endl;
					((xMesh*)(*(xmesh)))->vertices = new float[((xMesh*)(*(xmesh)))->numVertices * 3];
					m_iterator = 0;
				}
				else if (m_iterator < (((xMesh*)(*(xmesh)))->numVertices * 3))
				{
					((xMesh*)(*(xmesh)))->vertices[m_iterator] = atof(token);
					//cout << "Vertex Component " << m_iterator << "/" << m_component3D << ": " << ((xMesh*)(*(xmesh)))->vertices[m_iterator] << endl;
					m_iterator++;
					m_component3D++;
				}
				else if (((xMesh*)(*(xmesh)))->numVertices != NULL && m_iterator >= ((xMesh*)(*(xmesh)))->numVertices * 3)
				{
					m_currentData = VERTEX_INDICES;
					((xMesh*)(*(xmesh)))->numVertexIndices = atoi(token);
					std::cout << ((xMesh*)(*(xmesh)))->numVertexIndices << " faces detected in file." << std::endl;
					((xMesh*)(*(xmesh)))->vertexIndices = new unsigned short[((xMesh*)(*(xmesh)))->numVertexIndices * 3];
					m_iterator = 0;
				}
				break;

			case VERTEX_INDICES:
				if (lastToken != NULL)
				{
					((xMesh*)(*(xmesh)))->vertexIndices[m_iterator] = (unsigned short)strtoul(token, NULL, 0);
					//cout << "Face Component " << m_iterator << "/" << m_component3D << ": " << ((xMesh*)(*(xmesh)))->vertexIndices[m_iterator] << endl;
					m_iterator++;
					m_component3D++;
				}
				break;

			case NORMALS:
				if (((xMesh*)(*(xmesh)))->numNormals == NULL && lastToken == NULL)
				{
					((xMesh*)(*(xmesh)))->numNormals = atoi(token);
					std::cout << ((xMesh*)(*(xmesh)))->numNormals << " normals detected in file." << std::endl;
					((xMesh*)(*(xmesh)))->normals = new float[((xMesh*)(*(xmesh)))->numNormals * 3];
					m_iterator = 0;
				}
				else if (m_iterator < (((xMesh*)(*(xmesh)))->numNormals * 3))
				{
					((xMesh*)(*(xmesh)))->normals[m_iterator] = atof(token);
					//cout << "Normal Component " << m_iterator << "/" << m_component3D << ": " << ((xMesh*)(*(xmesh)))->normals[m_iterator] << endl;
					m_iterator++;
					m_component3D++;
				}
				else if (m_iterator >= ((xMesh*)(*(xmesh)))->numNormals * 3)
				{
					m_currentData = NORMAL_INDICES;
					((xMesh*)(*(xmesh)))->numNormalIndices = atoi(token);
					std::cout << ((xMesh*)(*(xmesh)))->numNormalIndices << " faces detected in file." << std::endl;
					((xMesh*)(*(xmesh)))->normalIndices = new unsigned short[((xMesh*)(*(xmesh)))->numNormalIndices * 3];
					m_iterator = 0;
				}
				break;

			case NORMAL_INDICES:
				if (lastToken != NULL)
				{
					((xMesh*)(*(xmesh)))->normalIndices[m_iterator] = (unsigned short)strtoul(token, NULL, 0);;
					//cout << "Normal Index Component " << m_iterator << "/" << m_component3D << ": " << ((xMesh*)(*(xmesh)))->normalIndices[m_iterator] << endl;
					m_iterator++;
					m_component3D++;
				}
				break;

			case COORDS:
				if (((xMesh*)(*(xmesh)))->numTextureCoords == NULL && lastToken == NULL)
				{
					((xMesh*)(*(xmesh)))->numTextureCoords = atoi(token);
					std::cout << ((xMesh*)(*(xmesh)))->numTextureCoords << " texture coordinates detected in file." << std::endl;
					((xMesh*)(*(xmesh)))->textureCoords = new float[((xMesh*)(*(xmesh)))->numTextureCoords * 3];
					m_iterator = 0;
				}
				else if (m_iterator < (((xMesh*)(*(xmesh)))->numTextureCoords * 2))
				{
					((xMesh*)(*(xmesh)))->textureCoords[m_iterator] = atof(token);
					//cout << "Coordinate Component " << m_iterator << "/" << m_component2D << ": " << ((xMesh*)(*(xmesh)))->textureCoords[m_iterator] << endl;
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
				std::cout << "Texture Detected" << std::endl;
			}
			else if (0 == strcmp(token, "Mesh") && lastToken == NULL)
			{
				m_currentData = VERTICES;
				std::cout << "Vertices Detected" << std::endl;
			}
			else if (0 == strcmp(token, "MeshNormals") && lastToken == NULL)
			{
				m_currentData = NORMALS;
				std::cout << "Normals Detected" << std::endl;
			}
			else if (0 == strcmp(token, "MeshTextureCoords") && lastToken == NULL)
			{
				m_currentData = COORDS;
				std::cout << "Texture Coordinates Detected" << std::endl;
			}
		}

		lastToken = token;
		token = strtok_s(NULL, separators, &unparsedData);
	}
	m_component3D = 120;
	m_component2D = 117;
}