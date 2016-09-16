#ifndef BOARD_H
#define BOARD_H

using namespace client;

class Board
{
public:
    static const int ROWS = 6;
    static const int COLUMNS = 7;

    static const int PIECE_EMPTY = 0;
    static const int PIECE_A = 1;
    static const int PIECE_B = 2;

    static const int WIN_STATE_NOT_FINISHED = 0;
    static const int WIN_STATE_DRAW = 1;
    static const int WIN_STATE_A = 2;
    static const int WIN_STATE_B = 3;

    Board();
    int getPiece(int row, int col);
    void setPiece(int row, int col, int piece);
    int addMove(int col, int piece);
    int checkWin();
    bool isColumnOpen(int col);
    int winStateToPiece(int winState);
    int pieceToWinState(int piece);
    void reset();

private:
    int pieces[ROWS][COLUMNS];
};

Board::Board()
{
    reset();
}

void Board::reset()
{
    for(int r = 0; r < ROWS; r++)
    {
        for(int c = 0; c < COLUMNS; c++)
        {
            pieces[r][c] = PIECE_EMPTY;
        }
    }
}

int Board::getPiece(int row, int col)
{
    return pieces[row][col];
}

void Board::setPiece(int row, int col, int piece)
{
    pieces[row][col] = piece;
}

int Board::addMove(int col, int piece)
{
    for(int row = ROWS - 1; row >= 0; row--)
    {
        if(getPiece(row, col) == PIECE_EMPTY)
        {
            setPiece(row, col, piece);
            return row;
        }
    }
}

int Board::checkWin()
{
    //Check vertical
    for(int c = 0; c < COLUMNS; c++)
    {
        for(int r = 0; r < ROWS - 3; r++)
        {
            if(getPiece(r, c) == PIECE_A && getPiece(r + 1, c) == PIECE_A && getPiece(r + 2, c) == PIECE_A && getPiece(r + 3, c) == PIECE_A)
                return WIN_STATE_A;
            if(getPiece(r, c) == PIECE_B && getPiece(r + 1, c) == PIECE_B && getPiece(r + 2, c) == PIECE_B && getPiece(r + 3, c) == PIECE_B)
                return WIN_STATE_B;
        }
    }

    //Check horizontal
    for(int r = 0; r < ROWS; r++)
    {
        for(int c = 0; c < COLUMNS - 3; c++)
        {
            if(getPiece(r, c) == PIECE_A && getPiece(r, c + 1) == PIECE_A && getPiece(r, c + 2) == PIECE_A && getPiece(r, c + 3) == PIECE_A)
                return WIN_STATE_A;
            if(getPiece(r, c) == PIECE_B && getPiece(r, c + 1) == PIECE_B && getPiece(r, c + 2) == PIECE_B && getPiece(r, c + 3) == PIECE_B)
                return WIN_STATE_B;
        }
    }

    //Check diagonal
    for(int c = 0; c < COLUMNS - 3; c++)
    {
        for(int r = 0; r < ROWS - 3; r++)
        {
            if(getPiece(r, c) == PIECE_A && getPiece(r + 1, c + 1) == PIECE_A && getPiece(r + 2, c + 2) == PIECE_A && getPiece(r + 3, c + 3) == PIECE_A)
                return WIN_STATE_A;
            if(getPiece(r, c) == PIECE_B && getPiece(r + 1, c + 1) == PIECE_B && getPiece(r + 2, c + 2) == PIECE_B && getPiece(r + 3, c + 3) == PIECE_B)
                return WIN_STATE_B;

            if(getPiece(r + 3, c) == PIECE_A && getPiece(r + 2, c + 1) == PIECE_A && getPiece(r + 1, c + 2) == PIECE_A && getPiece(r, c + 3) == PIECE_A)
                return WIN_STATE_A;
            if(getPiece(r + 3, c) == PIECE_B && getPiece(r + 2, c + 1) == PIECE_B && getPiece(r + 1, c + 2) == PIECE_B && getPiece(r, c + 3) == PIECE_B)
                return WIN_STATE_B;
        }
    }

    //Not finished
    for(int i = 0; i < COLUMNS; i++)
    {
        if(getPiece(0, i) == PIECE_EMPTY)
            return WIN_STATE_NOT_FINISHED;
    }

    //Draw
    return WIN_STATE_DRAW;
}

bool Board::isColumnOpen(int col)
{
    return getPiece(0, col) == PIECE_EMPTY;
}

int Board::winStateToPiece(int winState)
{
    if(winState == WIN_STATE_A)
        return PIECE_A;
    else if(winState == WIN_STATE_B)
        return PIECE_B;
    else
        return PIECE_EMPTY;
}

int Board::pieceToWinState(int piece)
{
    if(piece == PIECE_A)
        return WIN_STATE_A;
    else if(piece == PIECE_B)
        return WIN_STATE_B;
    else
        return WIN_STATE_NOT_FINISHED;
}

#endif // BOARD_H
