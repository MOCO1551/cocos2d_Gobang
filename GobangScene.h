#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "ui/CocosGUI.h"
using namespace ui;

#include"ConfigController.h"

/*
	功能：五子棋显示场景
	时间：2023-3-17 15:37:17
*/

class GobangScene :public Scene
{
public:
	CREATE_FUNC(GobangScene);
	virtual bool init();

	/*封装按钮
		参数：按钮名称
			  回调函数
	*/
	static MenuItemImage* createGameButton(std::string name, const ccMenuCallback& callback);
	
private:
	//记录背景层
	Node* whitelayer;
	//记录棋盘大小位置
	Rect prt_boarSize;
	//记录格子的宽度
	const float padding = 23.6;

	//棋子标识
	Sprite* flagSprite;

	//记录背景音乐静音状态
	bool prt_silence;

	//记录所有落子显示节点
	Vector<Node*> prt_allChesses;

	//游戏开始按钮点击事件
	void menuStartCallback(Ref* sender);
	//悔棋按钮点击事件
	void menuRevokeCallback(Ref* sender);
	//认输按钮点击事件
	void menuDefeatCallback(Ref* sender);
	//退出按钮点击事件
	void menuCloseCallback(Ref* sender);

	//阻挡触摸层
	Layer* barrierWall;
	Layer* buttonWall_1;
	Layer* buttonWall_2;
	//阻挡层触摸事件
	EventListenerTouchOneByOne* barrierListen;
	EventListenerTouchOneByOne* barrierListen1;
	EventListenerTouchOneByOne* barrierListen2;
	//是否处于结算时刻
	bool isAccomplish;

	//按下回调
	virtual bool onTouchBegan(Touch* touch, Event* ev);
	//释放回调
	virtual void onTouchEnded(Touch* touch, Event* ev);

	//创建一个棋子节点
	void createChessNode(int type, int x, int y);

	//播放胜利动画
	void playSuccessAnimation(int type);

};

