#ifndef AI_H
#define AI_H

#include <algorithm>
#include <limits>

#include "board.h"

class AI
{
public:
    AI(Board* board, int computerPiece, int playerPiece) : board(board), computerPiece(computerPiece), playerPiece(playerPiece) {}
    int computeMove();

private:
    Board* board;
    const int computerPiece, playerPiece;
    static const int startingDepth = 6;

    int minimax(int depth, bool maximizingPlayer);
    int getBoardHeuristicValue(int depth);
};

int AI::computeMove()
{
	int bestValue = std::numeric_limits<int>::min(), move = 0;

	for(int col = 0; col < Board::COLUMNS; col++)
    {
        if(board->isColumnOpen(col))
        {
            int row = board->addMove(col, computerPiece);

            int v = minimax(startingDepth, false);

            console.log(("Col " + std::to_string(col) + " Value " + std::to_string(v)).c_str());

            if (v > bestValue)
			{
				bestValue = v;
				move = col;
			}

            board->setPiece(row, col, Board::PIECE_EMPTY);
        }
    }

	return move;
}

int AI::minimax(int depth, bool maximizingPlayer)
{
    if(depth == 0 || board->checkWin() != Board::WIN_STATE_NOT_FINISHED)
        return getBoardHeuristicValue(depth);

    if(maximizingPlayer)
    {
        int bestValue = std::numeric_limits<int>::min();
        for(int col = 0; col < Board::COLUMNS; col++)
        {
            if(board->isColumnOpen(col))
            {
                int row = board->addMove(col, computerPiece);

                int v = minimax(depth - 1, false);
                bestValue = std::max(bestValue, v);

                board->setPiece(row, col, Board::PIECE_EMPTY);
            }
        }
        return bestValue;
    }
    else
    {
        int bestValue = std::numeric_limits<int>::max();
        for(int col = 0; col < Board::COLUMNS; col++)
        {
            if(board->isColumnOpen(col))
            {
                int row = board->addMove(col, playerPiece);

                int v = minimax(depth - 1, true);
                bestValue = std::min(bestValue, v);

                board->setPiece(row, col, Board::PIECE_EMPTY);
            }
        }
        return bestValue;
    }
}

int AI::getBoardHeuristicValue(int depth)
{
    int winState = board->checkWin();
    if(winState == board->pieceToWinState(computerPiece))
        return 1000 - (startingDepth - depth);
    else if(winState == board->pieceToWinState(playerPiece))
        return -1000 + (startingDepth - depth);
    else if(winState == Board::WIN_STATE_DRAW)
        return 0;

    int points = 0;

    //Check vertical
    for(int c = 0; c < Board::COLUMNS; c++)
    {
        for(int r = 0; r < Board::ROWS - 3; r++)
        {
            if(board->getPiece(r, c) == playerPiece && board->getPiece(r + 1, c) == playerPiece && board->getPiece(r + 2, c) == playerPiece && board->getPiece(r + 3, c) == Board::PIECE_EMPTY)
                points -= 3;
            if(board->getPiece(r, c) == Board::PIECE_EMPTY && board->getPiece(r + 1, c) == playerPiece && board->getPiece(r + 2, c) == playerPiece && board->getPiece(r + 3, c) == playerPiece)
                points -= 3;
            if(board->getPiece(r, c) == computerPiece && board->getPiece(r + 1, c) == computerPiece && board->getPiece(r + 2, c) == computerPiece && board->getPiece(r + 3, c) == Board::PIECE_EMPTY)
                points += 3;
            if(board->getPiece(r, c) == Board::PIECE_EMPTY && board->getPiece(r + 1, c) == computerPiece && board->getPiece(r + 2, c) == computerPiece && board->getPiece(r + 3, c) == computerPiece)
                points += 3;
        }
    }

    //Check horizontal
    for(int r = 0; r < Board::ROWS; r++)
    {
        for(int c = 0; c < Board::COLUMNS - 3; c++)
        {
            if(board->getPiece(r, c) == playerPiece && board->getPiece(r, c + 1) == playerPiece && board->getPiece(r, c + 2) == playerPiece && board->getPiece(r, c + 3) == Board::PIECE_EMPTY)
                points -= 3;
            if(board->getPiece(r, c) == Board::PIECE_EMPTY && board->getPiece(r, c + 1) == playerPiece && board->getPiece(r, c + 2) == playerPiece && board->getPiece(r, c + 3) == playerPiece)
                points -= 3;
            if(board->getPiece(r, c) == computerPiece && board->getPiece(r, c + 1) == computerPiece && board->getPiece(r, c + 2) == computerPiece && board->getPiece(r, c + 3) == Board::PIECE_EMPTY)
                points += 3;
            if(board->getPiece(r, c) == Board::PIECE_EMPTY && board->getPiece(r, c + 1) == computerPiece && board->getPiece(r, c + 2) == computerPiece && board->getPiece(r, c + 3) == computerPiece)
                points += 3;
        }
    }

    //Check diagonal
    for(int c = 0; c < Board::COLUMNS - 3; c++)
    {
        for(int r = 0; r < Board::ROWS - 3; r++)
        {
            if(board->getPiece(r, c) == playerPiece && board->getPiece(r + 1, c + 1) == playerPiece && board->getPiece(r + 2, c + 2) == playerPiece && board->getPiece(r + 3, c + 3) == Board::PIECE_EMPTY)
                points -= 3;
            if(board->getPiece(r, c) == Board::PIECE_EMPTY && board->getPiece(r + 1, c + 1) == playerPiece && board->getPiece(r + 2, c + 2) == playerPiece && board->getPiece(r + 3, c + 3) == playerPiece)
                points -= 3;
            if(board->getPiece(r, c) == computerPiece && board->getPiece(r + 1, c + 1) == computerPiece && board->getPiece(r + 2, c + 2) == computerPiece && board->getPiece(r + 3, c + 3) == Board::PIECE_EMPTY)
                points += 3;
            if(board->getPiece(r, c) == Board::PIECE_EMPTY && board->getPiece(r + 1, c + 1) == computerPiece && board->getPiece(r + 2, c + 2) == computerPiece && board->getPiece(r + 3, c + 3) == computerPiece)
                points += 3;

            if(board->getPiece(r + 3, c) == playerPiece && board->getPiece(r + 2, c + 1) == playerPiece && board->getPiece(r + 1, c + 2) == playerPiece && board->getPiece(r, c + 3) == Board::PIECE_EMPTY)
                points -= 3;
            if(board->getPiece(r + 3, c) == Board::PIECE_EMPTY && board->getPiece(r + 2, c + 1) == playerPiece && board->getPiece(r + 1, c + 2) == playerPiece && board->getPiece(r, c + 3) == playerPiece)
                points -= 3;
            if(board->getPiece(r + 3, c) == computerPiece && board->getPiece(r + 2, c + 1) == computerPiece && board->getPiece(r + 1, c + 2) == computerPiece && board->getPiece(r, c + 3) == Board::PIECE_EMPTY)
                points += 3;
            if(board->getPiece(r + 3, c) == Board::PIECE_EMPTY && board->getPiece(r + 2, c + 1) == computerPiece && board->getPiece(r + 1, c + 2) == computerPiece && board->getPiece(r, c + 3) == computerPiece)
                points += 3;
        }
    }

    return points;
}

#endif // AI_H
