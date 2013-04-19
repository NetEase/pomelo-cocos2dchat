#ifndef __USER_SCENE_H__
#define __USER_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "VisibleRect.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Users : public cocos2d::CCLayer, public cocos2d::extension::CCTableViewDataSource, public cocos2d::extension::CCTableViewDelegate
{
	CCArray* userQueue;
public:
	virtual bool init();  

	void backCallback(cocos2d::CCObject *sender);

	CREATE_FUNC(Users);

	void setUserQueue(CCArray* queue);
	virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view) {};
	virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view) {}
	virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
	virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
	virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
};

#endif  // __USER_SCENE_H__