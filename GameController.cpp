#include "GameController.h"

#define WIN_CONDITION 5

GameController* GameController::instance = nullptr;

GameController* GameController::getInstance()
{
	if (instance == nullptr)
	{
		instance = new GameController();
	}
	return instance;
}

void GameController::destroyInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

GameController::GameController()
{
	prt_board = ChessBoard::create();
	CC_SAFE_RETAIN(prt_board);
}

GameController::~GameController()
{
	//销毁棋盘对象
	CC_SAFE_DELETE(prt_board);
}

bool GameController::playChess(int type, int x, int y)
{
	bool result = false;

	//判定是否已有棋子
	if (!judgeHasChess(x,y))
	{
		//没有棋子，成功落子
		result = true;

		//棋盘记录当前落子
		ChessPiece* piece = ChessPiece::create();
		piece->setX(x);
		piece->setY(y);

		Vector<ChessPiece*>* chesses = (type == 1) ? prt_board->getWhites() : prt_board->getBlack();
		chesses->pushBack(piece);

	}

	//如果落子成功
	if (result)
	{
		//交换棋手
		prt_board->setType(3 - prt_board->getType());
		return true;
	}
	return false;
}

bool GameController::judgeWin(int type, int x, int y)
{
	//搜索区域范围，为当前点为中心的9*9的点阵范围
	int condition = WIN_CONDITION - 1;//向一个方向延伸四个点
	int leftEdge = x - condition;
	int rightEdge = x + condition;
	int topEdge = y + condition;
	int bottomEdge = y - condition;

	//边界优化处理，缩小搜索范围
	int length = prt_board->getWidth();
	if (leftEdge < 0) leftEdge = 0;
	if (rightEdge < length) rightEdge = length;
	if (topEdge < length) topEdge = length;
	if (bottomEdge < 0) bottomEdge = 0;

	//计算八个方向的连续棋子数
	const int counter = 8;
	Vec2 dirs[counter] = {
		Vec2(0,1),
		Vec2(1,1),
		Vec2(1,0),
		Vec2(1,-1),
		Vec2(0,-1),
		Vec2(-1,-1),
		Vec2(-1,0),
		Vec2(-1,1)
	};

	std::vector<int> lengths;
	for (int i = 0; i < counter; i++)
	{
		//记录一个连续长度（不包括当前落子点）
		int l = 0;
		Vec2 position = Vec2(x, y);
		while ((int)position.x >= leftEdge && (int)position.x <= rightEdge && (int)position.y >= bottomEdge && (int)position.y <= topEdge)
		{
			position += dirs[i];
			if (!judgeHasChess(type, (int)position.x, (int)position.y))
			{
				break;
			}
			l++;
		}
		//记录某方向上的连续长度（不包括当前落子点）
		lengths.push_back(l);
	}

	bool result = false;
	for (int i = 0; i < counter/2; i++)
	{
		//同一方向上的总长度（水平方向、竖直方向、正斜方向、反斜方向）
		int total = lengths[i] + lengths[i+4] + 1;
		if (total == WIN_CONDITION)
		{
			result = true;
		}
	}

	return result;
}

bool GameController::judgeHasChess(int x, int y)
{
	bool result = false;
	
	result = judgeHasChess(1, x, y);
	if (!result)
	{
		result = judgeHasChess(2, x, y);
	}
	
	return result;
}

bool GameController::judgeHasChess(int type, int x, int y)
{
	bool result = false;
	if (type == 1)
	{
		//从白棋中查找是否存在该点的棋子
		for (auto chess : *prt_board->getWhites())
		{
			if (chess->getX() == x && chess->getY() == y)
			{
				result = true;
				break;
			}
		}
	}
	
	else if (type == 2)
	{
		//从黑棋中查找是否存在该点的棋子
		for (auto chess : *prt_board->getBlack())
		{
			if (chess->getX() == x && chess->getY() == y)
			{
				result = true;
				break;
			}
		}
	}
	return result;
}
