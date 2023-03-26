#include "GobangScene.h"
#include"GameController.h"
#include"SimpleAudioEngine.h"
using namespace CocosDenshion;


bool GobangScene::init()
{
	if (!Scene::create())
	{
		return false;
	}

	//屏幕中心点
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//创建一个单点事件监听器
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GobangScene::onTouchBegan,this);
	listener->onTouchEnded = CC_CALLBACK_2(GobangScene::onTouchEnded, this);;
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//白色背景层
	whitelayer = LayerColor::create(Color4B::WHITE);
	whitelayer->setPosition(origin);
	whitelayer->setContentSize(visibleSize);
	this->addChild(whitelayer);

	//棋盘精灵
	auto board = Sprite::create("board.png");
	board->setPosition(Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height/2));
	whitelayer->addChild(board,1);

	//阻挡触摸层
	//棋盘挡板
	barrierWall = LayerColor::create(Color4B(255,255,255,150));
	barrierWall->setPosition(Vec2::ZERO);
	barrierWall->setContentSize(board->getContentSize());
	board->addChild(barrierWall);
	//阻挡事件
	barrierListen = EventListenerTouchOneByOne::create();
	barrierListen->setSwallowTouches(false);
	barrierListen->onTouchBegan = [](Touch* touch,Event* ev) {return true; };
	barrierListen->onTouchEnded = [](Touch* touch, Event* ev) {};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(barrierListen, barrierWall);
	barrierWall->setVisible(false);

	//棋盘左侧中心
	Vec2 leftOrigin = origin + Vec2((visibleSize.width / 2-board->getContentSize().width/2)/2,visibleSize.height/2);


	//五子棋名称标签
	std::string gameName = ConfigController::getInstance()->getCNByID(1001);
	Label* gameNameLabel = Label::create(gameName, "arial", 36);
	gameNameLabel->setPosition(Vec2(leftOrigin.x+10, visibleSize.height - gameNameLabel->getContentSize().height));
	gameNameLabel->setColor(Color3B::BLACK);
	whitelayer->addChild(gameNameLabel, 2);

	//默认播放背景音乐
	prt_silence = false;
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm.mp3", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1f);

	//静音按钮
	auto btnSound = Button::create("openSound.png", "openSound.png", "openSound.png");
	btnSound->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	btnSound->setPosition(gameNameLabel->getPosition() - Vec2(gameNameLabel->getContentSize().width / 2, 0));
	btnSound->addClickEventListener([this](Ref* sender) {
		Button* button = dynamic_cast<Button*>(sender);
		//如果正在静音
		if (prt_silence)
		{
			button->loadTextures("openSound.png","openSound.png");

			prt_silence = false;
			SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		}
		//正在播放
		else
		{
			button->loadTextures("turnSound.png", "turnSound.png");

			prt_silence = true;
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		}
	});
	whitelayer->addChild(btnSound,2);


	//开始游戏按钮
	auto startItem = createGameButton(ConfigController::getInstance()->getCNByID(1003), CC_CALLBACK_1(GobangScene::menuStartCallback,this));
	float x, y;
	x = leftOrigin.x;
	y = leftOrigin.y;
	y += startItem->getContentSize().height * 1.5f;
	startItem->setPosition(Vec2(x,y));

	//悔棋按钮
	auto revokeItem = createGameButton(ConfigController::getInstance()->getCNByID(1004), CC_CALLBACK_1(GobangScene::menuRevokeCallback, this));
	y = leftOrigin.y;
	revokeItem->setPosition(Vec2(x, y));

	//认输按钮
	auto defeatItem = createGameButton(ConfigController::getInstance()->getCNByID(1005), CC_CALLBACK_1(GobangScene::menuDefeatCallback, this));
	y = leftOrigin.y - defeatItem->getContentSize().height * 1.5f;
	defeatItem->setPosition(Vec2(x, y));

	//退出游戏按钮
	auto closeItem = createGameButton(ConfigController::getInstance()->getCNByID(1006), CC_CALLBACK_1(GobangScene::menuCloseCallback, this));
	x = leftOrigin.x+visibleSize.width/2+ board->getContentSize().width/2;
	y = leftOrigin.y - defeatItem->getContentSize().height * 1.5f;
	closeItem->setPosition(Vec2(x, y));

	//创建界面菜单
	auto menu = Menu::create(startItem, revokeItem, defeatItem, closeItem, nullptr);
	menu->setPosition(origin);
	whitelayer->addChild(menu,3);

	//棋盘所在区域
	/*Size boardSize = Size(padding, padding);
	int width = GameController::getInstance()->getChessBoard()->getWidth();
	Vec2 _origin = Vec2(visibleSize.width / 2 - width / 2*boardSize.width,visibleSize.height/2-width/2*boardSize.height);
	_origin = _origin - Vec2(boardSize.width / 2, boardSize.height / 2);
	prt_boarSize = Rect(_origin.x, _origin.y, width * padding, width * padding);*/
	Size boardSize = Size(padding, padding);//触点范围
	int width = GameController::getInstance()->getChessBoard()->getWidth();//棋盘横向格子数量
	Vec2 _origin = Vec2(visibleSize.width / 2 - width / 2 * boardSize.width - (248.f-242.4), visibleSize.height / 2 - width / 2 * boardSize.height+(246.5f-241.8f));//棋盘零点
	_origin = _origin - Vec2(boardSize.width / 2, boardSize.height / 2);//触摸起始点
	prt_boarSize = Rect(_origin.x, _origin.y, width * padding, width * padding);//触摸矩形


	//按钮挡板
	buttonWall_1 = LayerColor::create(Color4B(255, 255, 255, 150));
	buttonWall_1->setPosition(Vec2::ZERO);
	buttonWall_1->setContentSize(revokeItem->getContentSize());
	revokeItem->addChild(buttonWall_1);
	barrierListen1 = barrierListen->clone();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(barrierListen1, buttonWall_1);
	buttonWall_1->setVisible(false);

	buttonWall_2 = LayerColor::create(Color4B(255, 255, 255, 150));
	buttonWall_2->setPosition(Vec2::ZERO);
	buttonWall_2->setContentSize(defeatItem->getContentSize());
	defeatItem->addChild(buttonWall_2);
	barrierListen2 = barrierListen->clone();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(barrierListen2, buttonWall_2);
	buttonWall_2->setVisible(false);

	//是否处于结算时刻
	isAccomplish = false;

	//创建标识精灵
	flagSprite = Sprite::create("arrowhead.png");
	flagSprite->setVisible(false);
	flagSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	whitelayer->addChild(flagSprite, 1000);

	return true;
}

