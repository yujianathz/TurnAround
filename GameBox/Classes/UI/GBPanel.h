#ifndef _GBPANEL_H_
#define _GBPANEL_H_

#include "GBDefine.h"

class GBPanel : public Layout
{
public:
	static void				setPanelZOrder(int zorder);
	static void				setShowEvent(const std::string& e);
	static void				setHideEvent(const std::string& e);
	static bool				hasPanels();

	virtual void			onEnter();
	virtual void			onExit();
	virtual void			cleanup();

	virtual void			show();
	virtual void			hide(bool isShowAction = true);

	void					setCleanupCallback(const std::function<void()>& callback);

protected:
	virtual bool			init();

	void					updatePanels();

	std::function<void()>	_cleanupCallback;
};

#endif