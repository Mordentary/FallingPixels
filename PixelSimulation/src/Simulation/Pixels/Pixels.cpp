#pragma once
#include"Pixels.h"
#include <Orion/Core/Log.h>
#include <Orion/Core/Core.h>
#include <Orion/Renderer/GraphicsRendering/Renderer2D.cpp>
#include"../CellularMatrix.h"


namespace PixelSimulation
{


	void AbstractPixel::Update(CellularMatrix& matrix, int32_t x, int32_t y)
	{
		switch (m_Type)
		{
		case STONE: break;
		case SAND:
		{

			if
				(
					matrix.InBound(x, y - 1)
					&&
					(matrix[{x, y - 1}].IsEqual(EMPTY)
						||
						matrix[{x, y - 1}].IsEqual(WATER))
					)
			{
				matrix[{x, y}].SetType(matrix[{x, y - 1}].GetType());
				matrix[{x, y - 1}].SetType(SAND);
			}
			else if
				(
					matrix.InBound(x - 1, y - 1)
					&&
					(
						matrix[{x - 1, y - 1}].IsEqual(EMPTY)
						||
						matrix[x - 1, y - 1].IsEqual(WATER)
						)
					&&
					matrix.GetRow(x, y) - matrix.GetRow(x - 1, y - 1) < 2
					)
			{
				matrix[{x, y}].SetType(matrix[{x - 1, y - 1}].GetType());
				matrix[{x - 1, y - 1}].SetType(SAND);

			}
			else if
				(
					matrix.InBound(x + 1, y - 1)
					&& 
					(
						matrix[{x + 1, y - 1}].IsEqual(EMPTY)
						||
						matrix[x + 1, y - 1].IsEqual(WATER)
					)
					&&
					matrix.GetRow(x, y) != matrix.GetRow(x + 1 , y - 1)
					)
			{
				matrix[{x, y}].SetType(matrix[{x + 1, y - 1}].GetType());
				matrix[{x + 1, y - 1}].SetType(SAND);
			}

		}
		break;

		case WATER:
		{
			if
				(
					matrix.InBound(x, y - 1)
					&&
					(
						matrix[{x, y - 1}].IsEqual(EMPTY)
					
						)
					)

			{
				matrix[{x, y}].SetType(matrix[{x, y - 1}].GetType());
				matrix[{x, y - 1}].SetType(WATER);
			}
			else if
				(
					matrix.InBound(x - 1, y - 1)
					&&
					(
						matrix[{x - 1, y - 1}].IsEqual(EMPTY)
						
						)
					&&
					matrix.GetRow(x, y) - matrix.GetRow(x - 1, y - 1) < 2
					)

			{
				matrix[{x, y}].SetType(matrix[{x - 1, y - 1}].GetType());
				matrix[{x - 1, y - 1}].SetType(WATER);

			}
			else if
				(
					matrix.InBound(x + 1, y - 1)
					&&
					(
						matrix[{x + 1, y - 1}].IsEqual(EMPTY)
						
						)
					&&
					matrix.GetRow(x, y) != matrix.GetRow(x + 1, y - 1)
					)

			{
				matrix[{x, y}].SetType(matrix[{x + 1, y - 1}].GetType());
				matrix[{x + 1, y - 1}].SetType(WATER);
			}
			else if
			(
					matrix.InBound(x - 1, y)
					&&
					matrix[{x - 1, y}].IsEqual(EMPTY)
			)

			{
				matrix[{x,y}].SetType(EMPTY);
				matrix[{x - 1, y}].SetType(WATER);
			}
			else if
				(
					matrix.InBound(x + 1, y)
					&&
					matrix[{x + 1, y}].IsEqual(EMPTY)
					)

			{
				matrix[{x, y}].SetType(EMPTY);
				matrix[{x + 1, y}].SetType(WATER);
			}

		}
		default:
			//ORI_ASSERT(false, "Uknown pixel type");
			break;;
		}
	}


	void AbstractPixel::Draw(glm::vec3& screenPosition, glm::vec2& size)
	{
		switch (m_Type)
		{
	
		case STONE:
			m_Color = glm::vec4(0.43f, 0.53f, 0.6f, 1.0f) * m_lifeTime;
			Orion::Renderer2D::DrawBorderedQuad(screenPosition, size, m_Color); 
			break;
		case SAND:
			m_Color = glm::vec4(0.74f, 0.7f, 0.4f, 1.0f) * m_lifeTime;
			Orion::Renderer2D::DrawBorderedQuad(screenPosition, size, m_Color);
			break;
		case WATER:
			m_Color = glm::vec4(0.3f, 0.2f, 0.8f, 1.0f) * m_lifeTime;
			Orion::Renderer2D::DrawBorderedQuad(screenPosition, size, m_Color);
			break;

		default:
			//ORI_ASSERT(false, "Uknown pixel type");
			break;;
		}
	}
	
}