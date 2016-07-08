#include "GameOverScene.h"

Scene* GameOverScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameOverScene::init()
{
	if (!Layer::init()) {
		return false;
	}
	_visibleSize = Director::getInstance()->getVisibleSize();

	auto label = Label::create(GameScene::_gameStatus, "fonts/arial.ttf", 50);
	label->setPosition(_visibleSize / 2);
	addChild(label);

	
	return true;
}
