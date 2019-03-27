// Christos Savvopoulos <savvopoulos@gmail.com>
// Elias Sprengel <blockbattle@webagent.eu>

#ifndef __FIELD_H
#define __FIELD_H

#include <cassert>
#include <cstdlib>
#include <memory>
#include <string>

#include "cell.h"

using namespace std;

/**
 * Represents the playing field for one player.
 * Has some basic methods already implemented.
 */
class Field {
public:
	// Parses the input string to get a grid with Cell objects.
	Field(int width, int height, const string& fieldStr)
		: width_(width), height_(height), grid_(width * height) {
		int x = 0;
		int y = 0;
		const char* strPos = fieldStr.c_str();

		while (strPos < fieldStr.c_str() + fieldStr.size()) {
			// Read cell code.
			char* strNext = nullptr;
			int cellCode = strtol(strPos, &strNext, 10);
			assert(strPos != strNext);  // check that we read sth.
			strPos = strNext;
			strNext = nullptr;

			// Update this cell.
			grid_[y * width + x].SetLocation(x, y);
			grid_[y * width + x].set_state(cellCode);

			// Advance position, parse separator.
			x++;
			if (x == width_) {
				assert(*strPos == ';' || y == height_ - 1);
				x = 0;
				y++;
			}
			else {
				assert(*strPos == ',');
			}
			strPos++;
		}
	}

	int SolidRowCount() const
	{
		auto solidRowCount = 0;

		//Calculate solid lines
		for (auto y = height_ - 1; y >= 0; y--)
		{
			if (GetCell(0, y).IsSolid())
			{
				solidRowCount++;
			}
		}

		return solidRowCount;
	}

