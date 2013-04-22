#include "UserScene.h"
#include "CustomTableViewCell.h"

USING_NS_CC;
USING_NS_CC_EXT;

// on "init" you need to initialize your instance
bool Users::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCTableView *tableView = CCTableView::create(this, CCSizeMake(VisibleRect::getVisibleRect().size.width, 60));

    tableView = CCTableView::create(this, CCSizeMake(VisibleRect::getVisibleRect().size.width, VisibleRect::getVisibleRect().size.height));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(CCPointZero);
    tableView->setDelegate(this);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView);
    tableView->reloadData();

    // Back Menu
    CCMenuItemFont *itemBack = CCMenuItemFont::create("Back", this, menu_selector(Users::backCallback));
    itemBack->setPosition(ccp(VisibleRect::rightBottom().x - 50, VisibleRect::rightBottom().y + 25));
    CCMenu *menuBack = CCMenu::create(itemBack, NULL);
    menuBack->setPosition(CCPointZero);
    addChild(menuBack);

    return true;
}

void Users::backCallback(cocos2d::CCObject *sender)
{
    CCDirector::sharedDirector()->popScene();
}

void Users::tableCellTouched(CCTableView *table, CCTableViewCell *cell)
{
    CCLOG("cell touched at index: %i", cell->getIdx());
}

CCSize Users::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(60, 60);
}

CCTableViewCell *Users::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCString *string = (CCString * )userQueue->objectAtIndex(idx);

    CCTableViewCell *cell = table->dequeueCell();
    if (!cell)
    {
        cell = new CustomTableViewCell();
        cell->autorelease();
        CCSprite *sprite = CCSprite::create("Icon.png");
        sprite->setAnchorPoint(CCPointZero);
        sprite->setPosition(ccp(0, 0));
        cell->addChild(sprite);

        CCLabelTTF *label = CCLabelTTF::create(string->getCString(), "Helvetica", 20.0);
        label->setPosition(CCPointZero);
        label->setAnchorPoint(CCPointZero);
        label->setTag(123);
        cell->addChild(label);
    }
    else
    {
        CCLabelTTF *label = (CCLabelTTF *)cell->getChildByTag(123);
        label->setString(string->getCString());
    }

    return cell;
}

unsigned int Users::numberOfCellsInTableView(CCTableView *table)
{
    return userQueue->count();
}

void Users::setUserQueue(CCArray *queue)
{
    userQueue = queue;
}
