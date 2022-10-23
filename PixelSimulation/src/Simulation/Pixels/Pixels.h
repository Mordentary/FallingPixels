#pragma once
#include<vector>
#include<glm/glm.hpp>



namespace PixelSimulation
{

	enum PixelTypes
	{
		EMPTY = 0,
		STONE = 1,
		SAND = 2,
		WATER = 3
	};

	class AbstractPixel
	{
	public:

		 AbstractPixel() = default;
		~AbstractPixel() = default;

		
		void Update(std::vector<AbstractPixel>& matrix, int32_t index, int32_t screenWidth);
		void Draw(glm::vec3& screenPosition, glm::vec2& size);
		inline void SetType(PixelTypes type) { m_Type = type; };
		inline bool IsEqual(PixelTypes type) { return m_Type == type; };
		inline PixelTypes GetType() const {return m_Type;}

	private:
		PixelTypes m_Type = EMPTY;
		float m_lifeTime = 1.0f;
		bool m_Updated = false;
		glm::vec2 m_Velocity{0.f,0.f};
		glm::vec4 m_Color{1.0f};
	};


}