#include <cheerp/client.h>
#include <cheerp/clientlib.h>

#include <string>
#include <cstring>
#include <cstdlib>

#include "ai.h"
#include "board.h"

namespace client
{
    void logOnline(const String& str);
}

using namespace client;

const int ROWS = 6;
const int COLUMNS = 7;

HTMLElement* images[ROWS][COLUMNS];
HTMLElement* buttons[COLUMNS];

HTMLElement* mainLabel;
HTMLElement* resetButton;

Board board = Board();
AI ai = AI(&board, Board::PIECE_B, Board::PIECE_A);

bool playing = true;

HTMLElement* createImageElement(int rowNumber, int colNumber)
{
    HTMLElement* image = document.createElement("img");
    image->setAttribute("class", "piece blank");
    images[rowNumber][colNumber] = image;
    return image;
}

HTMLElement* createImageRow(int rowNumber)
{
    HTMLElement* row = document.createElement("tr");

    for(int i = 0; i < COLUMNS; i++)
    {
        HTMLElement* td = document.createElement("td");
        HTMLElement* image = createImageElement(rowNumber, i);

        td->appendChild(image);
        row->appendChild(td);
    }

    return row;
}

void createTable(HTMLElement* body)
{
    HTMLElement* table = document.getElementById("main-table");

    for(int i = 0; i < ROWS; i++)
    {
        HTMLElement* row = createImageRow(i);
        table->appendChild(row);
    }

    body->appendChild(table);
}

void updateBoard()
{
    for(int r = 0; r < ROWS; r++)
    {
        for(int c = 0; c < COLUMNS; c++)
        {
            int piece = board.getPiece(r, c);
            if(piece == Board::PIECE_EMPTY)
                (images[r][c])->setAttribute("class", "piece blank");
            else if(piece == Board::PIECE_A)
                (images[r][c])->setAttribute("class", "piece red");
            else if(piece == Board::PIECE_B)
                (images[r][c])->setAttribute("class", "piece black");
        }
    }
}

void setLabelText(const String& str)
{
    mainLabel->set_innerText(str);
}

int turn = Board::PIECE_A;

void changeTurn()
{
    if(turn == Board::PIECE_A)
    {
        turn = Board::PIECE_B;
        setLabelText("Black's Turn");
    }
    else
    {
        turn = Board::PIECE_A;
        setLabelText("Red's Turn");
    }
}

void handleMove(int col, HTMLElement* button)
{
    int row = board.addMove(col, turn);
    updateBoard();

    logOnline(("Col " + std::to_string(col) + " Row " + std::to_string(row) + " Turn " + std::to_string(turn)).c_str());

    int winState = board.checkWin();
    if(winState == Board::WIN_STATE_DRAW)
    {
        setLabelText("Its a draw");
        logOnline("Its a draw");
        playing = false;
    }
    else if(winState == Board::WIN_STATE_A)
    {
        setLabelText("Red wins");
        logOnline("Red wins");
        playing = false;
    }
    else if(winState == Board::WIN_STATE_B)
    {
        setLabelText("Black wins");
        logOnline("Black wins");
        playing = false;
    }
    else
    {
        changeTurn();
        int move = ai.computeMove();
        row = board.addMove(move, turn);
        updateBoard();

        logOnline(("Col " + std::to_string(col) + " Row " + std::to_string(row) + " Turn " + std::to_string(turn)).c_str());

        winState = board.checkWin();
        if(winState == Board::WIN_STATE_DRAW)
        {
            setLabelText("Its a draw");
            logOnline("Its a draw");
            playing = false;
        }
        else if(winState == Board::WIN_STATE_A)
        {
            setLabelText("Red wins");
            logOnline("Red wins");
            playing = false;
        }
        else if(winState == Board::WIN_STATE_B)
        {
            setLabelText("Black wins");
            logOnline("Black wins");
            playing = false;
        }
        else
        {
            changeTurn();
        }
    }
}

void moveButtonCallback(Event* event)
{
    if(playing)
    {
        HTMLElement* button = static_cast<HTMLElement*>(event->get_target());
        std::string idString = static_cast<std::string>(*button->getAttribute("id"));
        int id = atoi(idString.c_str());
        handleMove(id, button);
    }
}

void resetButtonCallback(Event* event)
{
    board.reset();
    updateBoard();
    turn = Board::PIECE_A;
    playing = true;
    setLabelText("Red's Turn");

    logOnline("Game Reset");
}

void setMoveButtonCallbacks()
{
    for(int i = 0; i < COLUMNS; i++)
    {
        HTMLElement* button = document.getElementById((std::to_string(i)).c_str());
        button->set_onclick(cheerp::Callback(moveButtonCallback));
        buttons[i] = button;
    }
}

void loadOtherElements()
{
    mainLabel = document.getElementById("main-label");
    resetButton = document.getElementById("reset-button");
    resetButton->set_onclick(cheerp::Callback(resetButtonCallback));
}

void loadCallback()
{
    HTMLElement* body = document.get_body();
    createTable(body);
    setMoveButtonCallbacks();
    loadOtherElements();

    logOnline("Game Loaded");
}

void webMain()
{
    document.addEventListener("DOMContentLoaded", cheerp::Callback(loadCallback));
}
