#ifndef _GBFORM_H_
#define _GBFORM_H_

#include "GBPanel.h"

class GBForm : public GBPanel
{
public:
	static void			setFrame(const std::string& framename, const Rect& capInsets);
	static void			setMargin(float left, float right, float top, float bottom);
	static float		getLeftMargin();
	static float		getRightMargin();
	static float		getTopMargin();
	static float		getBottomMargin();

	virtual bool		init(const Size& size);
	virtual void		show();
	virtual void		hide(bool isShowAction = true);

protected:
	virtual void		didEnter();
	virtual void		didExit();

	ImageView *			_pBg;

private:

};

#endif