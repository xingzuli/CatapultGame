#include "Player.h"

Player::Player() {

}

Player* Player::create(Vec2 PlayerPos)
{
	Player* p = new Player();
	if (p && p->init(PlayerPos)) {
		p->autorelease();
		return p;
	}
	else {
		delete p;
		p = NULL;
		return  NULL;
	}
}


bool Player::init(Vec2 PlayerPos)
{
	if (!Sprite::init()) {
		return false;
	}
	this->_playerPos = PlayerPos;
	createPlayer();
	return true;
}

void Player::createPlayer()
{
	_PlayerBody = Sprite::create("catapult.png");
	_PlayerBody->setPosition(_playerPos);
	this->addChild(_PlayerBody);
}

void Player::throwStone(Vec2 power)
{
	auto stone = Sprite::create("stone.png");
	auto body = PhysicsBody::createCircle(stone->getContentSize().width / 2);
	body->setContactTestBitmask(0xF);
	stone->setPhysicsBody(body);
	stone->setPosition(_playerPos);
	this->addChild(stone);
	stone->getPhysicsBody()->setVelocity(power);
}

void Player::rotateArrow(Point pos)
{
	auto vector = pos - _playerPos;
	auto rotateRadians = vector.getAngle();
	auto rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians);
	auto speed = 0.5 / M_PI;
	auto duration = fabs(speed*rotateRadians);
	auto seq = Sequence::create(RotateTo::create(duration, rotateDegrees), CallFuncN::create([&](Node* node){node->setVisible(true); }), NULL);
	_arrow->runAction(seq);
	_powerbar->runAction(seq->clone());
}

void Player::releaseArrow()
{
	_arrow->removeFromParent();
	_powerbar->removeFromParent();
}

void Player::createArrow()
{
	_arrow = Sprite::create("arrow2.png");
	_arrow->setPosition(_playerPos);
	_arrow->setAnchorPoint(Vec2(0, 0.5f));
	_arrow->setVisible(false);
	this->addChild(_arrow);

	_powerbar = ProgressTimer::create(Sprite::create("arrow.png"));
	_powerbar->setType(ProgressTimer::Type::BAR);
	_powerbar->setMidpoint(Vec2(0, 0));
	_powerbar->setBarChangeRate(Vec2(1, 0));
	_powerbar->setAnchorPoint(Vec2(0, 0.5f));
	_powerbar->setPosition(_playerPos);
	_powerbar->setPercentage(0);
	_powerbar->setVisible(false);
	this->addChild(_powerbar);
}

void Player::playerUpdate(float t)
{
	auto time = getTouchTime() + t;
	if (time > 1) {
		time = 1;
	}
	_powerbar->setPercentage(time*100);
	setTouchTime(time);
}

