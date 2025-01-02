#pragma once
#include "Pixels.h"
#include <Orion/Core/Log.h>
#include <Orion/Core/Core.h>
#include <Orion/Renderer/GraphicsRendering/Renderer2D.h>
#include <random>
#include "../CellularMatrix.h"

namespace PixelSimulation
{
	namespace
	{
		bool RandomBool()
		{
			static std::mt19937 rng{ std::random_device{}() };
			static std::uniform_int_distribution<int> dist(0, 1);
			return (dist(rng) == 0);
		}
		bool RandomChance(int range)
		{
			static std::mt19937 rng{ std::random_device{}() };
			std::uniform_int_distribution<int> dist(1, range);
			return (dist(rng) == 1);
		}
		float RandomFloat()
		{
			static std::mt19937 rng{ std::random_device{}() };
			static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
			return dist(rng);
		}
	}

	void AbstractPixel::Update(CellularMatrix& matrix, int32_t x, int32_t y)
	{
		switch (m_Type)
		{
		case STONE:
		{
			// Stone remains static
			break;
		}

		case SAND:
		{
			if (matrix.InBound(x, y - 1) &&
				(matrix[{x, y - 1}].IsEqual(EMPTY) || matrix[{x, y - 1}].IsEqual(WATER)))
			{
				PixelTypes below = matrix[{x, y - 1}].GetType();
				matrix[{x, y}].SetType(below);
				matrix[{x, y - 1}].SetType(SAND);
			}
			else if (matrix.InBound(x - 1, y - 1) &&
				(matrix[{x - 1, y - 1}].IsEqual(EMPTY) || matrix[{x - 1, y - 1}].IsEqual(WATER)) &&
				(matrix.GetRow(x, y) - matrix.GetRow(x - 1, y - 1) < 2))
			{
				PixelTypes diagLeft = matrix[{x - 1, y - 1}].GetType();
				matrix[{x, y}].SetType(diagLeft);
				matrix[{x - 1, y - 1}].SetType(SAND);
			}
			else if (matrix.InBound(x + 1, y - 1) &&
				(matrix[{x + 1, y - 1}].IsEqual(EMPTY) || matrix[{x + 1, y - 1}].IsEqual(WATER)) &&
				(matrix.GetRow(x, y) - matrix.GetRow(x + 1, y - 1) < 2))
			{
				PixelTypes diagRight = matrix[{x + 1, y - 1}].GetType();
				matrix[{x, y}].SetType(diagRight);
				matrix[{x + 1, y - 1}].SetType(SAND);
			}
			break;
		}

		case WATER:
		{
			bool moved = false;

			if (!matrix.InBound(x, y)) {
				break;
			}

			if (matrix.InBound(x, y - 1) && matrix[{x, y - 1}].IsEqual(EMPTY))
			{
				matrix[{x, y}].SetType(EMPTY);
				matrix[{x, y - 1}].SetType(WATER);
				moved = true;
				break;
			}

			if (!moved)
			{
				bool canMoveDiagLeft = matrix.InBound(x - 1, y - 1) && matrix[{x - 1, y - 1}].IsEqual(EMPTY);
				bool canMoveDiagRight = matrix.InBound(x + 1, y - 1) && matrix[{x + 1, y - 1}].IsEqual(EMPTY);

				if (canMoveDiagLeft || canMoveDiagRight)
				{
					bool tryLeftFirst = RandomBool();

					auto tryDiagonalMove = [&](int dx) -> bool
						{
							int nx = x + dx;
							int ny = y - 1;

							if (matrix.InBound(nx, ny) && matrix[{nx, ny}].IsEqual(EMPTY))
							{
								if (RandomFloat() < 0.85)
								{
									matrix[{x, y}].SetType(EMPTY);
									matrix[{nx, ny}].SetType(WATER);
									return true;
								}
							}
							return false;
						};

					if (tryLeftFirst)
					{
						if (canMoveDiagLeft) moved = tryDiagonalMove(-1);
						if (!moved && canMoveDiagRight) moved = tryDiagonalMove(1);
					}
					else
					{
						if (canMoveDiagRight) moved = tryDiagonalMove(1);
						if (!moved && canMoveDiagLeft) moved = tryDiagonalMove(-1);
					}
				}

				if (!moved)
				{
					bool canMoveLeft = matrix.InBound(x - 1, y) && matrix[{x - 1, y}].IsEqual(EMPTY);
					bool canMoveRight = matrix.InBound(x + 1, y) && matrix[{x + 1, y}].IsEqual(EMPTY);

					if (canMoveLeft || canMoveRight)
					{
						int waterAbove = 0;
						for (int checkY = y + 1; checkY < std::min(y + 4, matrix.GetMatrixSize().second); checkY++)
						{
							if (matrix.InBound(x, checkY) && matrix[{x, checkY}].IsEqual(WATER))
							{
								waterAbove++;
							}
							else
							{
								break;
							}
						}

						float spreadProb = std::min(0.7f + (waterAbove * 0.1f), 0.95f);
						bool tryLeftFirst = RandomBool();

						auto tryHorizontalMove = [&](int dx) -> bool
							{
								int nx = x + dx;

								if (matrix.InBound(nx, y) && matrix[{nx, y}].IsEqual(EMPTY))
								{
									if (RandomFloat() < spreadProb)
									{
										matrix[{x, y}].SetType(EMPTY);
										matrix[{nx, y}].SetType(WATER);
										return true;
									}
								}
								return false;
							};

						if (tryLeftFirst)
						{
							if (canMoveLeft) moved = tryHorizontalMove(-1);
							if (!moved && canMoveRight) moved = tryHorizontalMove(1);
						}
						else
						{
							if (canMoveRight) moved = tryHorizontalMove(1);
							if (!moved && canMoveLeft) moved = tryHorizontalMove(-1);
						}
					}
				}
			}
			break;
		}

		case GAS:
		{
			bool moved = false;

			// Validate current position is in bounds
			if (!matrix.InBound(x, y)) {
				break;
			}

			auto canMoveToPosition = [&](int nx, int ny) -> bool {
				if (!matrix.InBound(nx, ny)) return false;
				return matrix[{nx, ny}].IsEqual(EMPTY) || matrix[{nx, ny}].IsEqual(WATER);
				};

			auto moveGas = [&](int nx, int ny) -> void {
				if (matrix[{nx, ny}].IsEqual(WATER)) {
					// If moving into water, swap positions
					matrix[{x, y}].SetType(WATER);
					matrix[{nx, ny}].SetType(GAS);
				}
				else {
					// Normal movement to empty space
					matrix[{x, y}].SetType(EMPTY);
					matrix[{nx, ny}].SetType(GAS);
				}
				};

			if (matrix.InBound(x, y + 1) && canMoveToPosition(x, y + 1))
			{
				moveGas(x, y + 1);
				moved = true;
				break;
			}

			if (!moved)
			{
				bool tryLeftFirst = RandomBool();

				auto tryDiagonalMove = [&](int dx) -> bool {
					int nx = x + dx;
					int ny = y + 1;

					if (canMoveToPosition(nx, ny))
					{
						if (RandomFloat() < 0.8)
						{
							moveGas(nx, ny);
							return true;
						}
					}
					return false;
					};

				if (tryLeftFirst)
				{
					if (matrix.InBound(x - 1, y + 1)) moved = tryDiagonalMove(-1);
					if (!moved && matrix.InBound(x + 1, y + 1)) moved = tryDiagonalMove(1);
				}
				else
				{
					if (matrix.InBound(x + 1, y + 1)) moved = tryDiagonalMove(1);
					if (!moved && matrix.InBound(x - 1, y + 1)) moved = tryDiagonalMove(-1);
				}
			}

			// If still hasn't moved, try horizontal movement with some drift
			if (!moved)
			{
				float driftFactor = RandomFloat() * 0.3;
				float horizontalChance = 0.4 + driftFactor;

				if (RandomFloat() < horizontalChance)
				{
					bool tryLeftFirst = RandomBool();

					auto tryHorizontalMove = [&](int dx) -> bool {
						int nx = x + dx;

						if (canMoveToPosition(nx, y))
						{
							moveGas(nx, y);
							return true;
						}
						return false;
						};

					if (tryLeftFirst)
					{
						if (matrix.InBound(x - 1, y)) moved = tryHorizontalMove(-1);
						if (!moved && matrix.InBound(x + 1, y)) moved = tryHorizontalMove(1);
					}
					else
					{
						if (matrix.InBound(x + 1, y)) moved = tryHorizontalMove(1);
						if (!moved && matrix.InBound(x - 1, y)) moved = tryHorizontalMove(-1);
					}
				}
			}
			break;
		}

		case FIRE:
		{
			m_lifeTime -= 0.05f;
			constexpr int offsets[4][2] = { {0,1}, {0,-1}, {1,0}, {-1,0} };

			for (auto& off : offsets)
			{
				int nx = x + off[0];
				int ny = y + off[1];

				if (!matrix.InBound(nx, ny))
					continue;

				auto& neighbor = matrix[{nx, ny}];

				if (neighbor.IsEqual(WOOD) || neighbor.IsEqual(GAS))
				{
					neighbor.SetType(FIRE);
				}
				else if (neighbor.IsEqual(WATER))
				{
					if (RandomChance(2))
					{
						matrix[{x, y}].SetType(EMPTY);
						neighbor.SetType(GAS);
					}
				}
			}

			if (m_lifeTime <= 0.0f)
			{
				matrix[{x, y}].SetType(EMPTY);
				break;
			}

			if (RandomChance(10))
			{
				matrix[{x, y}].SetType(EMPTY);
			}

			break;
		}

		case WOOD:
		{
			constexpr int offsets[4][2] = { {0,1}, {0,-1}, {1,0}, {-1,0} };
			for (auto& off : offsets)
			{
				int nx = x + off[0];
				int ny = y + off[1];
				if (matrix.InBound(nx, ny) && matrix[{nx, ny}].IsEqual(FIRE))
				{
					if (RandomChance(200))
					{
						matrix[{x, y}].SetType(FIRE);
					}
				}
			}
			break;
		}

		default:
			break;
		}
	}

