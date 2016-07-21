#include "GBList.h"

GBList::GBList():
_cache(0),
_startIndex(0),
_endIndex(0)
{

}

bool GBList::init(const Size& size, Direction direction, float boundMargin, float itemsMargin)
{
	if (!ListView::init())
		return false;

	setContentSize(size);
	setDirection(direction);
	setScrollBarEnabled(false);

	if (direction == Direction::HORIZONTAL)
		setGravity(Gravity::CENTER_VERTICAL);
	else if (direction == Direction::VERTICAL)
		setGravity(Gravity::CENTER_HORIZONTAL);

	_boundMargin = boundMargin;
	_itemsMargin = itemsMargin;

	return true;
}

void GBList::bindData(const std::vector<void *>& data, const GBList::GBUpdateItemCallback& callback, int cache)
{
	_data = data;
	_updateItemCallback = callback;
	_cache = MIN((int)data.size(), cache);

	removeAllItems();

	_startIndex = 0;
	_endIndex = _cache - 1;

	addEventListenerScrollView(this, scrollvieweventselector(GBList::onScrollEvent));
}

void GBList::onScrollEvent(Ref * pSender, ScrollviewEventType eventType)
{
	bool isStart = false;
	if (_direction == Direction::HORIZONTAL)
	{
		if (eventType == SCROLLVIEW_EVENT_SCROLL_TO_LEFT)
			isStart = true;
		else if (eventType == SCROLLVIEW_EVENT_SCROLL_TO_RIGHT)
			isStart = false;
		else
			return;
	}
	else if (_direction == Direction::VERTICAL)
	{
		if (eventType == SCROLLVIEW_EVENT_SCROLL_TO_TOP)
			isStart = true;
		else if (eventType == SCROLLVIEW_EVENT_SCROLL_TO_BOTTOM)
			isStart = false;
		else
			return;
	}

	if (isStart)
	{
		if (_startIndex > 0)
		{
			_startIndex--;
			_endIndex--;

			Widget * pWidget = *(_items.rbegin());
			void * pData = _data[_startIndex];
			_updateItemCallback(pWidget, pData);

			_items.popBack();
			_items.insert(0, pWidget);

			if (_direction == Direction::HORIZONTAL)
				_innerContainer->setPositionX(X(_innerContainer) - W(pWidget) - _itemsMargin);
			else if (_direction == Direction::VERTICAL)
				_innerContainer->setPositionY(Y(_innerContainer) + H(pWidget) + _itemsMargin);
		
			this->forceDoLayout();
		}
	}
	else
	{

		if (_endIndex < (int)_data.size() - 1)
		{
			_startIndex++;
			_endIndex++;

			Widget * pWidget = *(_items.begin());
			void * pData = _data[_endIndex];
			_updateItemCallback(pWidget, pData);

			_items.erase(_items.begin());
			_items.pushBack(pWidget);

			if (_direction == Direction::HORIZONTAL)
				_innerContainer->setPositionX(X(_innerContainer) + W(pWidget) + _itemsMargin);
			else if (_direction == Direction::VERTICAL)
				_innerContainer->setPositionY(Y(_innerContainer) - H(pWidget) - _itemsMargin);
			
			this->forceDoLayout();
		}
	}
}