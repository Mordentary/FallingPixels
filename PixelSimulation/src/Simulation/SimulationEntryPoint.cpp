#include<Orion.h>

#include"Orion/Core/EntryPoint.h"
#include"SimulationLayer.h"



class Simulation : public Orion::Application 
{
	
public: 
	Simulation() 
	{
		PushLayer(new PixelSimulation::SimulationLayer());
	}
	~Simulation() 
	{
	
	}
};

Orion::Application* Orion::CreateApplication()
{
	return new Simulation();
}