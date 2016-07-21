#ifndef  _GB_NUMBER_LABEL_H_
#define  _GB_NUMBER_LABEL_H_

#include "GBDefine.h"

#undef CLASSNAME
#define CLASSNAME GBNumberLabel

class  GBNumberLabel : public Node
{
public:
	static GBNumberLabel * createWithTTF(int targetNum, const std::string& format, const std::string& font, int size, bool isShort = false);
	static GBNumberLabel * createWithSystemFont(int targetNum, const std::string& format, const std::string& font, int size, bool isShort = false);
	static GBNumberLabel * createWithBMFont(int targetNum, const std::string& format, const std::string& fnt, bool isShort = false);
	static GBNumberLabel * createWithCharMap(int targetNum, const std::string& charMapPath, int itemWidth, int itemHeight);

	virtual bool			initWithTTF(int targetNum, const std::string& format, const std::string& font, int size, bool isShort);
	virtual bool			initWithSystemFont(int targetNum, const std::string& format, const std::string& font, int size, bool isShort);
	virtual bool			initWithBMFont(int targetNum, const std::string& format, const std::string& fnt, bool isShort);
	virtual bool			initWithCharMap(int targetNum, const std::string& charMapPath, int itemWidth, int itemHeight);

	virtual void			enableOutline(const Color4B& outlineColor, int outlineSize = -1);

	virtual void			onEnter();
	virtual void			onExit();
	virtual void			update(float dt);

	void						setTargetNum(int targetNum, const std::function<void()>& callback = nullptr);
	void						setFormat(const std::string& format);

	CC_SYNTHESIZE_READONLY(int, _targetNum, TargetNum);
	CC_SYNTHESIZE_READONLY(int, _curNumber, CurNumber);
	
protected:
	Label *					_pLabel;

	std::string				_format;
	int						_deltaNum;
	bool					_isShort;

	std::function<void()>	_targetCallback;
};

#endif // _GB_NUMBER_LABEL_H_

