#pragma once
#include"Pixels.h"
#include <Orion/Core/Log.h>
#include <Orion/Core/Core.h>
#include <Orion/Renderer/GraphicsRendering/Renderer2D.cpp>

namespace PixelSimulation
{
	void AbstractPixel::Update(std::vector<AbstractPixel>& matrix, int32_t index, int32_t screenWidth)
	{
		switch(m_Type) 
		{
		
			case SAND:
			{

				if (index - screenWidth > 0 && matrix[index - screenWidth].IsEqual(EMPTY))
				{
					matrix[index].SetType(EMPTY);
					matrix[index - screenWidth].SetType(SAND);
				}
				else if
					(
						(index - screenWidth) - 1 > 0
						&&
						matrix[(index - screenWidth) - 1].IsEqual(EMPTY)
						&&
						(index / screenWidth) - (((index - screenWidth) - 1) / screenWidth) < 2
						)
				{
					matrix[(index - screenWidth) - 1].SetType(SAND);
					matrix[index].SetType(EMPTY);

				}
				else if
					(
						(index - screenWidth) + 1 > 0
						&&
						matrix[(index - screenWidth) + 1].IsEqual(EMPTY)
						&&
						(((index - screenWidth) + 1) / screenWidth) != (index / screenWidth))
				{
					matrix[(index - screenWidth) + 1].SetType(SAND);
					matrix[index].SetType(EMPTY);
				}

			}
				break;

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