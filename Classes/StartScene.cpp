#include "StartScene.h"

Scene* StartScene::createScene()
{
	Scene* scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

bool StartScene::init()
{
	if (!Layer::init()) {
		return false;
	}
	_visiblesize = Director::getInstance()->getVisibleSize();

	_music = true;

	// add the menu
	auto startgame = MenuItemLabel::create(Label::createWithTTF("StartGame", "fonts/arial.ttf", 50), CC_CALLBACK_0(StartScene::startGame, this));
	auto setmusic = MenuItemLabel::create(Label::createWithTTF("SetMusic", "fonts/arial.ttf", 50), CC_CALLBACK_0(StartScene::setMusic, this));
	auto gethelp = MenuItemLabel::create(Label::createWithTTF("Help", "fonts/arial.ttf", 50), CC_CALLBACK_0(StartScene::getHelp, this));
	auto menu = Menu::create(startgame, setmusic, gethelp, NULL);
	menu->setPosition(_visiblesize / 2);
	menu->alignItemsVertically();
	this->addChild(menu);

	return true;
}

void StartScene::startGame()
{
	this->stopAllActions();
	Director::getInstance()->replaceScene(GameScene::createScene());
}

void StartScene::setMusic()
{
	// something for setting the music
}

// retrun the help of the game
void StartScene::getHelp()
{
	// waiting
}
