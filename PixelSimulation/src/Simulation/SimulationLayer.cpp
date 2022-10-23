#include"SimulationLayer.h"
#include"imgui.h"
#include"glm/gtc/type_ptr.hpp"
#include"glm/gtx/string_cast.hpp"

namespace PixelSimulation
{

		void SimulationLayer::Init() 
		{
			Orion::Renderer::Init();

			auto& app = Orion::Application::Get();
			m_ScreenHeight = app.GetWindow().GetHeight();
			m_ScreenWidth = app.GetWindow().GetWidth();
			app.GetWindow().SetVSync(false);
			ORI_INFO("{0}, {1}", m_ScreenWidth, m_ScreenHeight);
			m_Matrix = PixelSimulation::CellularMatrix(m_ScreenWidth, m_ScreenHeight);
			m_Camera = Orion::CreateShared<Orion::OrthographicCamera>(glm::vec3(0.0f),false, true);
			Orion::CamerasController::AddCamera("SecondaryCamera", Orion::CreateShared<Orion::PerspectiveCamera>(glm::vec3(0.f,0.f,1.0f), glm::vec3(0.f, 0.f, -1.f)));
			Orion::CamerasController::AddCamera("PrimaryCamera", m_Camera);
			m_Camera->SetPosition(glm::vec3(1.f, 1.f, 0.f));
			m_Camera->SetZoomLevel(1.0f);
		}

		void SimulationLayer::OnUpdate(Orion::Timestep deltaTime) 
		{
			ORI_PROFILE_FUNCTION();

			Orion::RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			Orion::RenderCommand::Clear();

			Orion::CamerasController::OnUpdate(deltaTime);

			Orion::Renderer2D::ResetStats();

			
			auto& cam = Orion::CamerasController::GetActiveCamera();

			auto[mouseX,mouseY] = Orion::Input::GetMousePosition();
			


			float x = (2.0f * mouseX) / m_ScreenWidth - 1.0f;
			float y = 1.0f - (2.0f * mouseY) / m_ScreenHeight;
			float pixelSizeX = 2.0 / m_ScreenWidth;
		


			glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);
			glm::vec2 ray_eye = glm::vec2(glm::inverse(cam->GetProjectionMatrix()) * ray_clip);
			
			if (Orion::Input::IsMouseButtonPressed(ORI_MOUSE_BUTTON_1))
			{
				mouseY = (m_ScreenHeight - mouseY);
				for (size_t i = 0; i < 25; i++)
				{
					for (size_t j = 0; j < 25; j++)
					{

						//std::cout << glm::distance(glm::vec2(mouseX + i, mouseY + j), glm::vec2(mouseX, mouseY)) << "\n";
						if (glm::distance(glm::vec2(mouseX + i, mouseY + j), glm::vec2(mouseX, mouseY)) < 5) 
						{
							m_Matrix.SetCell(mouseX - i, mouseY - j, 1);
							m_Matrix.SetCell(mouseX + i, mouseY + j, 1);
						}

					}
				}
			}


			for (size_t i = 0; i < 20; i+=1)
			{
				for (size_t j = 0; j < 20; j+=1)
				{
					//m_Matrix.SetCell(i, j, 1);

				}
			}
		



			Orion::Renderer2D::BeginScene(Orion::CamerasController::GetActiveCamera());

			m_Matrix.DrawElemets();

			Orion::Renderer2D::EndScene();

		}


		void SimulationLayer::OnEvent(Orion::Event& event) 
		{

			Orion::CamerasController::OnEvent(event);
			m_Dispatcher = Orion::EventDispatcher::CreateDispatcher(event);

			m_Dispatcher->Dispatch<Orion::MouseMovedEvent>(ORI_BIND_EVENT_FN(SimulationLayer::OnMouseMoved));

			m_Dispatcher->Dispatch<Orion::WindowResizeEvent>(ORI_BIND_EVENT_FN(SimulationLayer::OnWindowResized));

			m_Dispatcher->Dispatch<Orion::KeyPressedEvent>(ORI_BIND_EVENT_FN(SimulationLayer::OnKeyPressed));





		


		}
	

		bool SimulationLayer::OnWindowResized(Orion::WindowResizeEvent e) 
		{

			return false;
		}


		
		bool SimulationLayer::OnMouseMoved(Orion::MouseMovedEvent e)
		{
			auto& cam = Orion::CamerasController::GetActiveCamera();
			float mouseX = e.GetX();
			float mouseY = e.GetY();
			ORI_INFO("Viewport: {0}", e);
			float x = (2.0f * mouseX) / m_ScreenWidth - 1.0f;
			float y = 1.0f - (2.0f * mouseY) / m_ScreenHeight;
			float z = 1.0f;

			glm::vec3 ray_nds = glm::vec3(x, y, z);

			std::cout << "NDS: " << glm::to_string(ray_nds) << std::endl;


			glm::vec4 ray_clip = glm::vec4(ray_nds.x, -ray_nds.y, -1.0, 1.0);


			std::cout << "CLIP: " << glm::to_string(ray_clip) << std::endl;


			glm::vec4 ray_eye = glm::inverse(cam->GetProjectionMatrix()) * ray_clip;


			std::cout << "EYE: " << glm::to_string(ray_eye) << std::endl;

			glm::vec3 ray_wor = glm::vec3((inverse(cam->GetViewMatrix()) * glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0f)));

			std::cout << "WORLD: " << glm::to_string(ray_eye) << std::endl; 


		
			return false;
		}
		bool SimulationLayer::OnKeyPressed(Orion::KeyPressedEvent e)
		{
			if (e.GetKeyCode() == ORI_KEY_1)
			{
				Orion::CamerasController::SetActiveCamera("PrimaryCamera");
			}
			if (e.GetKeyCode() == ORI_KEY_2)
			{
				Orion::CamerasController::SetActiveCamera("SecondaryCamera");
			}

			return false;
		}


		void SimulationLayer::OnImGuiRender(Orion::Timestep ts) 
		{
			ImGui::Begin("Setting");
			ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));

			auto& stats = Orion::Renderer2D::GetStats();


			ImGui::Text("FPS: %f", ts.GetFPS());
			ImGui::Text("DrawCalls: %d", stats.GetTotalDrawCalls());
			ImGui::Text("Quads: %d", stats.GetTotalQuadCount());
			ImGui::Text("Lines: %d", stats.GetTotalLineCount());
			ImGui::Text("Vertcies: %d", stats.GetTotalVertexCount());
			ImGui::Text("Index: %d", stats.GetTotalQuadIndexCount());


			ImGui::End();

		}

}


