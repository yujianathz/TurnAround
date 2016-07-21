#ifndef _GBUTILS_H_
#define _GBUTILS_H_

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class GBUtils
{
public:
	static bool								containTouch(Touch * pTouch, Node * pNode);
	static void								split(const std::string& str, std::vector<std::string>& ret_, std::string sep);
	static std::vector<std::string>			readDirectory(const std::string& relativePath, const std::string keyWord = "");
	static std::vector<unsigned int>		randomNumberArray(unsigned int number, unsigned int totalNumber);

	static std::string						coding(const std::string &str,const char* toCode,const char* formCode);
	static void								openURL(std::string &url);

	static void								addChild2Center(Node * pParent, Node * pChild, int zorder = 0);
	static void								addChild2Pos(Node * pParent, Node * pChild, const Vec2& pos = Vec2::ZERO, int zorder = 0);

	static Vector<SpriteFrame *>			getAnimationFrames(const std::string& animationName);

	static std::string						getChannel();
	static std::string						getDeviceId();

	static void								vibration(float duration);

	static void								purchase(const string& payInfo);

	static std::string						getExternalPath(const std::string& relativePath);

	static std::string						getOpenUDID();
};

#endif // _GBUTILS_H_