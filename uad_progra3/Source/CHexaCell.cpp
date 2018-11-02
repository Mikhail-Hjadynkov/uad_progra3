#include "../Include/CHexaCell.h"

CHexaCell::CHexaCell()
{
}

CHexaCell::CHexaCell(CVector3 coordinates)
{
	m_centerPoint[0] = coordinates.X;
	m_centerPoint[1] = coordinates.Y;
	m_centerPoint[2] = coordinates.Z;
	cout << "Center Coordinate" << endl;
	cout << "(" << setprecision(2) << m_centerPoint[0] << "," << setprecision(2) << m_centerPoint[1] << ")" << endl << endl;
}

CHexaCell::~CHexaCell()
{
}

void CHexaCell::BuildFlat(CVector3 coordinates, float radius)
{
	cout << "Flat Hexagon Coordinates" << endl;
	for (int i = 0; i < 6; i++)
	{
		m_degreeAngle = 60 * i;
		m_radianAngle = PI / 180 * m_degreeAngle;

		m_vertices[3 * i + 0] = coordinates.X + radius * cos(m_radianAngle); //X Coordinate
		m_vertices[3 * i + 1] = coordinates.Y + radius * sin(m_radianAngle); //Y Coordinate
		m_vertices[3 * i + 2] = coordinates.Z; //Z Coordinate
		cout << "(" << fixed << m_vertices[3 * i + 0] << "," << fixed << m_vertices[3 * i + 1] << ")" << endl;
	}
}

void CHexaCell::BuildPointy( CVector3 coordinates, float radius)
{
	cout << "Pointy Hexagon Coordinates" << endl;
	for (int i = 0; i < 6; i++)
	{
		m_degreeAngle = 60 * i - 30;
		m_radianAngle = PI / 180 * m_degreeAngle;

		m_vertices[3 * i + 0] = coordinates.X + radius * cos(m_radianAngle); //X Coordinate
		m_vertices[3 * i + 1] = coordinates.Y + radius * sin(m_radianAngle); //Y Coordinate
		m_vertices[3 * i + 2] = coordinates.Z; //Z Coordinate
		cout << "(" << fixed << m_vertices[3 * i + 0] << "," << fixed << m_vertices[3 * i + 1] << ")" << endl;
	}
}

void CHexaCell::BuildIndices(int hexNumber)
{
	//Assigning vertices to build indices
	cout << endl << "Hexagon Indices" << endl;
	hexNumber;
	for (int i = 0; i < 4; i++)
	{
		m_indices[i * 3 + 0] = (hexNumber * 6); //All triangles will have one vertex as common vertex
		m_indices[i * 3 + 1] = (hexNumber * 6) + i + 2;
		m_indices[i * 3 + 2] = (hexNumber * 6) + i + 1;
		cout << "(" << m_indices[i * 3 + 0] << "," << m_indices[i * 3 + 1] << "," << m_indices[i * 3 + 2] << ")" << endl;
	}
}

float * CHexaCell::GetVertices()
{
	return m_vertices;
}

unsigned short * CHexaCell::GetIndices()
{
	return m_indices;
}