	bool CheckValidShapePosition(const int &shape, const int &rotation, const int &xPosition, const int &yPosition, double &moveScore)
	{
		auto shapeFits = true;
		Cell shapeCells[4];

		switch (shape)
		{
		case 0:
			if (rotation == 0 && xPosition + 3 < width_)
			{
				shapeCells[0] = GetCell(xPosition, yPosition);
				shapeCells[1] = GetCell(xPosition + 1, yPosition);
				shapeCells[2] = GetCell(xPosition + 2, yPosition);
				shapeCells[3] = GetCell(xPosition + 3, yPosition);
			}
			else if (rotation == 1 && yPosition - 3 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition);
				shapeCells[1] = GetCell(xPosition, yPosition - 1);
				shapeCells[2] = GetCell(xPosition, yPosition - 2);
				shapeCells[3] = GetCell(xPosition, yPosition - 3);
			}
			else
			{
				shapeFits = false;
			}
			break;
		case 1:
			if (rotation == 0 && xPosition + 2 < width_ && yPosition - 1 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition);
				shapeCells[1] = GetCell(xPosition + 1, yPosition);
				shapeCells[2] = GetCell(xPosition + 2, yPosition);
				shapeCells[3] = GetCell(xPosition, yPosition - 1);
			}
			else if (rotation == 1 && xPosition + 1 < width_ && yPosition - 2 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition);
				shapeCells[1] = GetCell(xPosition, yPosition - 1);
				shapeCells[2] = GetCell(xPosition, yPosition - 2);
				shapeCells[3] = GetCell(xPosition + 1, yPosition - 2);
			}
			else if (rotation == 2 && xPosition + 2 < width_ && yPosition - 1 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition - 1);
				shapeCells[1] = GetCell(xPosition + 1, yPosition - 1);
				shapeCells[2] = GetCell(xPosition + 2, yPosition - 1);
				shapeCells[3] = GetCell(xPosition + 2, yPosition);
			}
			else if (rotation == 3 && xPosition + 1 < width_ && yPosition - 2 >= 0)
			{
				shapeCells[0] = GetCell(xPosition + 1, yPosition);
				shapeCells[1] = GetCell(xPosition + 1, yPosition - 1);
				shapeCells[2] = GetCell(xPosition + 1, yPosition - 2);
				shapeCells[3] = GetCell(xPosition, yPosition);
			}
			else
			{
				shapeFits = false;
			}
			break;
		case 2:
			if (rotation == 0 && xPosition + 2 < width_ && yPosition - 1 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition);
				shapeCells[1] = GetCell(xPosition + 1, yPosition);
				shapeCells[2] = GetCell(xPosition + 2, yPosition);
				shapeCells[3] = GetCell(xPosition + 2, yPosition - 1);
			}
			else if (rotation == 1 && xPosition + 1 < width_ && yPosition - 2 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition);
				shapeCells[1] = GetCell(xPosition, yPosition - 1);
				shapeCells[2] = GetCell(xPosition, yPosition - 2);
				shapeCells[3] = GetCell(xPosition + 1, yPosition);
			}
			else if (rotation == 2 && xPosition + 2 < width_ && yPosition - 1 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition - 1);
				shapeCells[1] = GetCell(xPosition + 1, yPosition - 1);
				shapeCells[2] = GetCell(xPosition + 2, yPosition - 1);
				shapeCells[3] = GetCell(xPosition, yPosition);
			}
			else if (rotation == 3 && xPosition + 1 < width_ && yPosition - 2 >= 0)
			{
				shapeCells[0] = GetCell(xPosition + 1, yPosition);
				shapeCells[1] = GetCell(xPosition + 1, yPosition - 1);
				shapeCells[2] = GetCell(xPosition + 1, yPosition - 2);
				shapeCells[3] = GetCell(xPosition, yPosition - 2);
			}
			else
			{
				shapeFits = false;
			}
			break;
		case 3:
			if (rotation == 0 && xPosition + 1 < width_ && yPosition - 1 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition);
				shapeCells[1] = GetCell(xPosition + 1, yPosition);
				shapeCells[2] = GetCell(xPosition, yPosition - 1);
				shapeCells[3] = GetCell(xPosition + 1, yPosition - 1);
			}
			else
			{
				shapeFits = false;
			}
			break;
		case 4:
			if (rotation == 0 && xPosition + 2 < width_ && yPosition - 1 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition);
				shapeCells[1] = GetCell(xPosition + 1, yPosition);
				shapeCells[2] = GetCell(xPosition + 1, yPosition - 1);
				shapeCells[3] = GetCell(xPosition + 2, yPosition - 1);
			}
			else if (rotation == 1 && xPosition + 1 < width_ && yPosition - 2 >= 0)
			{
				shapeCells[0] = GetCell(xPosition + 1, yPosition);
				shapeCells[1] = GetCell(xPosition + 1, yPosition - 1);
				shapeCells[2] = GetCell(xPosition, yPosition - 1);
				shapeCells[3] = GetCell(xPosition, yPosition - 2);
			}
			else
			{
				shapeFits = false;
			}
			break;
		case 5:
			if (rotation == 0 && xPosition + 2 < width_ && yPosition - 1 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition);
				shapeCells[1] = GetCell(xPosition + 1, yPosition);
				shapeCells[2] = GetCell(xPosition + 2, yPosition);
				shapeCells[3] = GetCell(xPosition + 1, yPosition - 1);
			}
			else if (rotation == 1 && xPosition + 2 < width_ && yPosition - 2 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition);
				shapeCells[1] = GetCell(xPosition, yPosition - 1);
				shapeCells[2] = GetCell(xPosition, yPosition - 2);
				shapeCells[3] = GetCell(xPosition + 1, yPosition - 1);
			}
			else if (rotation == 2 && xPosition + 2 < width_ && yPosition - 1 >= 0)
			{
				shapeCells[0] = GetCell(xPosition + 1, yPosition);
				shapeCells[1] = GetCell(xPosition, yPosition - 1);
				shapeCells[2] = GetCell(xPosition + 1, yPosition - 1);
				shapeCells[3] = GetCell(xPosition + 2, yPosition - 1);
			}
			else if (rotation == 3 && xPosition + 1 < width_ && yPosition - 2 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition - 1);
				shapeCells[1] = GetCell(xPosition + 1, yPosition);
				shapeCells[2] = GetCell(xPosition + 1, yPosition - 1);
				shapeCells[3] = GetCell(xPosition + 1, yPosition - 2);
			}
			else
			{
				shapeFits = false;
			}
			break;
		case 6:
			if (rotation == 0 && xPosition + 2 < width_ && yPosition - 1 >= 0)
			{
				shapeCells[0] = GetCell(xPosition + 1, yPosition);
				shapeCells[1] = GetCell(xPosition + 2, yPosition);
				shapeCells[2] = GetCell(xPosition, yPosition - 1);
				shapeCells[3] = GetCell(xPosition + 1, yPosition - 1);
			}
			else if (rotation == 1 && xPosition + 1 < width_ && yPosition - 2 >= 0)
			{
				shapeCells[0] = GetCell(xPosition, yPosition);
				shapeCells[1] = GetCell(xPosition, yPosition - 1);
				shapeCells[2] = GetCell(xPosition + 1, yPosition - 1);
				shapeCells[3] = GetCell(xPosition + 1, yPosition - 2);
			}
			else
			{
				shapeFits = false;
			}
			break;

			default: 
			return false;
		}

		for (auto& cell : shapeCells)
		{
			if (!IsAccessible(cell))
			{
				shapeFits = false;
			}
		}

		//If the shape fits then we'll place the shape calculate a score to return based off the AI's heuristics
		if (shapeFits)
		{
			for (auto& cell : shapeCells)
			{
				SetCell(cell.x(), cell.y(), 2);
			}

			//Calculate score
			CalculateMoveScore(moveScore);

			//Set the cells back to empty after the score is calculated
			for (auto& cell : shapeCells)
			{
				SetCell(cell.x(), cell.y(), 0);
			}
		}

		return shapeFits;
	}

	bool CheckTwoPieceCollision(const int shape[2], const int rotation[2], const int xPosition[2], const int yPosition[2])
	{
		auto collision = false;
		Cell shapeCells[8];

		for(unsigned int i = 0; i < 8; i += 4)
		{
			switch (shape[i])
			{
			case 0:
				if (rotation[i] == 0)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i] + 1, yPosition[i]);
					shapeCells[i+2] = GetCell(xPosition[i] + 2, yPosition[i]);
					shapeCells[i+3] = GetCell(xPosition[i] + 3, yPosition[i]);
				}
				else if (rotation[i] == 1)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i], yPosition[i] - 1);
					shapeCells[i+2] = GetCell(xPosition[i], yPosition[i] - 2);
					shapeCells[i+3] = GetCell(xPosition[i], yPosition[i] - 3);
				}
				break;
			case 1:
				if (rotation[i] == 0)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i] + 1, yPosition[i]);
					shapeCells[i+2] = GetCell(xPosition[i] + 2, yPosition[i]);
					shapeCells[i+3] = GetCell(xPosition[i], yPosition[i] - 1);
				}
				else if (rotation[i] == 1)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i], yPosition[i] - 1);
					shapeCells[i+2] = GetCell(xPosition[i], yPosition[i] - 2);
					shapeCells[i+3] = GetCell(xPosition[i] + 1, yPosition[i] - 2);
				}
				else if (rotation[i] == 2)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i] - 1);
					shapeCells[i+1] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
					shapeCells[i+2] = GetCell(xPosition[i] + 2, yPosition[i] - 1);
					shapeCells[i+3] = GetCell(xPosition[i] + 2, yPosition[i]);
				}
				else if (rotation[i] == 3)
				{
					shapeCells[i] = GetCell(xPosition[i] + 1, yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
					shapeCells[i+2] = GetCell(xPosition[i] + 1, yPosition[i] - 2);
					shapeCells[i+3] = GetCell(xPosition[i], yPosition[i]);
				}
				break;
			case 2:
				if (rotation[i] == 0)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i] + 1, yPosition[i]);
					shapeCells[i+2] = GetCell(xPosition[i] + 2, yPosition[i]);
					shapeCells[i+3] = GetCell(xPosition[i] + 2, yPosition[i] - 1);
				}
				else if (rotation[i] == 1)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i], yPosition[i] - 1);
					shapeCells[i+2] = GetCell(xPosition[i], yPosition[i] - 2);
					shapeCells[i+3] = GetCell(xPosition[i] + 1, yPosition[i]);
				}
				else if (rotation[i] == 2)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i] - 1);
					shapeCells[i+1] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
					shapeCells[i+2] = GetCell(xPosition[i] + 2, yPosition[i] - 1);
					shapeCells[i+3] = GetCell(xPosition[i], yPosition[i]);
				}
				else if (rotation[i] == 3)
				{
					shapeCells[i] = GetCell(xPosition[i] + 1, yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
					shapeCells[i+2] = GetCell(xPosition[i] + 1, yPosition[i] - 2);
					shapeCells[i+3] = GetCell(xPosition[i], yPosition[i] - 2);
				}
				break;
			case 3:
				if (rotation[i] == 0)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i] + 1, yPosition[i]);
					shapeCells[i+2] = GetCell(xPosition[i], yPosition[i] - 1);
					shapeCells[i+3] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
				}
				break;
			case 4:
				if (rotation[i] == 0)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i] + 1, yPosition[i]);
					shapeCells[i+2] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
					shapeCells[i+3] = GetCell(xPosition[i] + 2, yPosition[i] - 1);
				}
				else if (rotation[i] == 1)
				{
					shapeCells[i] = GetCell(xPosition[i] + 1, yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
					shapeCells[i+2] = GetCell(xPosition[i], yPosition[i] - 1);
					shapeCells[i+3] = GetCell(xPosition[i], yPosition[i] - 2);
				}
				break;
			case 5:
				if (rotation[i] == 0)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i] + 1, yPosition[i]);
					shapeCells[i+2] = GetCell(xPosition[i] + 2, yPosition[i]);
					shapeCells[i+3] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
				}
				else if (rotation[i] == 1)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i], yPosition[i] - 1);
					shapeCells[i+2] = GetCell(xPosition[i], yPosition[i] - 2);
					shapeCells[i+3] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
				}
				else if (rotation[i] == 2)
				{
					shapeCells[i] = GetCell(xPosition[i] + 1, yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i], yPosition[i] - 1);
					shapeCells[i+2] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
					shapeCells[i+3] = GetCell(xPosition[i] + 2, yPosition[i] - 1);
				}
				else if (rotation[i] == 3)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i] - 1);
					shapeCells[i+1] = GetCell(xPosition[i] + 1, yPosition[i]);
					shapeCells[i+2] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
					shapeCells[i+3] = GetCell(xPosition[i] + 1, yPosition[i] - 2);
				}
				break;
			case 6:
				if (rotation[i] == 0)
				{
					shapeCells[i] = GetCell(xPosition[i] + 1, yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i] + 2, yPosition[i]);
					shapeCells[i+2] = GetCell(xPosition[i], yPosition[i] - 1);
					shapeCells[i+3] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
				}
				else if (rotation[i] == 1)
				{
					shapeCells[i] = GetCell(xPosition[i], yPosition[i]);
					shapeCells[i+1] = GetCell(xPosition[i], yPosition[i] - 1);
					shapeCells[i+2] = GetCell(xPosition[i] + 1, yPosition[i] - 1);
					shapeCells[i+3] = GetCell(xPosition[i] + 1, yPosition[i] - 2);
				}
				break;

			default:
				return false;
			}
		}

		//Set first piece cells to grid
		for(unsigned int i = 0; i < 4; i++)
		{
			SetCell(shapeCells[i].x(), shapeCells[i].y(), 2);
		}

		//Check for a collision
		//Now get the cells of the second piece and check if they are blocked, if so we have a collision
		for(unsigned int i = 4; i < 8; i++)
		{
			if (shapeCells[i].IsBlock())
			{
				collision = true;
			}
		}

		//Reset cells
		for (unsigned int i = 0; i < 4; i++)
		{
			SetCell(shapeCells[i].x(), shapeCells[i].y(), 2);
		}

		return collision;
	}

	bool IsAccessible(const Cell& c) const
	{
		auto accessible = true;

		const auto xPosition = c.x();
		const auto yPosition = c.y();

		const auto loopLimit = yPosition - 8 < 0 ? 0 : yPosition - 8;

		for (auto y = yPosition; y >= loopLimit; y--)
		{
			auto cell = GetCell(xPosition, y);

			if (cell.IsBlock() || cell.IsSolid())
			{
				accessible = false;
				break;
			}
		}		

		return accessible;
	}

	bool DetectGameLoss() const
	{
		vector<int> blockPositions;

		for (auto x = 0; x < width_; x++)
		{
			if (GetCell(x, 0).IsShape())
			{
				blockPositions.emplace_back(x);
			}
		}

		auto count = 0;

		for (auto x : blockPositions)
		{
			if (GetCell(x, 1).IsBlock())
			{
				return true;
			}
		}

		return false;
	}

	bool IsOutOfBounds(const Cell& c) const{
		return c.x() >= width_ || c.x() < 0 || c.y() >= height_ || c.y() < 0;
	}

	bool HasCollision(const Cell& block_cell) const {
		const Cell& field_cell = GetCell(block_cell.x(), block_cell.y());
		return (block_cell.IsShape() &&
			(field_cell.IsSolid() || field_cell.IsBlock()));
	}

	const Cell& GetCell(int x, int y) const { return grid_[y * width_ + x]; }
	void SetCell(const int x, const int y, const int state) { grid_[y*width_ + x].set_state(state); }

	int width() const { return width_; }

	int height() const { return height_; }

