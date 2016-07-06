#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	void addNewSprite(Point p);
	void rotateArrow(Point touchpoint);
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	void throwStone(Sprite* stone);
	void movesFinished(Object* pSender);
	void update(float t){ timeCount += t; }

private:
	cocos2d::TMXTiledMap* _tileMap;
	cocos2d::Sprite* _player;
	cocos2d::Sprite* _npc;
	cocos2d::Sprite* _arrow;
	Point preTouchPoint;
	Point currTouchPoint;
	int throwTime;
	float timeCount;
	float touchTime;
	bool runAction;
};

#endif // __HELLOWORLD_SCENE_H__
