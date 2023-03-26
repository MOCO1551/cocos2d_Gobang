
/*
	功能：棋子数据模型
	时间：2023-3-20 16:22:57
*/

#pragma once

#include"cocos2d.h"
USING_NS_CC;



class ChessPiece :public Ref
{
public:
	CREATE_FUNC(ChessPiece);
	virtual bool init();

	//获取棋子的点阵横坐标位置
	int getX() { return _x; };
	//获取棋子的点阵横坐标位置
	void setX(int x) { _x = x; };
	//获取棋子的点阵横坐标位置
	int getY() { return _y; };
	//获取棋子的点阵横坐标位置
	void setY(int y) { _y = y; };

private:
	//棋子的点阵横坐标位置
	int _x;
	//棋子的点阵纵坐标位置
	int _y;
};

