#include "CellularMatrix.h"

namespace PixelSimulation
{

	CellularMatrix::CellularMatrix(uint32_t  width, uint32_t height) 
		: 
		m_Width(width),
		m_Height(height), 
		m_Matrix(std::vector<int32_t>((size_t)width * height))
	{
		
	}
	 void CellularMatrix::SetCell(uint32_t index, int32_t val) 
	{
		m_Matrix[index] = val;
	}
	 void CellularMatrix::SetCell(uint32_t x, uint32_t y, int32_t val) 
	{
		m_Matrix[static_cast<size_t>(x + y * m_Width)] = val;
	}
	 int32_t CellularMatrix::GetCell(uint32_t index) 
	{
		return m_Matrix[index];
	}

	void CellularMatrix::DrawElemets()
	{

		
		float pixelSize = 1.0f;


		for (int32_t i = 0; i < m_Matrix.size(); i++)
		{
			if (m_Matrix[i] == 0) continue;

			if (m_Matrix[i] == 1)
			{

				float x = (i % m_Width);
				float y = (i / m_Width);
				if (i - m_Width > 0 && m_Matrix[i - m_Width] == 0)
				{
						m_Matrix[i] = 0;
						m_Matrix[i - m_Width] = 1;
				}
				else if((i - m_Width)-1 > 0 && m_Matrix[(i - m_Width) - 1] == 0 && (i/m_Width) - (((i - m_Width) - 1) / m_Width) < 2)
				{
					m_Matrix[i] = 0;
					m_Matrix[(i - m_Width) - 1] = 1;
				}
				else if((i - m_Width) + 1 > 0 && m_Matrix[(i - m_Width) + 1] == 0 && (((i - m_Width) + 1) / m_Width) != (i / m_Width))
				{
					m_Matrix[i] = 0;
					m_Matrix[(i - m_Width) + 1] = 1;
				}


				Orion::Renderer2D::DrawBorderedQuad(glm::vec3(x * pixelSize, y* pixelSize, 0.0f), glm::vec2(pixelSize/2), glm::vec4(0.925f, 0.923f, 0.0f, 1.0f));
			}
		}


	}

}