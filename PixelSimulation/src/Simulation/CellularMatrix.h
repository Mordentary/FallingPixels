#pragma once
#include"Pixels/Pixels.h"

namespace PixelSimulation
{
	const static float PixelSize = 5.f;
	struct Coord2D
	{
		int32_t x, y;
	};

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
		void UpdateElements();
		void DrawElements();

		//Helper methods
		inline int32_t GetRow(int32_t index) { return index / m_Width; }
		inline int32_t GetColumn(int32_t index) { return index % m_Width; }
		inline Coord2D GetCoord(int32_t index) { return { index % m_Width, index / m_Width }; }

		inline int32_t GetRow(int32_t x, int32_t y) { return GetIndex(x, y) / m_Width; }
		inline int32_t GetColumn(int32_t x, int32_t y) { return GetIndex(x, y) % m_Width; }

		inline int32_t GetIndex(int32_t x, int32_t y) { return x + (y * m_Width); }

		inline bool InBound(int32_t index) { return index <= m_Matrix.size() && index >= 0; }
		inline bool InBound(int32_t x, int32_t y) { return  (x >= 0 && y >= 0) && GetIndex(x, y) >= 0 && GetIndex(x, y) <= m_Matrix.size(); }

		std::pair<int32_t, int32_t> GetMatrixSize() { return { m_Width, m_Height }; };

		AbstractPixel& operator[](Coord2D coord)
		{
			return m_Matrix[coord.x + (coord.y * m_Width)];
		}
		AbstractPixel& operator[](int32_t index)
		{
			return m_Matrix[index];
		}

	private:
		int32_t m_Width, m_Height;
		std::vector<AbstractPixel> m_Matrix;
	};
}