private:
	void CalculateMoveScore(double &totalScore) const
	{
		int heights[10];

		auto sumOfHeights = 0;
		auto completedLines = 0;
		auto blockHolesCount = 0;
		auto surfaceRoughness = 0;

		//Analysis the grid and gather values to determine the above values

		//Calculate sumOfHeights and store heights and keep track of inaccessible blocked holes
		for (auto x = 0; x < width_; x++)
		{
			auto yHoleCount = 0;
			auto height = 0;

			for (auto y = height_ - 1; y >= 0; y--)
			{
				auto cell = GetCell(x, y);

				if (cell.IsBlock() || cell.IsSolid())
				{
					height = height_ - y;
				}

				if (cell.IsEmpty())
				{
					yHoleCount++;


					continue;
				}

				if (cell.IsBlock() || cell.IsSolid())
				{
					blockHolesCount += yHoleCount;
					yHoleCount = 0;
				}
			}

			sumOfHeights += height;
			heights[x] = height;
		}

		//Calculate completedLines
		for (auto y = 0; y < height_; y++)
		{
			for (auto x = 0; x < width_; x++)
			{
				const auto cell = GetCell(x, y);

				if (cell.IsEmpty() || cell.IsSolid())
				{
					break;
				}
				
				if (x == width_ - 1)
				{
					completedLines++;
				}
			}
		}

		//Calculate surface roughness
		for (unsigned int i = 0; i < 9; i++)
		{
			surfaceRoughness += abs(heights[i] - heights[i + 1]);
		}

		//cerr << "SumOfHeights: " << sumOfHeights << ", CompletedLines: " << completedLines << ", BlockedHolesCount: " << blockHolesCount << ", SurfaceRoughness: " << surfaceRoughness << endl;

		totalScore = m_sumOfHeightsWeight * sumOfHeights + m_completedLinesWeight * completedLines + m_blockedHoleCountWeight * blockHolesCount + m_surfaceRoughness * surfaceRoughness;

		//cerr << "MoveScore: " << totalScore << endl;
	}

	double m_sumOfHeightsWeight = -0.510066;
	double m_completedLinesWeight = 0.760666;
	double m_blockedHoleCountWeight = -0.35663;
	double m_surfaceRoughness = -0.184483;

	int width_;
	int height_;
	vector<Cell> grid_;
};

#endif  // __FIELD_H
