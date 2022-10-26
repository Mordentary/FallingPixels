#pragma once
#include"Pixels/Pixels.h"

namespace PixelSimulation 
{
	const static float PixelSize = 10.f;

	class CellularMatrix
	{
	public:
		CellularMatrix(int32_t width, int32_t height);

		CellularMatrix() = default;
		~CellularMatrix() = default;

		//TODO: Fix invalidate..
		void Invalidate(int32_t width, int32_t height);

		void SetCell(uint32_t index, PixelTypes val);
		void SetCell(int32_t x, int32_t y, PixelTypes val);
		inline PixelTypes GetCell(uint32_t  index);
		void DrawElemets();

		//Help method
		inline int32_t GetRow(int32_t index) { return index / m_Width; }
		inline int32_t GetColumn(int32_t index) { return index / m_Height; }



		std::pair<int32_t, int32_t> GetMatrixSize() { return { m_Width, m_Height }; };
	
	private:

		int32_t m_Width, m_Height;
		std::vector<AbstractPixel> m_Matrix;

	};
}

