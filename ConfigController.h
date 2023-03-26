#pragma once
/*
	功能：配置文件控制器，实现读取和换件配置文件数据
	时间：2023-3-17 15:44:49
*/

#include<string>
#include<map>
#include"cocos2d.h"
USING_NS_CC;

class ConfigController
{
public:
	static ConfigController* getInstance();
	static void destroyInstance();
	ConfigController();
	~ConfigController();

	//通过编号获取指定文字
	std::string getCNByID(int id);

private:
	static ConfigController* instance;

	//记录文字表(缓存数据)
	std::map<int, Value> prt_cnWords;
};

