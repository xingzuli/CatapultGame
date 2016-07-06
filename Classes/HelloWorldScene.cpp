#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{

    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	preTouchPoint = Point(0, 0);
	runAction = false;
	timeCount = 0;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	log("visibleSize width:%f height:%f",visibleSize.width, visibleSize.height);
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto label = Label::createWithTTF("Catapult Game", "fonts/Marker Felt.ttf", 24);
    
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    this->addChild(label, 1);
    
	// add the tilemap
	_tileMap = TMXTiledMap::create("TileMap.tmx");
	log("tileMap width %f, height %f", _tileMap->getContentSize().width, _tileMap->getContentSize().height);
	addChild(_tileMap, -1);

	// add the player
	TMXObjectGroup *objects = _tileMap->getObjectGroup("Objects");
	auto playerPoint = objects->getObject("player");
	CCASSERT(!playerPoint.empty(), "player not found");
	int x = playerPoint["x"].asInt();
	int y = playerPoint["y"].asInt();
	_player = Sprite::create("catapult2.png");
	_player->setPosition(x, y);
	addChild(_player);

	// add the npc
	auto npcPoint = objects->getObject("NPC");
	CCASSERT(!npcPoint.empty(), "NPC not found");
	x = npcPoint["x"].asInt();
	y = npcPoint["y"].asInt();
	_npc = Sprite::create("catapult2.png");
	_npc->setPosition(x, y);
	addChild(_npc);

	// add the arrow
	_arrow = Sprite::create("arrow.png");
	_arrow->setPosition(_player->getPosition());
	_arrow->setAnchorPoint(Vec2(0, 0.5));
	addChild(_arrow);

	//addNewSprite(Point(300, 300));
	// add listener
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	schedule(schedule_selector(HelloWorld::update));

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::addNewSprite(Point p)
{
	auto sprite = Sprite::create("Projectile.png");
	//auto body = PhysicsBody::createCircle(sprite->getContentSize().width / 2);
	//sprite->setPhysicsBody(body);
	sprite->setPosition(p);
	addChild(sprite);
}

void HelloWorld::rotateArrow(Point touchpoint)
{
	auto vector = touchpoint - _player->getPosition();
	auto rotateRadians = vector.getAngle();
	// because cocos rotate by clockwise,
	// it's the negative value of the angle in the math
	auto rotateDegrees = CC_RADIANS_TO_DEGREES(-1 * rotateRadians);
	// anothoer way to get angle
	/*auto rotateRadians = atanf((float)vector.y / (float)vector.x);
	auto rotateDegrees = CC_RADIANS_TO_DEGREES(rotateRadians);
	log("origin degrees %f", rotateDegrees);
	rotateDegrees = -1 * rotateDegrees;
	if (vector.x < 0) {
		rotateDegrees += 180;
	}*/

	auto speed = 0.5 / M_PI;
	auto Duration = fabs(speed*rotateRadians);
	_arrow->runAction(RotateTo::create(Duration, rotateDegrees));
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* unused_event)
{
	if (runAction) return false;
	touchTime = timeCount;
	currTouchPoint = touch->getLocation();
	//currTouchPoint = this->convertToNodeSpace(currTouchPoint);
	if (currTouchPoint != preTouchPoint) {
		rotateArrow(currTouchPoint);
	}
	if (!runAction)
		preTouchPoint = currTouchPoint;
	return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{

	currTouchPoint = touch->getLocation();
	//currTouchPoint = this->convertToNodeSpace(currTouchPoint);
	if (currTouchPoint != preTouchPoint) {
		rotateArrow(currTouchPoint);
	}
	preTouchPoint = currTouchPoint;
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
	// 在began处touchtime存储了开始时间，现在获得实际touchTime
	touchTime = timeCount - touchTime;
	runAction = true;
	log("TouchPoint %f %f\n", touch->getLocation().x, touch->getLocation().y);
	auto stone = Sprite::create("stone.png");
	stone->setPosition(_player->getPosition());
	this->addChild(stone);
	if (runAction){
		throwTime = 0;
		throwStone(stone);
	}
}

void HelloWorld::throwStone(Sprite * stone)
{
	auto vector = preTouchPoint - _player->getPosition();
	vector.normalize();
	throwTime++;
	log("touchTime %f", touchTime);
	if (touchTime > 1) touchTime = 1;
	int power = 80 * touchTime;
	float v0 = power*vector.x;
	float v1 = power*vector.y;
	float diffX = v0;
	float diffY = v1 - 6*throwTime;
	log("vo %f, v1 %f", v0, v1);
	log("delta %f %f\nthrowTime %d", diffX, diffY, throwTime);
	auto movestone = MoveBy::create(0.1, Point(diffX, diffY));
	auto movedone = CallFuncN::create(CC_CALLBACK_1(HelloWorld::movesFinished, this));
	stone->runAction(Sequence::create(movestone, movedone, NULL));
}

void HelloWorld::movesFinished(Object* pSender)
{
	Sprite* sprite = (Sprite*)pSender;
	if (sprite->getPositionX() > Director::getInstance()->getVisibleSize().width + sprite->getContentSize().width
		|| sprite->getPositionX()<-sprite->getContentSize().width
		|| sprite->getPositionY()<-sprite->getContentSize().height){
		runAction = false;
		sprite->removeFromParent();
		return;
	}
	this->throwStone(sprite);
}