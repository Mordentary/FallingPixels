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
	int32_t CellularMatrix::GetCell(int32_t index) 
	{
		return m_Matrix[index];
	}

	void CellularMatrix::DrawElemets()
	{

		auto& cam = Orion::CamerasController::GetActiveCamera();



		float elementSizeInPxl = 1.f;
		float elementSizeInUnits = 2 / 4.f;

		auto [mouseX, mouseY] = Orion::Input::GetMousePosition();
		float pixelCountX = m_Width / elementSizeInPxl;
		float pixelCountY =  m_Height / elementSizeInPxl;

		float pixelWidth = 2.0f / m_Width;



		//ORI_TRACE("PixelWidth: {0}",pixelWidth);
		//float x =  (2.0 * 1 + 1.0) / w - 1.0
		//float y =  (2.0 * 1 + 1.0) / h - 1.0

		if (Orion::Input::IsMouseButtonPressed(ORI_MOUSE_BUTTON_1)) 
		{

			//SetCell(mouseX, m_Height - mouseY, 1);
		}
		if (Orion::Input::IsMouseButtonPressed(ORI_MOUSE_BUTTON_2))
		{

			//SetCell(mouseX, m_Height - mouseY, 0);
		}


		

		
			
		//Orion::Renderer2D::DrawBorderedQuad(glm::vec3(0.f ,0.f ,0.0f), glm::vec2(0.5f), glm::vec4(0.925f, 0.323f, 0.4f, 1.0f));
		


		for (size_t i = 0; i < m_Matrix.size(); i++)
		{
			if (m_Matrix[i] == 1) 
			{

				float x = (2.0 * (i % m_Width) +1) / m_Width - 1.0;
				float y = (2.0 * (i / m_Width) + 1) / m_Height - 1.0;

				glm::vec4 ray_clip = glm::vec4(x, -y, -1.0, 1.0);
				glm::vec2 ray_eye = glm::vec2(glm::inverse(cam->GetProjectionMatrix()) * ray_clip);
				ORI_TRACE("X: {0} AND Y: {1}", ray_eye.x, ray_eye.y);


				Orion::Renderer2D::DrawBorderedQuad(glm::vec3(ray_eye,0.0f), glm::vec2(pixelWidth), glm::vec4(0.925f, 0.923f, 0.0f, 1.0f));
			}
		}


	}

}