	void AbstractPixel::Draw(glm::vec3& screenPosition, glm::vec2& size)
	{
		switch (m_Type)
		{
		case STONE:
		{
			m_Color = glm::vec4(0.43f, 0.53f, 0.6f, 1.0f) * m_lifeTime;
			Orion::Renderer2D::DrawBorderedQuad(screenPosition, size, m_Color);
			break;
		}
		case SAND:
		{
			m_Color = glm::vec4(0.74f, 0.7f, 0.4f, 1.0f) * m_lifeTime;
			Orion::Renderer2D::DrawBorderedQuad(screenPosition, size, m_Color);
			break;
		}
		case WATER:
		{
			m_Color = glm::vec4(0.3f, 0.2f, 0.8f, 1.0f) * m_lifeTime;
			Orion::Renderer2D::DrawBorderedQuad(screenPosition, size, m_Color);
			break;
		}
		case GAS:
		{
			m_Color = glm::vec4(0.8f, 0.8f, 0.8f, 0.25f) * m_lifeTime;
			Orion::Renderer2D::DrawBorderedQuad(screenPosition, size, m_Color);
			break;
		}
		case FIRE:
		{
			m_Color = glm::vec4(1.0f, 0.3f, 0.0f, 0.8f) * m_lifeTime;
			Orion::Renderer2D::DrawBorderedQuad(screenPosition, size, m_Color);
			break;
		}
		case WOOD:
		{
			m_Color = glm::vec4(0.55f, 0.27f, 0.07f, 1.0f) * m_lifeTime;
			Orion::Renderer2D::DrawBorderedQuad(screenPosition, size, m_Color);
			break;
		}
		default:
			break;
		}
	}
}