#ifndef  _GB_DEFINE_H_
#define  _GB_DEFINE_H_

#include "cocos2d.h"
#include "CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace std;

#include "GBSqlite.h"
#include "GBLang.h"

#define DIRECTOR()				Director::getInstance()
#define CONFIG()				Configuration::getInstance()
#define FILE()					FileUtils::getInstance()
#define USER()					UserDefault::getInstance()
#define LANG()					GBLang::getInstance()
#define FRAMES()				SpriteFrameCache::getInstance()
#define SCENE()					DIRECTOR()->getRunningScene()

#define STR(strIndex)			LANG()->getString(strIndex)

#define SIZE(node)				node->getContentSize()
#define W(node)					node->getContentSize().width
#define SW(node)				(node->getContentSize().width * node->getScaleX())
#define H(node)					node->getContentSize().height
#define SH(node)				(node->getContentSize().height * node->getScaleY())
#define X(node)					node->getPositionX()
#define Y(node)					node->getPositionY()
#define AX(node)				node->getAnchorPoint().x
#define AY(node)				node->getAnchorPoint().y
#define LEFT(node)				(X(node) - AX(node) * SW(node))
#define RIGHT(node)				(X(node) + (1 - AX(node)) * SW(node))
#define BOTTOM(node)			(Y(node) - AY(node) * SH(node))
#define TOP(node)				(Y(node) + (1 - AY(node)) * SH(node))

#define FRAME_BY_NAME(framename)	SpriteFrameCache::getInstance()->getSpriteFrameByName(framename)

#define SAFE_CALL(o, f)			if (o) o->f

#define P1(t)					t p1
#define P1_V(t, v)				t p1 = v
#define P2(t)					t p2
#define P2_V(t, v)				t p2 = v
#define P3(t)					t p3
#define P3_V(t, v)				t p3 = v
#define P4(t)					t p4
#define P4_V(t, v)				t p4 = v
#define P5(t)					t p5
#define P5_V(t, v)				t p5 = v
#define P6(t)					t p6
#define P6_V(t, v)				t p6 = v

#define CREATE_WITH_P0()		\
	public:						\
		static CLASSNAME * create()			\
		{\
			CLASSNAME * pObj = new (std::nothrow) CLASSNAME();		\
			if (pObj && pObj->init())				\
			{\
				pObj->autorelease();				\
				return pObj;						\
			}\
			CC_SAFE_DELETE(pObj);					\
			return NULL;							\
		}\
		virtual bool init()							

#define CREATE_WITH_P1(tp1)							\
	public:											\
		static CLASSNAME * create(tp1)				\
		{\
			CLASSNAME * pObj = new (std::nothrow) CLASSNAME();		\
			if (pObj && pObj->init(p1))				\
			{\
				pObj->autorelease();				\
				return pObj;						\
			}\
			CC_SAFE_DELETE(pObj);					\
			return NULL;							\
		}\
		virtual bool init(tp1)		

#define CREATE_WITH_P2(tp1, tp2)					\
	public:											\
		static CLASSNAME * create(tp1, tp2)			\
		{\
			CLASSNAME * pObj = new (std::nothrow) CLASSNAME();		\
			if (pObj && pObj->init(p1, p2))			\
			{\
				pObj->autorelease();				\
				return pObj;						\
			}\
			CC_SAFE_DELETE(pObj);					\
			return NULL;							\
		}\
		virtual bool init(tp1, tp2)	

#define CREATE_WITH_P3(tp1, tp2, tp3)				\
	public:											\
		static CLASSNAME * create(tp1, tp2, tp3)	\
		{\
			CLASSNAME * pObj = new (std::nothrow) CLASSNAME();		\
			if (pObj && pObj->init(p1, p2, p3))		\
			{\
				pObj->autorelease();				\
				return pObj;						\
			}\
			CC_SAFE_DELETE(pObj);					\
			return NULL;							\
		}\
		virtual bool init(tp1, tp2, tp3)	

#define CREATE_WITH_P4(tp1, tp2, tp3, tp4)				\
	public:											\
	static CLASSNAME * create(tp1, tp2, tp3, tp4)	\
		{\
		CLASSNAME * pObj = new (std::nothrow) CLASSNAME();		\
		if (pObj && pObj->init(p1, p2, p3, p4))		\
			{\
			pObj->autorelease();				\
			return pObj;						\
			}\
			CC_SAFE_DELETE(pObj);					\
			return NULL;							\
		}\
		virtual bool init(tp1, tp2, tp3, tp4)	

#define CREATE_WITH_P5(tp1, tp2, tp3, tp4, tp5)				\
	public:											\
	static CLASSNAME * create(tp1, tp2, tp3, tp4, tp5)	\
		{\
		CLASSNAME * pObj = new (std::nothrow) CLASSNAME();		\
		if (pObj && pObj->init(p1, p2, p3, p4, p5))		\
			{\
				pObj->autorelease();				\
				return pObj;						\
			}\
			CC_SAFE_DELETE(pObj);					\
			return NULL;							\
		}\
		virtual bool init(tp1, tp2, tp3, tp4, tp5)	

#define CREATE_WITH_P6(tp1, tp2, tp3, tp4, tp5, tp6)				\
	public:											\
	static CLASSNAME * create(tp1, tp2, tp3, tp4, tp5, tp6)	\
		{\
		CLASSNAME * pObj = new (std::nothrow) CLASSNAME();		\
if (pObj && pObj->init(p1, p2, p3, p4, p5, p6))		\
			{\
			pObj->autorelease();				\
			return pObj;						\
			}\
			CC_SAFE_DELETE(pObj);					\
			return NULL;							\
		}\
		virtual bool init(tp1, tp2, tp3, tp4, tp5, tp6)

#endif