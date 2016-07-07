#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* GameScene::createScene()
{

	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -300));
    auto layer = GameScene::create();
    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
	_winSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	_playerItems = 0;
	_playerItemsTotal = 0;
	_enemyItems = 0;
	_enemyItemsTotal = 0;

	log("visibleSize width:%f height:%f",visibleSize.width, visibleSize.height);
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));
    
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

	// add the border
	auto body = PhysicsBody::createEdgeBox(visibleSize);
	auto edgeNode = Node::create();
	edgeNode->setPosition(visibleSize / 2);
	edgeNode->setPhysicsBody(body);
	this->addChild(edgeNode);

	// add the bg
	_tileMap = TMXTiledMap::create("TileMap.tmx");
	_tileMap->setAnchorPoint(Vec2(0.5, 0.5));
	_tileMap->setPosition(visibleSize / 2);
	this->addChild(_tileMap);

	// add the player
	TMXObjectGroup * objects = _tileMap->getObjectGroup("Objects");
	auto playPoint = objects->getObject("player");
	int x = playPoint["x"].asInt();
	int y = playPoint["y"].asInt();
	_playerPos = Vec2(x, y);
	_player = Player::create(_playerPos);
	addChild(_player);
	// add the enemy
	auto enemyPoint = objects->getObject("enemy");
	x = enemyPoint["x"].asInt();
	y = enemyPoint["y"].asInt();
	_enemy = Enemy::create(Vec2(x, y));
	addChild(_enemy);

	// add the obstacles
	_obstacles = _tileMap->getLayer("Obstacles");

	// obstacle层是暗红色的，游戏中可以关闭
	_obstacles->setVisible(false);

	log("layerSize %f %f", _obstacles->getLayerSize().width, _obstacles->getLayerSize().height);
	// getTile返回的是一个精灵，GID是图块代号，如果该点没有图块，GID为0
	// 此处应该注意body不能放到循环外，因为body附加给一个精灵后就不能再附加给其他精灵，所以要不断创建
	for (int i = 0; i < _obstacles->getLayerSize().width; ++i) {
		for (int j = 0; j < _obstacles->getLayerSize().height; ++j) {
			auto tileGid = _obstacles->getTileGIDAt(Vec2(i, j));
			if (tileGid) {
				log("x%d, y%d, gid%d", i, j, tileGid);
				auto obstacleTile = _obstacles->getTileAt(Vec2(i, j));
				auto obstacleBody = PhysicsBody::createEdgeBox(_tileMap->getTileSize());
				obstacleBody->setDynamic(false);
				obstacleTile->setPhysicsBody(obstacleBody);
			}
		}
	}

	// add the items
	TMXObjectGroup* items = _tileMap->getObjectGroup("Items");
	for (auto& item : items->getObjects()) {
		auto dict = item.asValueMap();
		x = dict["x"].asInt();
		y = dict["y"].asInt();
		if (dict["class"].asInt() == 1) {
			_playerItems++;
			auto itemsp = Sprite::create("item1.png");
			itemsp->setTag(3);
			auto body = PhysicsBody::createBox(itemsp->getContentSize());
			body->setGravityEnable(false);
			body->setContactTestBitmask(0xFFFFFFFF);
			itemsp->setPhysicsBody(body);
			itemsp->setPosition(Vec2(x, y));
			this->addChild(itemsp);
		}
		if (dict["class"].asInt() == 2) {
			_enemyItems++;
			auto itemsp = Sprite::create("item2.png");
			itemsp->setTag(4);
			auto body = PhysicsBody::createBox(itemsp->getContentSize());
			body->setGravityEnable(false);
			body->setContactTestBitmask(0xFFFFFFFF);
			itemsp->setPhysicsBody(body);
			itemsp->setPosition(Vec2(x, y));
			this->addChild(itemsp);
		}
		
	}
	_playerItemsTotal = _playerItems;
	_enemyItemsTotal = _enemyItems;
	log("playerItemTotal %d enemyItemsTotal %d", _playerItemsTotal, _enemyItemsTotal);

	// add the process bar
	addScoresBar();

	// add the update of enemy
	_enemy->setDTime(0);
	_enemy->schedule(schedule_selector(Enemy::EnemyUpdate));

	// add the touch listener
	auto touchListener= EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// add the contact listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegan, this);
	this->_eventDispatcher->addEventListenerWithFixedPriority(contactListener, 1);
    return true;
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
	auto touchPos = touch->getLocation();
	_player->createArrow();
	_player->rotateArrow(touchPos);
	_player->setTouchTime(0);
	_player->schedule(schedule_selector(Player::playerUpdate));
	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	auto touchPos = touch->getLocation();
	_player->rotateArrow(touchPos);
}

void GameScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto touchPos = touch->getLocation();
	auto vector = touchPos - _playerPos;
	vector.normalize();

	_player->throwStone(vector*_player->getTouchTime()*500);
	_player->releaseArrow();

	_player->unschedule(schedule_selector(Player::playerUpdate));
}

bool GameScene::onContactBegan(PhysicsContact& contact)
{
	log("contact happened");
	auto sp1 = contact.getShapeA()->getBody()->getNode();
	auto sp2 = contact.getShapeB()->getBody()->getNode();
	if ((sp1->getTag() == 1 && sp2->getTag() == 3 )||(sp2->getTag()==1&&sp1->getTag()==3)) {
		log("player shoot the item");
		sp1->removeFromParent();
		sp2->removeFromParent();
		_playerItems--;
		auto dd = ProgressTo::create(1, 100-(float)_playerItems / _playerItemsTotal * 100);
		_playerScores->runAction(dd);
	}
	else if ((sp1->getTag() == 2 && sp2->getTag() == 4) || (sp2->getTag() == 2 && sp1->getTag() == 4)) {
		log("enemy shoot the item");
		sp1->removeFromParent();
		sp2->removeFromParent();
		_enemyItems--;
		auto dd = ProgressTo::create(1, 100-(float)_enemyItems / _enemyItemsTotal * 100);
		_enemyScores->runAction(dd);
		log("enemyItems left %d", _enemyItems);
	}
	return true;
}

void GameScene::addScoresBar()
{
	_playerScores = ProgressTimer::create(Sprite::create("Bar.png"));
	_playerScores->setType(ProgressTimer::Type::BAR);
	_playerScores->setAnchorPoint(Vec2(0, 0.5));
	_playerScores->setMidpoint(Vec2(0, 0));
	_playerScores->setBarChangeRate(Vec2(1, 0));
	_playerScores->setPosition(Vec2(_winSize.width / 10, _winSize.height / 8 * 7));
	_playerScores->setPercentage(100-(float)_playerItems/_playerItemsTotal*100);
	addChild(_playerScores);

	_enemyScores = ProgressTimer::create(Sprite::create("Bar.png"));
	_enemyScores->setType(ProgressTimer::Type::BAR);
	_enemyScores->setAnchorPoint(Vec2(0, 0.5));
	_enemyScores->setMidpoint(Vec2(1, 0));
	_enemyScores->setBarChangeRate(Vec2(1, 0));
	_enemyScores->setPosition(Vec2(_winSize.width / 10 * 7, _winSize.height / 8 * 7));
	_enemyScores->setPercentage(100 - (float)_enemyItems / _enemyItemsTotal * 100);
	addChild(_enemyScores);
}

