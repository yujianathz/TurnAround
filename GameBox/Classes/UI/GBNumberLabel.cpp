#include "GBNumberLabel.h"
#include "GBUtils.h"

GBNumberLabel * GBNumberLabel::createWithTTF(int targetNum, const std::string& format, const std::string& font, int size, bool isShort)
{
	GBNumberLabel * pLabel = new GBNumberLabel();
	if (pLabel && pLabel->initWithTTF(targetNum, format, font, size, isShort))
	{
		pLabel->autorelease();
		return pLabel;
	}
	CC_SAFE_DELETE(pLabel);
	return NULL;
}

GBNumberLabel * GBNumberLabel::createWithSystemFont(int targetNum, const std::string& format, const std::string& font, int size, bool isShort)
{
	GBNumberLabel * pLabel = new GBNumberLabel();
	if (pLabel && pLabel->initWithSystemFont(targetNum, format, font, size, isShort))
	{
		pLabel->autorelease();
		return pLabel;
	}
	CC_SAFE_DELETE(pLabel);
	return NULL;
}

GBNumberLabel * GBNumberLabel::createWithBMFont(int targetNum, const std::string& format, const std::string& fnt, bool isShort)
{
	GBNumberLabel * pLabel = new GBNumberLabel();
	if (pLabel && pLabel->initWithBMFont(targetNum, format, fnt, isShort))
	{
		pLabel->autorelease();
		return pLabel;
	}
	CC_SAFE_DELETE(pLabel);
	return NULL;
}

GBNumberLabel * GBNumberLabel::createWithCharMap(int targetNum, const std::string& charMapPath, int itemWidth, int itemHeight)
{
	GBNumberLabel * pLabel = new GBNumberLabel();
	if (pLabel && pLabel->initWithCharMap(targetNum, charMapPath, itemWidth, itemHeight))
	{
		pLabel->autorelease();
		return pLabel;
	}
	CC_SAFE_DELETE(pLabel);
	return NULL;
}

bool GBNumberLabel::initWithTTF(int targetNum, const std::string& format, const std::string& font, int size, bool isShort)
{
	if (!Node::init())
		return false;

	_isShort = isShort;
	_curNumber = 0;
	_format = format;
	_pLabel = Label::createWithTTF(StringUtils::format(format.c_str(), _curNumber), font, size);
	setContentSize(_pLabel->getContentSize());
	setAnchorPoint(Vec2(0.5f, 0.5f));
	setCascadeOpacityEnabled(true);
	setCascadeColorEnabled(true);
	GBUtils::addChild2Center(this, _pLabel);

	setTargetNum(targetNum);

	return true;
}

bool GBNumberLabel::initWithSystemFont(int targetNum, const std::string& format, const std::string& font, int size, bool isShort)
{
	if (!Node::init())
		return false;

	_isShort = isShort;
	_curNumber = 0;
	_format = format;
	_pLabel = Label::createWithSystemFont(StringUtils::format(format.c_str(), _curNumber), font, size);
	setContentSize(_pLabel->getContentSize());
	setAnchorPoint(Vec2(0.5f, 0.5f));
	setCascadeOpacityEnabled(true);
	setCascadeColorEnabled(true);
	GBUtils::addChild2Center(this, _pLabel);

	setTargetNum(targetNum);

	return true;
}

bool GBNumberLabel::initWithBMFont(int targetNum, const std::string& format, const std::string& fnt, bool isShort)
{
	if (!Node::init())
		return false;

	_isShort = isShort;
	_curNumber = 0;
	_format = format;
	_pLabel = Label::createWithBMFont(fnt, StringUtils::format(format.c_str(), _curNumber));
	setContentSize(_pLabel->getContentSize());
	setAnchorPoint(Vec2(0.5f, 0.5f));
	setCascadeOpacityEnabled(true);
	setCascadeColorEnabled(true);
	GBUtils::addChild2Center(this, _pLabel);

	setTargetNum(targetNum);

	return true;
}

bool GBNumberLabel::initWithCharMap(int targetNum, const std::string& charMapPath, int itemWidth, int itemHeight)
{
	if (!Node::init())
		return false;

	_isShort = false;
	_curNumber = 0;
	_format = "%d";
	_pLabel = Label::createWithCharMap(charMapPath, itemWidth, itemHeight, '0');
	_pLabel->setString(StringUtils::format(_format.c_str(), _curNumber));
	setContentSize(_pLabel->getContentSize());
	setAnchorPoint(Vec2(0.5f, 0.5f));
	setCascadeOpacityEnabled(true);
	setCascadeColorEnabled(true);
	GBUtils::addChild2Center(this, _pLabel);

	setTargetNum(targetNum);

	return true;
}

void GBNumberLabel::enableOutline(const Color4B& outlineColor, int outlineSize /* = -1 */)
{
	_pLabel->enableOutline(outlineColor, outlineSize);
}

void GBNumberLabel::onEnter()
{
	Node::onEnter();

	scheduleUpdate();
}

void GBNumberLabel::onExit()
{
	unscheduleUpdate();

	Node::onExit();
}

void GBNumberLabel::update(float dt)
{
	if (_curNumber == _targetNum)
		return;

	_curNumber = MIN(_targetNum, _curNumber + _deltaNum);
	if (_targetCallback)
	{
		if (_curNumber == _targetNum)
		{
			_targetCallback();
		}
	}

	if (_isShort && _curNumber >= 10000)
	{
		size_t pos = _format.find("%d");
		string str1 = _format.substr(0, pos);
		string str2 = _format.substr(pos + 2);
		_pLabel->setString(StringUtils::format("%s%.1fw%s", str1.c_str(), _curNumber / 10000.0f, str2.c_str()));
	}
	else
		_pLabel->setString(StringUtils::format(_format.c_str(), _curNumber));
	setContentSize(_pLabel->getContentSize());
	_pLabel->setPosition(Vec2(W(this) / 2, H(this) / 2));
	if (_pLabel->getNumberOfRunningActions() == 0)
		_pLabel->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.05f, 1.1f), ScaleTo::create(0.05f, 1.0f)));
}

void GBNumberLabel::setTargetNum(int targetNum, const std::function<void()>& callback)
{
	if (_targetNum == targetNum)
		return;
	_targetNum = targetNum;
	_targetCallback = callback;

	_deltaNum = MAX(1, (_targetNum - _curNumber) / 80.0f);
}

void GBNumberLabel::setFormat(const std::string& format)
{
	_format = format;
	_pLabel->setString(StringUtils::format(format.c_str(), _curNumber));
}