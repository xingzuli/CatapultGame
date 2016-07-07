#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
	virtual bool onContactBegan(PhysicsContact& contact);
	void addScoresBar();

private:
	Vec2 _playerPos;
	Player * _player;
	TMXTiledMap* _tileMap;
	TMXLayer* _obstacles;
	int _playerItems;
	int _playerItemsTotal;
	int _enemyItems;
	Size _winSize;
	ProgressTimer* _playerScores;
	ProgressTimer* _NPCScores;
};

#endif // __GAME_SCENE_H__
