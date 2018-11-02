#include "CHexaCell.h"
#include <vector>
#pragma once

class CHexaGrid
{
public:
	CHexaGrid();
	CHexaGrid(int width, int height, float radius, bool type);
	~CHexaGrid();

	void BuildGrid(); //Creates the cells that build up the grid
	vector<CHexaCell*> ExtractGrid(); //Method to extract info from built cells
	void Load3DObject(); //Loads meshes to the cell

private:
	CVector3 m_initialCoordinates;
	vector<CHexaCell*> m_CellVector;
	
	bool m_gridType;
	bool m_isFlatOffset = false; //Makes the zig-zag pattern for each row in flat grid
	bool m_isPointyOffset = false; //Makes the zig-zag pattern for each column in pointy grid

	int m_width;
	int m_height;
	int m_counter;

	float m_radius;
	float m_apothem;
	float m_spacing;
};