//封装按钮
MenuItemImage* GobangScene::createGameButton(std::string name, const ccMenuCallback& callback)
{
	//图片菜单项
	auto item = MenuItemImage::create("button.png", "button.png", callback);

	//文字标签
	auto label = Label::create(name, "arial", 36);
	label->setPosition(item->getContentSize() / 2);
	label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	label->setColor(Color3B::WHITE);
	item->addChild(label);

	return item;
}

void GobangScene::menuStartCallback(Ref* sender)
{
	CCLOG("menuStartCallback");
	//CCLOG("游戏开始按钮点击事件");

	//重置棋盘数据
	GameController::destroyInstance();

	//重置棋盘
	for (auto node : prt_allChesses)
	{
		node->removeFromParent();
	}
	int type = GameController::getInstance()->getChessBoard()->getType();
	auto chessBoard = GameController::getInstance()->getChessBoard();

	//隐藏标识
	flagSprite->setVisible(false);

	//隐藏阻挡层
	barrierWall->setVisible(false);
	barrierListen->setSwallowTouches(false);
	buttonWall_1->setVisible(false);
	barrierListen1->setSwallowTouches(false);
	buttonWall_2->setVisible(false);
	barrierListen2->setSwallowTouches(false);

	//重新开始构建棋盘逻辑控制器
	GameController::getInstance();

	//是否处于结算时刻
	isAccomplish = false;
}

void GobangScene::menuRevokeCallback(Ref* sender)
{
	CCLOG("menuRevokeCallback");
	//CCLOG("悔棋按钮点击事件");

	//是否处于结算时刻
	if (isAccomplish) return;

	//判断是否为空
	if (prt_allChesses.empty())return;

	//悔棋
	//判断当前棋手是谁
	int type = GameController::getInstance()->getChessBoard()->getType();
	auto chessBoard = GameController::getInstance()->getChessBoard();
	Vector<ChessPiece*>* chesses = 
		(type == 2) ? chessBoard->getWhites() : chessBoard->getBlack();

	//移除最后一颗棋子
	if (chesses->empty())return;
	chesses->popBack();
	prt_allChesses.back()->removeFromParent();
	prt_allChesses.popBack();

	//更改标识到前一颗棋子上
	//判断是否为空
	if (prt_allChesses.empty())
	{
		//先手必为黑子
		chessBoard->setType(2);
		return;
	}
	auto chessNode = prt_allChesses.back();
	flagSprite->setPosition(chessNode->getPosition() + Vec2(0, chessNode->getContentSize().height / 2));

	//交换棋手
	chessBoard->setType(3 - type);
}

