#pragma once
#include<Orion.h>
#include"CellularMatrix.h"


namespace PixelSimulation 
{


	class SimulationLayer : public Orion::Layer
	{
	public:
		SimulationLayer() : Orion::Layer("Simulation") {}
		void Init() override;

		void OnUpdate(Orion::Timestep deltaTime) override;

		void OnEvent(Orion::Event& event) override;

		bool OnWindowResized(Orion::WindowResizeEvent e);
		bool OnMouseMoved(Orion::MouseMovedEvent e);
		bool OnKeyPressed(Orion::KeyPressedEvent e);

		virtual void OnImGuiRender(Orion::Timestep ts) override;

		
	private:

		PixelSimulation::CellularMatrix m_Matrix;
		float m_ScreenHeight, m_ScreenWidth;
		Orion::ShaderLibrary m_ShaderLibrary;
		Orion::Shared<Orion::EventDispatcher> m_Dispatcher;
		Orion::Shared<Orion::OrthographicCamera> m_Camera;
		glm::vec4 m_Color{ 0.842f, 0.523f, 0.768f, 1.0f };
	};
}