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
		app.GetWindow().SetVSync(true);
		ORI_INFO("{0}, {1}", m_ScreenWidth, m_ScreenHeight);
		m_Matrix = PixelSimulation::CellularMatrix(m_ScreenWidth, m_ScreenHeight);
		m_Camera = Orion::CreateShared<Orion::OrthographicCamera>(glm::vec3(0.0f), false, true);
		Orion::CamerasController::AddCamera("SecondaryCamera", Orion::CreateShared<Orion::PerspectiveCamera>(glm::vec3(0.f, 0.f, 0.0f), glm::vec3(0.f, 0.f, -1.f)));
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

		auto [mouseX, mouseY] = Orion::Input::GetMousePosition();

		mouseY = (m_ScreenHeight - mouseY);

		m_UpdateAccumulator += deltaTime;

		while (m_UpdateAccumulator >= m_FixedTimeStep)
		{
			m_Matrix.UpdateElements(); // Update simulation at fixed time step
			m_UpdateAccumulator -= m_FixedTimeStep;
		}

		if (Orion::Input::IsMouseButtonPressed(ORI_MOUSE_BUTTON_LEFT))
		{
			m_Matrix.SetCell(mouseX, mouseY, m_SelectedPixelType);
		}
		if (Orion::Input::IsMouseButtonPressed(ORI_MOUSE_BUTTON_4))
		{
			m_Matrix.SetCell(mouseX, mouseY, EMPTY);
		}

		Orion::Renderer2D::BeginScene(Orion::CamerasController::GetActiveCamera());

		m_Matrix.DrawElements();

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
		m_Matrix.Invalidate(e.GetWidth(), e.GetHeight());
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

		/*glm::vec3 ray_nds = glm::vec3(x, y, z);

		std::cout << "NDS: " << glm::to_string(ray_nds) << std::endl;

		glm::vec4 ray_clip = glm::vec4(ray_nds.x, -ray_nds.y, -1.0, 1.0);

		std::cout << "CLIP: " << glm::to_string(ray_clip) << std::endl;

		glm::vec4 ray_eye = glm::inverse(cam->GetProjectionMatrix()) * ray_clip;

		std::cout << "EYE: " << glm::to_string(ray_eye) << std::endl;

		glm::vec3 ray_wor = glm::vec3((inverse(cam->GetViewMatrix()) * glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0f)));

		std::cout << "WORLD: " << glm::to_string(ray_eye) << std::endl; */

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
		ImGui::Begin("Settings");

		ImGui::Text("Select Pixel Type:");

		struct PixelOption
		{
			PixelTypes type;
			const char* name;
			glm::vec4 color;
		};

		PixelOption options[] = {
			{ EMPTY, "Empty", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f) },
			{ STONE, "Stone", glm::vec4(0.43f, 0.53f, 0.6f, 1.0f) },
			{ SAND, "Sand", glm::vec4(0.74f, 0.7f, 0.4f, 1.0f) },
			{ WATER, "Water", glm::vec4(0.3f, 0.2f, 0.8f, 1.0f) },
			{ GAS, "Gas", glm::vec4(0.8f, 0.8f, 0.8f, 0.2f) },
			{ FIRE, "Fire", glm::vec4(1.0f, 0.3f, 0.0f, 0.8f) },
			{ WOOD, "Wood", glm::vec4(0.55f, 0.27f, 0.07f, 1.0f) },
		};

		// Display buttons for each pixel type
		for (auto& option : options)
		{
			// Color button or small colored square
			ImGui::PushID(option.type); // Unique ID per button
			if (ImGui::ColorButton("", ImVec4(option.color.x, option.color.y, option.color.z, option.color.w), ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20)))
			{
				m_SelectedPixelType = option.type;
			}
			ImGui::SameLine();
			if (ImGui::Button(option.name))
			{
				m_SelectedPixelType = option.type;
			}
			ImGui::PopID();
		}

		ImGui::Separator();

		// Display the currently selected type
		PixelOption* current = nullptr;
		for (auto& option : options)
		{
			if (option.type == m_SelectedPixelType)
			{
				current = &option;
				break;
			}
		}

		if (current)
		{
			ImGui::Text("Current Selection: %s", current->name);
			ImGui::SameLine();
			ImGui::ColorButton("##CurrentColor", ImVec4(current->color.x, current->color.y, current->color.z, current->color.w), ImGuiColorEditFlags_NoTooltip, ImVec2(20, 20));
		}

		// Existing stats display
		auto& stats = Orion::Renderer2D::GetStats();

		ImGui::Text("FPS: %.1f", ts.GetFPS());
		ImGui::Text("Draw Calls: %d", stats.GetTotalDrawCalls());
		ImGui::Text("Quads: %d", stats.GetTotalQuadCount());
		ImGui::Text("Lines: %d", stats.GetTotalLineCount());
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalQuadIndexCount());

		ImGui::End();
	}
}