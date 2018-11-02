#include "C3DModel.h"
#pragma once

class CGameObject
{
public:
	CGameObject();
	~CGameObject();

private:
	C3DModel* m_pMesh;

	float scale;
};