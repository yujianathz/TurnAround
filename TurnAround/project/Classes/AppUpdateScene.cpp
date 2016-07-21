#include "AppUpdateScene.h"
#include "AppDelegate.h"

#include "GBUtils.h"

bool AppUpdateScene::init()
{
	if (!Scene::init())
		return false;
	setContentSize(DIRECTOR()->getWinSize());

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("loading.plist");

	auto listener = DIRECTOR()->getEventDispatcher()->addCustomEventListener(GBUPDATE_EVENT_UPDATE_START, [this](EventCustom * pEvent){
		onUpdateStart();
	});
	_eventListeners.pushBack(listener);

	listener = DIRECTOR()->getEventDispatcher()->addCustomEventListener(GBUPDATE_EVENT_UPDATE_FINISH, [this](EventCustom *){
		onUpdateFinish();
	});
	_eventListeners.pushBack(listener);

	listener = DIRECTOR()->getEventDispatcher()->addCustomEventListener(GBUPDATE_EVENT_UPDATED_ONE, [this](EventCustom * pEvent){
		onUpdatedOne((GBUpdateInfo *)pEvent->getUserData());
	});
	_eventListeners.pushBack(listener);

	listener = DIRECTOR()->getEventDispatcher()->addCustomEventListener(GBUPDATE_EVENT_UPDATE_READY, [this](EventCustom * pEvent){
		onUpdateReady();
	});
	_eventListeners.pushBack(listener);

	_pBg = Sprite::create("loading_bg.jpg");
	GBUtils::addChild2Center(this, _pBg);

	_pProgressBg = Scale9Sprite::createWithSpriteFrameName("loading_progress_bg", Rect(40, 0, 4, 54));
	_pProgressBg->setContentSize(Size(W(this) - 80, 54));
	GBUtils::addChild2Pos(this, _pProgressBg, Vec2(W(this) / 2, H(this) / 2 + 60));

	_pProgressFg = LoadingBar::create();
	_pProgressFg->setScale9Enabled(true);
	_pProgressFg->setCapInsets(Rect(40, 0, 4, 54));
	_pProgressFg->setContentSize(_pProgressBg->getContentSize());
	_pProgressFg->loadTexture("loading_progress_fg", TextureResType::PLIST);
	_pProgressFg->setPercent(0);
	GBUtils::addChild2Center(_pProgressBg, _pProgressFg);

	_pLabelPercentage = Label::createWithTTF("", "res/font.ttf", 36);	
	_pLabelPercentage->enableOutline(Color4B::BLACK, 2);
	GBUtils::addChild2Center(_pProgressBg, _pLabelPercentage);

	_pLabelSize = Label::createWithTTF("", "res/font.ttf", 36);
	_pLabelSize->enableOutline(Color4B::BLACK, 2);
	_pLabelSize->setAnchorPoint(Vec2(0.5f, 1.0f));
	GBUtils::addChild2Pos(this, _pLabelSize, Vec2(X(_pProgressBg), BOTTOM(_pProgressBg) - 10));

	_pLabelVersion = Label::createWithTTF("", "res/font.ttf", 24);
	_pLabelVersion->enableOutline(Color4B::BLACK, 1);
	_pLabelVersion->setAnchorPoint(Vec2(1.0f, 0));
	GBUtils::addChild2Pos(this, _pLabelVersion, Vec2(W(this) - 10, 10));

	_pBtnSkip = GBShaderButton::create("loading_btn_skip", "res/shader/highlight.fsh");
	_pBtnSkip->setVisible(false);
	_pBtnSkip->addClickEventListener([this](Ref * pSender){
		_pBtnSkip->setVisible(false);
		_pBtnUpdate->setVisible(false);
		DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBUPDATE_EVENT_UPDATE_FINISH);
	});
	GBUtils::addChild2Pos(this, _pBtnSkip, Vec2(X(_pProgressBg) - W(_pBtnSkip) / 2 - 40, BOTTOM(_pProgressBg) - H(_pBtnSkip) / 2 - 60));

	_pBtnUpdate = GBShaderButton::create("loading_btn_update", "res/shader/highlight.fsh");
	_pBtnUpdate->setVisible(false);
	_pBtnUpdate->addClickEventListener([this](Ref * pSender){
		_pBtnSkip->setVisible(false);
		_pBtnUpdate->setVisible(false);
		DIRECTOR()->getEventDispatcher()->dispatchCustomEvent(GBUPDATE_EVENT_UPDATE_START);
	});
	GBUtils::addChild2Pos(this, _pBtnUpdate, Vec2(X(_pProgressBg) + W(_pBtnUpdate) / 2 + 40, Y(_pBtnSkip)));

	ValueMap dict = FileUtils::getInstance()->getValueMapFromFile("config.plist");
	_updateSize = dict["UPDATE_SIZE"].asString();
	_updateUrl = dict["UPDATE_URL"].asString();
	_updateFolder = dict["UPDATE_FOLDER"].asString();

	return true;
}

