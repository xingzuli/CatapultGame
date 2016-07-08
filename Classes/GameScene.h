#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Player.h"
#include "Enemy.h"
#include "GameOverScene.h"

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
	void isFinished();
	static std::string _gameStatus;

private:
	Size _winSize;	
	TMXTiledMap* _tileMap;
	TMXLayer* _obstacles;
	Player * _player;
	Enemy * _enemy;
	Vec2 _playerPos;
	int _playerItems;
	int _playerItemsTotal;
	int _enemyItems;
	int _enemyItemsTotal;
	ProgressTimer* _playerScores;
	ProgressTimer* _enemyScores;
};

#endif // __GAME_SCENE_H__
