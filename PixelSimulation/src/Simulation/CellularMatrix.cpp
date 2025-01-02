#include "CellularMatrix.h"
#include<Orion/Core/Core.h>
#include<Orion/Core/Log.h>
#include<Orion/Renderer/GraphicsRendering/Renderer2D.h>
#include"Pixels/Pixels.h"

namespace PixelSimulation
{
	CellularMatrix::CellularMatrix(int32_t width, int32_t height)
		:
		m_Width(width / PixelSize),
		m_Height(height / PixelSize),
		m_Matrix(std::vector<AbstractPixel>(static_cast<size_t>(m_Width* m_Height)))
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

		if (InBound(x, y))
			m_Matrix[static_cast<size_t>(x + y * m_Width)].SetType(type);

		if (InBound(x + 1, y))
			m_Matrix[static_cast<size_t>(x + 1 + y * m_Width)].SetType(type);

		if (InBound(x - 1, y))
			m_Matrix[static_cast<size_t>(x - 1 + y * m_Width)].SetType(type);

		if (InBound(x, y + 1))
			m_Matrix[static_cast<size_t>(x + (y + 1) * m_Width)].SetType(type);

		if (InBound(x, y - 1))
			m_Matrix[static_cast<size_t>(x + (y - 1) * m_Width)].SetType(type);
	}

	PixelTypes CellularMatrix::GetCell(uint32_t index)
	{
		return m_Matrix[index].GetType();
	}

	void CellularMatrix::Invalidate(int32_t width, int32_t height)
	{
		width /= PixelSize;
		height /= PixelSize;

		int32_t differenceX = m_Width - width;
		int32_t differenceY = m_Height - height;

		m_Width = width;
		m_Height = height;

		std::vector<AbstractPixel> newVector(width * height);
		if (differenceX <= 0 && differenceY <= 0)
		{
			for (size_t i = 0; i < m_Matrix.size(); i++)
			{
				if (InBound(GetCoord(i).x - differenceX * GetCoord(i).y, GetCoord(i).y + differenceY))
					newVector[GetIndex(GetCoord(i).x - differenceX * GetCoord(i).y, GetCoord(i).y + differenceY)] = m_Matrix[i];
			}
		}
		else if (differenceX >= 0 && differenceY >= 0)
		{
			for (size_t i = 0; i < newVector.size(); i++)
			{
				if (!InBound(GetCoord(i).x, GetCoord(i).y)) continue;

				newVector[i] = m_Matrix[GetIndex(GetCoord(i).x + differenceX * GetCoord(i).y, GetCoord(i).y + differenceY)];
			}
		}
		m_Matrix = newVector;
	}

	void CellularMatrix::UpdateElements()
	{
		for (int32_t i = 0; i < m_Matrix.size(); i++)
		{
			PixelTypes type = m_Matrix[i].GetType();
			if (type == EMPTY)
				continue;

			int32_t x = (i % m_Width);
			int32_t y = (i / m_Width);

			m_Matrix[i].Update(*this, x, y);
		}
	}

	// Implementation of DrawElements
	void CellularMatrix::DrawElements()
	{
		for (int32_t i = 0; i < m_Matrix.size(); i++)
		{
			PixelTypes type = m_Matrix[i].GetType();
			if (type == EMPTY)
				continue;

			int32_t x = (i % m_Width);
			int32_t y = (i / m_Width);
			glm::vec3 position{ (x * PixelSize) + PixelSize / 2, (y * PixelSize) + PixelSize / 2, 0.0f };
			glm::vec2 halfExtend{ PixelSize / 2 };
			m_Matrix[i].Draw(position, halfExtend);
		}
	}
}