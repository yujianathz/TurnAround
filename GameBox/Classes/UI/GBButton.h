#ifndef  _GB_BUTTON_H_
#define  _GB_BUTTON_H_

#include "UIButton.h"

#include "GBDefine.h"

using namespace cocos2d::ui;

#undef CLASSNAME
#define CLASSNAME GBButton

#define GBBUTTON_RELEASE_UP				"gbbutton release up"

class  GBButton : public Button
{
public:
	enum 
	{
		ACTION_TAG_PRESS = 0xffff,		
	};

	CREATE_WITH_P1(P1(const std::string&));

protected:
	virtual void			onPressStateChangedToNormal() override;
	virtual void			onPressStateChangedToPressed() override;
	virtual void			onPressStateChangedToDisabled() override;

	virtual void			releaseUpEvent() override;
};

#endif // _GB_BUTTON_H_

