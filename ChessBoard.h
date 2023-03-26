
/*
	功能：棋盘数据模型
	时间：2023-3-20 16:22:57
*/


#pragma once

#include"cocos2d.h"
USING_NS_CC;

#include"ChessPiece.h"

class ChessBoard : public Ref
{
public:
	CREATE_FUNC(ChessBoard);
	virtual bool init();

	//获取白棋集合
	Vector<ChessPiece*>* getWhites();

	//获取黑棋集合
	Vector<ChessPiece*>* getBlack();

	//获得棋盘尺寸
	int getWidth() { return _width; };

	//获取下一次落子类型
	int getType() { return prt_type; };
	//设置下一次落子类型
	void setType(int type) { prt_type = type; };

private:

	//记录下一次落子类型
	int prt_type;

	//棋盘点阵宽度
	int _width;
	//棋盘点阵高度
	int _height;

	//记录已经下过的白棋子
	Vector<ChessPiece*> prt_whites;

	//记录已经下过的黑棋子
	Vector<ChessPiece*> prt_blacks;

};

