#include "GBForm.h"
#include "GBUtils.h"

static std::string s_framename = "";
static Rect s_capInsets = Rect::ZERO;
static float s_leftMargin = 0;
static float s_rightMargin = 0;
static float s_topMargin = 0;
static float s_bottomMargin = 0;

void GBForm::setFrame(const std::string& framename, const Rect& capInsets)
{
	s_framename = framename;
	s_capInsets = capInsets;
}

void GBForm::setMargin(float left, float right, float top, float bottom)
{
	s_leftMargin = left;
	s_rightMargin = right;
	s_topMargin = top;
	s_bottomMargin = bottom;
}

float GBForm::getLeftMargin()
{
	return s_leftMargin;
}

float GBForm::getRightMargin()
{
	return s_rightMargin;
}

float GBForm::getTopMargin()
{
	return s_topMargin;
}

float GBForm::getBottomMargin()
{
	return s_bottomMargin;
}

bool GBForm::init(const Size& size)
{
	if (!GBPanel::init())
		return false;

	CCASSERT(!s_framename.empty(), "Please call setFrame first");

	_pBg = ImageView::create(s_framename, TextureResType::PLIST);
	_pBg->setScale9Enabled(true);
	_pBg->setCapInsets(s_capInsets);	
	_pBg->setContentSize(size);	
	addChild(_pBg);
	
	return true;
}

void GBForm::show()
{
	if (_pBg->getNumberOfRunningActions() == 0)
	{
		GBPanel::show();
		_pBg->setPosition(Vec2(W(this) / 2, -H(_pBg) / 2));
		_pBg->runAction(Sequence::createWithTwoActions(EaseBackOut::create(MoveTo::create(0.3f, Vec2(X(_pBg), H(this) / 2))), CallFunc::create([this]()
		{
			didEnter();
		})));
	}
}

void GBForm::hide(bool isShowAction)
{
	if (isShowAction)
	{
		if (_pBg->getNumberOfRunningActions() == 0)
		{
			didExit();
			_pBg->runAction(Sequence::create(EaseBackIn::create(MoveTo::create(0.3f, Vec2(X(_pBg), -H(this) / 2))), CallFunc::create([this](){ GBPanel::hide(); }), NULL));
		}			
	}
	else
	{
		GBPanel::hide(isShowAction);
	}
}

void GBForm::didEnter()
{
	// override
}

void GBForm::didExit()
{
	// override
}