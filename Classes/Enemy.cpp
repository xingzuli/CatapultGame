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
	_EnemyBody = Sprite::create("catapult");
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
	this->addChild(stone);
	stone->getPhysicsBody()->setVelocity(power);

}

void Enemy::EnemyUpdate(float t)
{
	float time = getDTime() + t;
	setDTime(time);
	if (time > 1) {
		throwStone(Vec2(200,200));
	}
}
