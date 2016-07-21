#ifndef  _GB_LIST_H_
#define  _GB_LIST_H_

#include "UIListView.h"

#include "GBDefine.h"

using namespace cocos2d::ui;

#undef CLASSNAME
#define CLASSNAME GBList

class GBList : public ListView
{
public:	
	typedef std::function<void(Widget *, void *)> GBUpdateItemCallback;

	GBList();

	CREATE_WITH_P4(P1(const Size&), P2(Direction), P3_V(float, 0.0f), P4_V(float, 0.0f));
	
	void bindData(const std::vector<void *>& data, const GBUpdateItemCallback& callback, int cache);

	CC_SYNTHESIZE_READONLY(int, _cache, Cache);

protected:
	void			onScrollEvent(Ref * pSender, ScrollviewEventType eventType);

	GBUpdateItemCallback						_updateItemCallback;

	std::vector<void *>							_data;

	int											_startIndex;
	int											_endIndex;
};

#endif // _GB_BUTTON_H_

