#include "ChessBoard.h"

bool ChessBoard::init()
{
	_width = 19;
	_height = 19;

	//默认黑棋先手
	prt_type = 2;

	return true;
}

Vector<ChessPiece*>* ChessBoard::getWhites()
{
	return &prt_whites;
}

Vector<ChessPiece*>* ChessBoard::getBlack()
{
	return &prt_blacks;
}
