#ifndef __Player__
#define __Player__

#include "cocos2d.h"

USING_NS_CC;

class Player : public Sprite
{
public:
	Player();
	virtual bool init(Vec2 PlayerPos);
	static Player* create(Vec2 PlayerPos);
	void createPlayer();
	void throwStone(Sprite* stone, Vec2 power);
	void createArrow();
	void rotateArrow(Point pos);
	void releaseArrow();
	void playerUpdate(float t);
	CC_SYNTHESIZE(float, _touchTime, TouchTime);

private:
	Sprite* _PlayerBody;
	Sprite* _stone;
	Sprite* _arrow;
	ProgressTimer* _powerbar;
	Vec2 _playerPos;
	Vec2 _power;

};

#endif // __Player__
