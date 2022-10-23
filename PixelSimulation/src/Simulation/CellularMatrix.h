#pragma once
#include"vector"
#include <Orion.h>


namespace PixelSimulation 
{

	class CellularMatrix
	{
	public:
		CellularMatrix(uint32_t width, uint32_t height);
		CellularMatrix() = default;

		~CellularMatrix() = default;

		void SetCell(uint32_t index, int32_t val);
		void SetCell(uint32_t x, uint32_t y, int32_t val);
		inline int32_t GetCell(uint32_t  index);
		void DrawElemets();

	
	private:
		std::vector<int32_t> m_Matrix;
		int32_t m_Width, m_Height;

	};
}

