#ifndef  _APP_UPDATE_SCENE_H_
#define  _APP_UPDATE_SCENE_H_

#include "GBDefine.h"
#include "GBUpdate.h"
#include "GBShaderButton.h"

#undef CLASSNAME
#define CLASSNAME AppUpdateScene

class AppUpdateScene : public Scene
{
public:
	CREATE_WITH_P0();

	virtual void onEnter();
	virtual void onExit();
	virtual void cleanup();

protected:
	void			onUpdateStart();
	void			onUpdateFinish();
	void			onUpdatedOne(GBUpdateInfo * pInfo);
	void			onUpdateReady();

	void			updateLabels();

	Vector<EventListener *>			_eventListeners;

	Sprite *							_pBg;
	Scale9Sprite *				_pProgressBg;
	LoadingBar *					_pProgressFg;

	std::string						_updateUrl;
	std::string						_updateSize;
	std::string						_updateFolder;

	Label *							_pLabelPercentage;
	Label *							_pLabelSize;
	Label *							_pLabelVersion;

	GBShaderButton *			_pBtnSkip;
	GBShaderButton *			_pBtnUpdate;
};

#endif // _APP_UPDATE_SCENE_H_