void AppUpdateScene::onEnter()
{
	Scene::onEnter();

	string url = StringUtils::format("http://%s/update/%s", _updateUrl.c_str(), _updateFolder.c_str());
	GBUpdate::getInstance()->setIsUpdateEnabled(true, url);
}

void AppUpdateScene::onExit()
{
	Scene::onExit();
}

void AppUpdateScene::cleanup()
{
	for (ssize_t i = 0; i < _eventListeners.size(); i++)
		DIRECTOR()->getEventDispatcher()->removeEventListener(_eventListeners.at(i));
	_eventListeners.clear();

	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("loading.plist");
	DIRECTOR()->getTextureCache()->removeTexture(_pBg->getTexture());
	
	Scene::cleanup();
}

void AppUpdateScene::onUpdateStart()
{
	_pLabelVersion->setString(StringUtils::format("ver:%s", GBUpdate::getInstance()->getRemoteVersion().c_str()));

	updateLabels();
}

void AppUpdateScene::onUpdateFinish()
{
	_pProgressFg->setPercent(100);

	runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create([](){
		AppDelegate::executeScript(true);
	})));
}

void AppUpdateScene::onUpdatedOne(GBUpdateInfo * pInfo)
{
	updateLabels();
}

void AppUpdateScene::onUpdateReady()
{
	int totalSize = GBUpdate::getInstance()->getDownloadTotalSize();
	if (totalSize / 1024.0f < 1024)
		_pLabelSize->setString(StringUtils::format(_updateSize.c_str(), StringUtils::format("%d K", totalSize / 1024).c_str()));
	else
		_pLabelSize->setString(StringUtils::format(_updateSize.c_str(), StringUtils::format("%.1f M", totalSize / 1024.0f / 1024.0f).c_str()));
	_pBtnSkip->setVisible(true);
	_pBtnUpdate->setVisible(true);
}

void AppUpdateScene::updateLabels()
{
	int totalSize = GBUpdate::getInstance()->getDownloadTotalSize();
	int downloadedSize = GBUpdate::getInstance()->getDownloadedSize();
	if (totalSize > 0)
	{
		_pLabelPercentage->setString(StringUtils::format("%d%%", downloadedSize * 100 / totalSize));			
		_pProgressFg->setPercent(downloadedSize * 100.0f / totalSize);
		_pLabelSize->setString(StringUtils::format("%d/%d", downloadedSize, totalSize));

		if (totalSize / 1024.0f < 1024)
		{
			_pLabelSize->setString(StringUtils::format("%d K / %d K", downloadedSize / 1024, totalSize / 1024));
		}
		else
		{
			if (downloadedSize / 1024.0f < 1024)
			{
				_pLabelSize->setString(StringUtils::format("%d K / %.1f M", downloadedSize / 1024, totalSize / 1024.0f / 1024.0f));
			}
			else
			{
				_pLabelSize->setString(StringUtils::format("%.1f M / %.1f M", downloadedSize / 1024.0f / 1024.0f, totalSize / 1024.0f / 1024.0f));
			}
		}
	}	
}