#ifndef __GAME_OVER_SCENE__
#define __GAME_OVER_SCENE__
#include "cocos2d.h"
#include "GameScene.h"
USING_NS_CC;
class GameOverScene:public cocos2d::Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	CREATE_FUNC(GameOverScene);

private:
	Size _visibleSize;
	bool _gameStatus;
};
#endif