#include "GBPanel.h"

static std::string s_showEvent = "";
static std::string s_hideEvent = "";
static int s_localZOrder = 0;
static Vector<GBPanel *> s_Panels;

void GBPanel::setPanelZOrder(int zorder)
{
	s_localZOrder = zorder;
}

void GBPanel::setShowEvent(const std::string& e)
{
	s_showEvent = e;
}

void GBPanel::setHideEvent(const std::string& e)
{
	s_hideEvent = e;
}

bool GBPanel::hasPanels()
{
	return s_Panels.size() > 0;
}

bool GBPanel::init()
{
	if (!Layout::init())
		return false;

	setContentSize(DIRECTOR()->getWinSize());
	setBackGroundColorType(BackGroundColorType::SOLID);
	setBackGroundColor(Color3B::BLACK);
	setBackGroundColorOpacity(0xc0);
	setTouchEnabled(true);

	_cleanupCallback = nullptr;

	return true;
}

void GBPanel::onEnter()
{
	Layout::onEnter();

	if (s_Panels.find(this) == s_Panels.end())
	{
		s_Panels.pushBack(this);
		updatePanels();
	}		

	if (!s_showEvent.empty() && !s_Panels.empty())
		_eventDispatcher->dispatchCustomEvent(s_showEvent, this);
}

void GBPanel::onExit()
{
	Layout::onExit();

	if (s_Panels.find(this) != s_Panels.end())
	{
		s_Panels.eraseObject(this);
		updatePanels();
	}		

	if (!s_hideEvent.empty() && s_Panels.empty())	
		_eventDispatcher->dispatchCustomEvent(s_hideEvent, this);
}

void GBPanel::cleanup()
{
	Layout::cleanup();

	if (_cleanupCallback)
		_cleanupCallback();
}

void GBPanel::show()
{
	DIRECTOR()->getRunningScene()->addChild(this, s_localZOrder);
}

void GBPanel::hide(bool isShowAction)
{
	removeFromParent();
}

void GBPanel::setCleanupCallback(const std::function<void()>& callback)
{
	_cleanupCallback = callback;
}

void GBPanel::updatePanels()
{
	if (!s_Panels.empty())
	{
		s_Panels.back()->setBackGroundColorOpacity(0xc0);
		for (auto it = s_Panels.rbegin() + 1; it != s_Panels.rend(); it++)
			(*it)->setBackGroundColorOpacity(0);
	}
}