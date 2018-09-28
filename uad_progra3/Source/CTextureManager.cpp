#include "../Include/CTextureManager.h"

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
}

bool CTextureManager::createTextureObject(unsigned int *textureObjectId, unsigned char *textureData, int width, int height)
{
	if (textureObjectId != NULL && textureData != NULL)
	{
		// Create an OpenGL texture object
		glGenTextures(1, textureObjectId);

		// Activate texture unit #0
		glActiveTexture(GL_TEXTURE0);

		// Bind the new texture objectly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, *textureObjectId);

		// Pass the data to OpenGL
		// *NOTE: We're specifying GL_BGR because we're reading TGA files, which have the RED and BLUE channels switched.
		//        Ideally, we should pass a parameter to specify this ;)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData);

		// Set mipmap filtering modes and generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// Check for OpenGL errors
		m_OpenGLError = checkOpenGLError("COpenGLRenderer::createTextureObject");

		return true;
	}

	return false;
}

/* Read texture file and generate an OpenGL texture object */
bool CTextureManager::loadTexture(const char *filename, unsigned int *newTextureID)
{
	TGAFILE tgaFile;
	tgaFile.imageData = nullptr;

	if (filename == nullptr || newTextureID == nullptr)
	{
		return false;
	}

	*newTextureID = 0;

	if (LoadTGAFile(filename, &tgaFile))
	{
		if (tgaFile.imageData == nullptr ||
			tgaFile.imageHeight < 0 ||
			tgaFile.imageWidth < 0)
		{
			if (tgaFile.imageData != nullptr)
			{
				delete[] tgaFile.imageData;
			}

			return false;
		}

		// Create a texture object for the menu, and copy the texture data to graphics memory
		if (!createTextureObject(
			newTextureID,
			tgaFile.imageData,
			tgaFile.imageWidth,
			tgaFile.imageHeight
		))
		{
			return false;
		}

		// Texture data is stored in graphics memory now, we don't need this copy anymore
		if (tgaFile.imageData != nullptr)
		{
			delete[] tgaFile.imageData;
		}
	}
	else
	{
		// Free texture data
		if (tgaFile.imageData != nullptr)
		{
			delete[] tgaFile.imageData;
		}

		return false;
	}

	return true;
}
