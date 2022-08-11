#include<Orion.h>

#include"Orion/Core/EntryPoint.h"
#include"SimulationLayer.h"



class Sandbox : public Orion::Application 
{
	
public: 
	Sandbox() 
	{
		PushLayer(new PixelSimulation::SimulationLayer());
	}
	~Sandbox() 
	{
	
	}
};

Orion::Application* Orion::CreateApplication()
{
	return new Sandbox();
}