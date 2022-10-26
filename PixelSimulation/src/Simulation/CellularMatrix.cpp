#include "CellularMatrix.h"
#include<Orion/Core/Core.h>
#include<Orion/Core/Log.h>
#include<Orion/Renderer/GraphicsRendering/Renderer2D.h>

namespace PixelSimulation
{
	
	CellularMatrix::CellularMatrix(int32_t width, int32_t height) 
		: 
		m_Width(width / PixelSize),
		m_Height(height / PixelSize),
		m_Matrix(std::vector<AbstractPixel>(static_cast<size_t>(m_Width * m_Height)))
	{
		
	}


	void CellularMatrix::SetCell(uint32_t index, PixelTypes type) 
	{
		m_Matrix[index].SetType(type);
	}


	void CellularMatrix::SetCell(int32_t x, int32_t y, PixelTypes type)
	{
		x /= PixelSize;
		y /= PixelSize;
		m_Matrix[static_cast<size_t>(x  + y  * m_Width)  ].SetType(type);


		
		m_Matrix[static_cast<size_t>(x+1 + y * m_Width)].SetType(type);
		m_Matrix[static_cast<size_t>(x-1 + y * m_Width)].SetType(type);
		m_Matrix[static_cast<size_t>(x + (y+1) * m_Width)].SetType(type);
		m_Matrix[static_cast<size_t>(x + (y-1) * m_Width)].SetType(type);
		

	}


	PixelTypes CellularMatrix::GetCell(uint32_t index)
	{
		return m_Matrix[index].GetType();
	}

	void CellularMatrix::Invalidate(int32_t width, int32_t height)
	{
		width /= PixelSize;
		height /= PixelSize;
		std::vector<AbstractPixel> newMatrix(width * height);


		auto srcIt = m_Matrix.begin();
		int32_t rowIndex = 0;
		int32_t index = 0;

		for (auto it = newMatrix.begin(); it != newMatrix.end(); it++, index++, srcIt++)
		{
			if(index / width > rowIndex)
			{
				rowIndex = index / width;
				srcIt += m_Width - index;
			}

			*it = *srcIt;
			
		}

		m_Width = width;
		m_Height = height;

		m_Matrix = newMatrix;


	}


	void CellularMatrix::DrawElemets()
	{

		
		
		float x = 0.f;
		float y = 0.f;
		
		for (int32_t i = 0; i < m_Matrix.size(); i++)
		{
			PixelTypes type = m_Matrix[i].GetType();
			if (type == EMPTY) 
			{
				//Orion::Renderer2D::DrawBorderedQuad(position, halfExtend, glm::vec4(0.9f, 0.1f, 0.1f, 1.0f));
				continue;
			}

			
			x = (i % (m_Width));
			y = (i / (m_Width));
			glm::vec3 position{ (x * PixelSize) + PixelSize/2, (y * PixelSize) + PixelSize/2, 0.0f };
			glm::vec2 halfExtend{ PixelSize / 2 };
			m_Matrix[i].Draw(position, halfExtend);
			m_Matrix[i].Update(m_Matrix,i,m_Width);




		}


	}

}