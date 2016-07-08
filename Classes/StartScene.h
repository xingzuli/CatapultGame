#ifndef __START_SCENE__
#define __START_SCENE__

#include "cocos2d.h"
#include "GameScene.h"
USING_NS_CC;
class StartScene: public cocos2d::Layer
{
public:
	static Scene* createScene();
	virtual bool init();
	void startGame();
	void setMusic();
	void getHelp();
	CREATE_FUNC(StartScene);
	static bool getMusicStatus();
private:
	Size _visiblesize;
	bool _music;
};
#endif// __START_SCENE__