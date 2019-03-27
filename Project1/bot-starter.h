// Christos Savvopoulos <savvopoulos@gmail.com>
// Elias Sprengel <blockbattle@webagent.eu>

#ifndef __BOT_STARTER_H
#define __BOT_STARTER_H

#include <cstdlib>
#include <vector>

#include "bot-state.h"
#include "move.h"

#include <fstream>

using namespace std;

/**
 * This class is where the main logic should be. Implement getMoves() to
 * return something better than random moves.
 */
class BotStarter {
public:
	/**
	 * Returns a random amount of random moves
	 * @param state : current state of the bot
	 * @param timeout : time to respond
	 * @return : a list of moves to execute
	 */
	vector<Move::MoveType> GetMoves(BotState& state,long long timeout) {

		vector<Move::MoveType> bestMoveSet;

		//Score, rotation, position
		multimap<float, tuple<int, Point>, greater<>> pieceOneAllPossibleMoves = {};
		multimap<float, tuple<int, Point>, greater<>> pieceTwoAllPossibleMoves = {};

		auto rotation = 0;
		auto xPosition = 0;
		auto yPosition = 0;

		if (!state.MyField().DetectGameLoss())
		{
			ofstream foutStream;

			foutStream.open("test.txt", fstream::out);

			foutStream << "We're Winning!" << endl;

			foutStream.close();
		}
		else
		{
			ofstream foutStream;

			foutStream.open("test.txt", fstream::out);

			foutStream << "We Lost!" << endl;

			foutStream.close();
		}

		const auto solidRowCount = state.MyField().SolidRowCount();

		//cerr << "Number of Solid Rows: " << solidRowCount << endl;

		//cerr << "Current Piece: " << endl;

		//Get all possible moves for the current piece
		//Loop through each possible rotation
		for (rotation = 0; rotation < 4; rotation++)
		{
			//Loop through the grid, start from the bottom left
			for (xPosition = 0; xPosition < state.MyField().width(); xPosition++)
			{
				for (yPosition = state.MyField().height() - (1 + solidRowCount); yPosition >= 0;  yPosition--)
				{
					double moveScore = 0.0f;

					//Checks if position is valid for the shape and is accessible
					if (state.MyField().CheckValidShapePosition(state.CurrentShape(), rotation, xPosition, yPosition, moveScore))
					{
						//We'll just insert a max of the five best possible moves, we ignore moves that produce the same score
						pieceOneAllPossibleMoves.insert(pieceOneAllPossibleMoves.begin(), make_pair(moveScore, make_tuple(rotation, make_pair(xPosition, yPosition))));

						//cerr << "Possible position with rotation " << rotation << " at position x" << xPosition << " y" << yPosition << endl << endl;
						break;
					}
				}
			}
		}

		//cerr << endl << "Next Piece: " << endl;

		//Get all possible moves for the next piece
		//Loop through each possible rotation
		for (rotation = 0; rotation < 4; rotation++)
		{
			//Loop through the grid, start from the bottom left
			for (xPosition = 0; xPosition < state.MyField().width(); xPosition++)
			{
				for (yPosition = state.MyField().height() - 1; yPosition >= 0; yPosition--)
				{
					double moveScore = 0.0f;

					//Checks if position is valid for the shape and is accessible
					if (state.MyField().CheckValidShapePosition(state.NextShape(), rotation, xPosition, yPosition, moveScore))
					{
						//We'll just insert a max of the five best possible moves, we ignore moves that produce the same score
						pieceTwoAllPossibleMoves.insert(pieceTwoAllPossibleMoves.begin(), make_pair(moveScore, make_tuple(rotation, make_pair(xPosition, yPosition))));

						//cerr << "Possible position with rotation " << rotation << " at position x" << xPosition << " y" << yPosition << endl << endl;
						break;
					}
				}
			}
		}

		auto secondPieceCount = 0;
		auto firstPieceCount = 0;
		const auto lookAheadLimitFirst = 10;
		const auto lookAheadLimitSecond = 10;

		auto bestCombinationFirst = 0;
		auto bestCombinationSecond = 0;
		auto bestTotalScoreCombination = -10000000.0;

		auto bestRotation = 0;
		auto bestXPosition = 0;
		auto bestYPosition = 0;

		//Iterative though all possible moves for both pieces and find the best move to make with the first piece in mind
		//(i.e. Best combination of score without a collision between the pieces)
		for (auto firstPiece = pieceOneAllPossibleMoves.begin(); firstPiece != pieceOneAllPossibleMoves.end() && firstPieceCount < lookAheadLimitFirst; ++firstPiece)
		{
			secondPieceCount = 0;

			for (auto secondPiece = pieceTwoAllPossibleMoves.begin(); secondPiece != pieceTwoAllPossibleMoves.end() && secondPieceCount < lookAheadLimitSecond; ++secondPiece)
			{
				if (firstPiece->first + secondPiece->first > bestTotalScoreCombination)
				{
					int shapes[2] = { state.CurrentShape(), state.NextShape() };
					int rotations[2] = { std::get<0>(firstPiece->second), std::get<0>(secondPiece->second) };
					int xPositions[2] = { std::get<1>(firstPiece->second).first, std::get<1>(secondPiece->second).first };
					int yPositions[2] = { std::get<1>(firstPiece->second).second, std::get<1>(secondPiece->second).second };

					//Check for a collision
					if (!state.MyField().CheckTwoPieceCollision(shapes, rotations, xPositions, yPositions))
					{
						bestCombinationFirst = firstPieceCount;
						bestCombinationSecond = secondPieceCount;

						bestTotalScoreCombination = firstPiece->first + secondPiece->first;

						bestRotation = std::get<0>(firstPiece->second);
						bestXPosition = std::get<1>(firstPiece->second).first;
						bestYPosition = std::get<1>(firstPiece->second).second;
					}
				}

				secondPieceCount++;
			}

			firstPieceCount++;
		}

		//cerr << "Best Move Combination: " << "CurrentPiece " << bestCombinationFirst << " and SecondPiece " << bestCombinationSecond << endl;
		//cerr << "Rotation: " << bestRotation << " XPosition: " << bestXPosition << " YPosition: " << bestYPosition << endl;

		//Add translation to moveset
		auto xCurrentPosition = state.ShapeLocation().first;
		auto yCurrentPosition = state.ShapeLocation().second;

		//cerr << "CurrentXPosition: " << xCurrentPosition << " CurrentYPosition: " << yCurrentPosition << endl;

		CorrectCurrentPosition(state.CurrentShape(), bestRotation, xCurrentPosition, yCurrentPosition);

		//cerr << "AdjustedXPosition: " << xCurrentPosition << " AdjustedYPosition: " << yCurrentPosition << endl;

		//Add rotations to moveset
		for (auto i = 0; i < bestRotation; i++)
		{
			bestMoveSet.emplace_back(Move::MoveType::TURNRIGHT);
		}

		if (xCurrentPosition < bestXPosition)
		{
			while (xCurrentPosition != bestXPosition)
			{
				bestMoveSet.emplace_back(Move::MoveType::RIGHT);
				xCurrentPosition++;
			}
		}

		if (xCurrentPosition > bestXPosition)
		{
			while (xCurrentPosition != bestXPosition)
			{
				bestMoveSet.emplace_back(Move::MoveType::LEFT);
				xCurrentPosition--;
			}
		}

		bestMoveSet.emplace_back(Move::MoveType::DROP);

		//Calculate move set

		return bestMoveSet;
	}

private:
	void CorrectCurrentPosition(const int shape, const int rotation, int &currentXPosition, int &currentYPosition)
	{
		switch (shape)
		{
		case 0:
			if (rotation == 0)
			{

			}
			else if (rotation == 1)
			{
				currentXPosition += 2;
			}
			break;
		case 1:
			if (rotation == 0)
			{

			}
			else if (rotation == 1)
			{
				currentXPosition += 1;
			}
			else if (rotation == 2)
			{

			}
			else if (rotation == 3)
			{

			}
			break;
		case 2:
			if (rotation == 0)
			{

			}
			else if (rotation == 1)
			{
				currentXPosition += 1;
			}
			else if (rotation == 2)
			{

			}
			else if (rotation == 3)
			{

			}

			break;
		case 3:
			if (rotation == 0)
			{

			}

			break;
		case 4:
			if (rotation == 0)
			{

			}
			else if (rotation == 1)
			{
				currentXPosition += 1;
			}
			break;
		case 5:
			if (rotation == 0)
			{

			}
			else if (rotation == 1)
			{
				currentXPosition += 1;
			}
			else if (rotation == 2)
			{

			}
			else if (rotation == 3)
			{

			}
			break;
		case 6:
			if (rotation == 0)
			{

			}
			else if (rotation == 1)
			{
				currentXPosition += 1;
			}
			break;

		default:
			break;
		}
	}
};

#endif  //__BOT_STARTER_H
