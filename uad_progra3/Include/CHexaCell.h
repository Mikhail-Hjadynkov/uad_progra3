#include"CGameObject.h"
#include"CVector3.h"
#include <math.h>
#include <iomanip>
#include <iostream>

#pragma once
constexpr double PI = 3.14159265359;

class CHexaCell
{
public:
	CHexaCell();
	CHexaCell(CVector3 coordinates);
	~CHexaCell();

	void BuildFlat(CVector3 coordinates, float radius);
	void BuildPointy(CVector3 coordinates, float radius);
	void BuildIndices(int hexNumber);

	float* GetVertices();
	unsigned short* GetIndices();

private:
	CGameObject *m_pGameObject;

	float m_degreeAngle;
	float m_radianAngle;

	float m_centerPoint[3];
	float m_vertices[18];
	unsigned short m_indices[12];
};