void GobangScene::menuDefeatCallback(Ref* sender)
{
	CCLOG("menuDefeatCallback");
	//CCLOG("认输按钮点击事件");

	//是否处于结算时刻
	if (isAccomplish) return;

	//判断当前棋手是谁
	int type = GameController::getInstance()->getChessBoard()->getType();
	if (type == 1)
	{
		//白棋投降
		//播放胜利动画
		playSuccessAnimation(3-type);
	}
	else
	{
		//黑棋投降
		//播放胜利动画
		playSuccessAnimation(3-type);
	}

	//出现挡板层
	barrierWall->setVisible(true);
	barrierListen->setSwallowTouches(true);
	buttonWall_1->setVisible(true);
	barrierListen1->setSwallowTouches(false);
	buttonWall_2->setVisible(true);
	barrierListen2->setSwallowTouches(false);
}

void GobangScene::menuCloseCallback(Ref* sender)
{
	CCLOG("menuCloseCallback");
	//CCLOG("退出按钮点击事件");
	Director::getInstance()->end();
}

bool GobangScene::onTouchBegan(Touch* touch, Event* ev)
{
	return true;
}

void GobangScene::onTouchEnded(Touch* touch, Event* ev)
{
	//当前触摸的坐标点
	Vec2 position = touch->getLocation();

	//判断坐标点是否在棋盘中
	if (prt_boarSize.containsPoint(position))
	{
		Vec2 relative = position - prt_boarSize.origin;
		float val = relative.x / padding;
		int row = int(val);
		if (val > row) row++;
		val = relative.y / padding;
		int cols = int(val);
		if (val > cols) cols++;

		//落子
		int type = GameController::getInstance()->getChessBoard()->getType();
		bool result = GameController::getInstance()->playChess(type,row,cols);

		//落子成功
		if (result)
		{
			//显示一个棋子精灵
			createChessNode(type, row, cols);

			//落子声音
			if (!prt_silence)
			{
				SimpleAudioEngine::getInstance()->playEffect("PlacingBoard.wav");
			}

			//判断是否胜利
			if (GameController::getInstance()->judgeWin(type, row, cols))
			{
				//播放胜利动画
				playSuccessAnimation(type);

				//出现挡板层
				barrierWall->setVisible(true);
				barrierListen->setSwallowTouches(true);
				buttonWall_1->setVisible(true);
				barrierListen1->setSwallowTouches(false);
				buttonWall_2->setVisible(true);
				barrierListen2->setSwallowTouches(false);
			}
		}
	}
}

void GobangScene::createChessNode(int type, int x, int y)
{
	//点阵位置从（0,0）开始
	x--;
	y--;

	Sprite* chessNode = Sprite::create(String::createWithFormat("Piece%d.png", 3 - type)->getCString());
	chessNode->setPosition(prt_boarSize.origin + Vec2(padding * (x + 0.5), padding * (y + 0.5)));
	whitelayer->addChild(chessNode, 4, x * 100 + y);
	//记录所有落子
	prt_allChesses.pushBack(chessNode);

	//标识当前棋子
	if (!flagSprite->isVisible())
	{
		flagSprite->setVisible(true);
	}
	flagSprite->setPosition(chessNode->getPosition() + Vec2(0, chessNode->getContentSize().height/2));
	flagSprite->stopAllActions();
	flagSprite->runAction(RepeatForever::create(Sequence::create(
		MoveBy::create(.5f, Vec2(0, -10)),
		MoveBy::create(.5f, Vec2(0, 10)),
		nullptr)));
}

void GobangScene::playSuccessAnimation(int type)
{
	int tipsId = (type == 2) ? 1010 : 1011;
	std::string words = ConfigController::getInstance()->getCNByID(tipsId);
	//创建文字标签
	Label* label = Label::create(words, "arial", 50);
	label->setPosition(Vec2(Director::getInstance()->getVisibleSize() / 2));
	whitelayer->addChild(label, 1002);
	label->setColor(Color3B::RED);

	//动画
	label->setScale(0.f);
	auto action_show = ScaleTo::create(0.2f, 1.2f);
	auto action_back = ScaleTo::create(0.2f, 1.f);
	auto action_fadeout = FadeOut::create(.2f);

	label->runAction(Sequence::create(
		action_show,
		action_back,
		DelayTime::create(2.5f),
		action_fadeout,
		nullptr));

	//是否处于结算时刻
	isAccomplish = true;
}
