#include "Enemy.h"

Enemy* Enemy::create(Vec2 EnemyPos)
{
	Enemy* enemy = new Enemy();
	if (enemy && enemy->init(EnemyPos)) {
		enemy->autorelease();
		return enemy;
	}
	else {
		delete enemy;
		enemy = NULL;
		return NULL;
	}
}

bool Enemy::init(Vec2 EnemyPos)
{
	if (!Sprite::init()) {
		return false;
	}
	_EnemyPos = EnemyPos;
	createEnemy();
	return true;
}

void Enemy::createEnemy()
{
	_EnemyBody = Sprite::create("catapult.png");
	_EnemyBody->setPosition(_EnemyPos);
	this->addChild(_EnemyBody);
}

void Enemy::throwStone(Vec2 power)
{
	auto stone = Sprite::create("stone.png");
	auto body = PhysicsBody::createCircle(stone->getContentSize().width / 2);
	body->setContactTestBitmask(0xF);
	stone->setPhysicsBody(body);
	stone->setPosition(_EnemyPos);
	stone->setTag(2);
	this->addChild(stone);
	stone->getPhysicsBody()->setVelocity(power);

}

void Enemy::EnemyUpdate(float t)
{
	float time = getDTime() + t;
	if (time > 2) {
		float powerX = -100 + random(0, 100);
		float powerY = 100 + random(0, 100);
		log("pow x%f y %f", powerX, powerY);
		auto power = Vec2(powerX, powerY).getNormalized();
		throwStone(power*500);
		time -= 2;
	}
	setDTime(time);
}
