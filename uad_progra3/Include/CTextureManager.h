#include "LoadTGA.h"
#include "COpenGLRenderer.h"
#include "CApp.h"
#pragma once

class CTextureManager : COpenGLRenderer
{
public:
	CTextureManager();
	~CTextureManager();

	bool createTextureObject(unsigned int *textureObjectId, unsigned char *textureData, int width, int height);
	bool loadTexture(const char *filename, unsigned int *newTextureID);

private:

	
};