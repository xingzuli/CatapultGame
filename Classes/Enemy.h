#ifndef __ENEMY__
#define __ENEMY__

#include "cocos2d.h"

USING_NS_CC;
class Enemy : public Sprite
{
public:
	virtual bool init(Vec2 EnemyPos);
	static Enemy* create(Vec2 EnemyPos);
	void createEnemy();
	void throwStone(Vec2 power);
	void EnemyUpdate(float t);
	CC_SYNTHESIZE(float, _dTime, DTime);

private:
	Sprite* _EnemyBody;
	Sprite* _stone;
	Vec2 _EnemyPos;
	float _difficult;

};
#endif //__ENEMY__
