
/*
	功能：游戏逻辑控制器，实现落子和胜负盘定、交换棋手
	时间：2023-3-20 15:58:43
*/

#pragma once

#include"ChessBoard.h"

class GameController
{
public:
	static GameController* getInstance();
	static void destroyInstance();
	GameController();
	~GameController();

	/*
		作用：下棋落子
		参数1：int type 棋子类型，1表示白棋，2表示黑棋
		参数2：int x 点阵横坐标
		参数3：int y 点阵纵坐标
		返回值：bool 是否成功落子
	*/
	bool playChess(int type, int x, int y);

	/*
		作用：获取当前落子类型
		返回值：int 返回当前落子类型
	*/
	//bool getChessType() { return prt_type; };

	/*
		作用：交换棋手
		参数1：int type 棋子类型，1表示白棋，2表示黑棋
		参数2：int x 点阵横坐标
		参数3：int y 点阵纵坐标
		返回值：bool 胜负结果
	*/
	//bool player(int type, int x, int y);

	/*
		作用：胜负判定
		参数1：int type 棋子类型，1表示白棋，2表示黑棋
		参数2：int x 点阵横坐标
		参数3：int y 点阵纵坐标
		返回值：bool 胜负结果
	*/
	bool judgeWin(int type, int x, int y);

	//获取棋盘对象
	ChessBoard* getChessBoard() { return prt_board; };

private:
	static GameController* instance;

	//记录一个棋盘对象
	ChessBoard* prt_board;

	//判断当前点阵上的点的位置是否有棋子
	bool judgeHasChess(int x, int y);

	//判断当前点阵上的点的位置是否有指定类型的棋子
	bool judgeHasChess(int type, int x, int y);

};

