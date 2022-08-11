#include"SimulationLayer.h"
#include"imgui.h"
#include"glm/gtc/type_ptr.hpp"
#include"glm/gtx/string_cast.hpp"

namespace PixelSimulation
{

		void SimulationLayer::Init() 
		{
			Orion::Renderer::Init();

			m_Camera = std::make_unique<Orion::OrthographicCamera>(-1.6, 1.6, -0.9, 0.9);

			Orion::CamerasController2D::AddCamera("PrimaryCamera", m_Camera->GetAspectRatio());
			Orion::CamerasController2D::AddCamera("SecondaryCamera", m_Camera);

		}

		void SimulationLayer::OnUpdate(Orion::Timestep deltaTime) 
		{
			ORI_PROFILE_FUNCTION();

			Orion::RenderCommand::SetClearColor(glm::vec4(0.850f, 0.796f, 0.937f, 1.0f));
			Orion::RenderCommand::Clear();
			Orion::CamerasController2D::OnUpdate(deltaTime);
			Orion::Renderer2D::ResetStats();


			Orion::Renderer2D::BeginScene(Orion::CamerasController2D::GetActiveCamera());





			Orion::Renderer2D::EndScene();

		}


		void SimulationLayer::OnEvent(Orion::Event& event) 
		{

			Orion::CamerasController2D::OnEvent(event);
			m_Dispatcher = Orion::EventDispatcher::CreateDispatcher(event);

		}

		bool SimulationLayer::OnMouseMoved(Orion::MouseMovedEvent e)
		{



			return false;
		}
		bool SimulationLayer::OnKeyPressed(Orion::KeyPressedEvent e)
		{
			if (e.GetKeyCode() == ORI_KEY_1)
			{
				Orion::CamerasController2D::SetActiveCamera("PrimaryCamera");
			}
			if (e.GetKeyCode() == ORI_KEY_2)
			{
				Orion::CamerasController2D::SetActiveCamera("SecondaryCamera");
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
			ImGui::Text("Vertcies: %d", stats.GetTotalVertexCount());
			ImGui::Text("Index: %d", stats.GetTotalIndexCount());


			ImGui::End();

		}

}


