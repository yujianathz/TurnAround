#include "GBUtils.h"
#include "GBDefine.h"
#include <string>

bool GBUtils::containTouch(Touch * pTouch, Node * pNode)
{
	Vec2 locationInNode = pNode->convertToNodeSpace(pTouch->getLocation());
	Size s = pNode->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	if (rect.containsPoint(locationInNode))
		return true;
	
	return false;
}

void GBUtils::split(const std::string& str, std::vector<std::string>& ret_, std::string sep)
{
	if (str.empty())
		return;

	std::string tmp;
	std::string::size_type pos_begin = str.find_first_not_of(sep);
	std::string::size_type comma_pos = 0;

	while (pos_begin != std::string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != std::string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			ret_.push_back(tmp);
			tmp.clear();
		}
	}
}

std::vector<unsigned int> GBUtils::randomNumberArray(unsigned int number, unsigned int totalNumber)
{
	std::vector<unsigned int> arr;
	while (arr.size() < number && arr.size() < totalNumber)
	{
		unsigned int n = rand() % totalNumber;
		bool isContain = false;
		for (unsigned int j = 0; j < arr.size(); ++ j)
		{
			if (arr[j] == n)
			{
				isContain = true;
				break;
			}
		}
		if (!isContain)
			arr.push_back(n);
	}

	return arr;
}

void GBUtils::addChild2Center(Node * pParent, Node * pChild, int zorder)
{
	Vec2 pos = Vec2(W(pParent) / 2 - (0.5f - AX(pChild)) * W(pChild), H(pParent) / 2 - (0.5f - AY(pChild)) * H(pChild));
	GBUtils::addChild2Pos(pParent, pChild, pos, zorder);
}

void GBUtils::addChild2Pos(Node * pParent, Node * pChild, const Vec2& pos, int zorder)
{
	pChild->setPosition(pos);
	pParent->addChild(pChild, zorder);
}

Vector<SpriteFrame *> GBUtils::getAnimationFrames(const std::string& animationName)
{
	Vector<SpriteFrame *> v;
	int i = 1;
	while (SpriteFrame * pFrame = FRAME_BY_NAME(StringUtils::format("%s_%02d", animationName.c_str(), i)))
	{
		v.pushBack(pFrame);
		i++;
	}
		
	return v;
}

std::string GBUtils::getExternalPath(const std::string& relativePath)
{
	string writablePath = FILE()->getWritablePath();
	char lastChar = *(writablePath.rbegin());
	if (lastChar == '\\' || lastChar == '/')
		writablePath = writablePath.substr(0, writablePath.length() - 1);

	string relPath = relativePath;
	char firstChar = *(relPath.begin());
	if (firstChar == '\\' || firstChar == '/')
		relPath = relPath.substr(1);
	
	return StringUtils::format("%s/%s", writablePath.c_str(), relPath.c_str());
}
