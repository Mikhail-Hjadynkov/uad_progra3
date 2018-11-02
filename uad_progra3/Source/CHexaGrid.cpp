#include "../Include/CHexaGrid.h"

CHexaGrid::CHexaGrid()
{
}

CHexaGrid::CHexaGrid(int width, int height, float radius, bool type)
{
	m_width = width;
	m_height = height;
	m_radius = radius;
	m_gridType = type;
	m_counter = 0;
	cout << "Grid Building Data" << endl;
	cout << "Width: " << m_width << endl;
	cout << "Height: " << m_height << endl;
	cout << "Radius: " << m_radius << endl;
	cout << "Type: " << m_gridType << endl;
}

CHexaGrid::~CHexaGrid()
{
}

void CHexaGrid::BuildGrid()
{
	//Extracting extra information for hexagon spacing
	m_apothem = sin(60 * PI / 180) * m_radius;
	cout << "Apothem: " << m_apothem << endl;
	m_spacing = m_radius + (m_radius / 2);
	cout << "Spacing: " << m_spacing << endl;
	cout << endl;

	//Cycle for building rows
	for (int i = 0; i < m_height; i++)
	{
		m_initialCoordinates.setValues(0, 0, 0);
		
		if (m_gridType) //If grid is flat
		{
			//Setting offset to false to continue with flat grid building
			m_isFlatOffset = false;
			m_initialCoordinates.Y -= m_apothem * i * 2;
		}
		else //Else, grid is pointy
		{
			//Comparison to offset pointy grid
			if (!m_isPointyOffset)
			{
				m_initialCoordinates.Y -= m_spacing * i;
				m_isPointyOffset = true;
			}
			else
			{
				m_initialCoordinates.Y -= m_spacing * i;
				m_initialCoordinates.X += m_apothem;
				m_isPointyOffset = false;
			}
		}

		//Cycle for building columns
		for (int j = 0; j < m_width; j++)
		{
			cout << "HEXAGON " << m_counter << endl;
			CHexaCell* m_pGridCell = new CHexaCell(m_initialCoordinates);

			//Comparison to build requested hexagon type
			if (m_gridType) //Flat Grid
			{
				m_pGridCell->BuildFlat(m_initialCoordinates, m_radius);

				//Comparison to offset flat grid
				if (!m_isFlatOffset)
				{
					m_initialCoordinates.X += m_spacing;
					m_initialCoordinates.Y -= m_apothem;
					m_isFlatOffset = true;
				}
				else
				{
					m_initialCoordinates.X += m_spacing;
					m_initialCoordinates.Y += m_apothem;
					m_isFlatOffset = false;
				}
			}
			else //Pointy Grid
			{
				m_pGridCell->BuildPointy(m_initialCoordinates, m_radius);
				//Continuing with pointy grid building
				m_initialCoordinates.X += m_apothem * 2;
			}
			//Pushing hexagon to grid vector
			m_pGridCell->BuildIndices(m_CellVector.size());
			m_CellVector.push_back(m_pGridCell);
			m_counter++;
			cout << endl;
		}
	}
	
}

vector<CHexaCell*> CHexaGrid::ExtractGrid()
{
	return m_CellVector;
}

void CHexaGrid::Load3DObject()
{
}
