#ifndef __CHAT_SCENE_H__
#define __CHAT_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "VisibleRect.h"
#include "pomelo.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class Chat : public cocos2d::CCLayer,public cocos2d::extension::CCEditBoxDelegate, public cocos2d::CCTextFieldDelegate
{
    private:
        CCPoint m_tBeginPos;
        CCMenu* m_pItemMenu;
		CCLayer* m_layer;
        CCTextFieldTTF* m_pTextField;
        cocos2d::CCLabelTTF* m_pTTFShowEditReturn;
        cocos2d::extension::CCEditBox* m_pEditName;
        cocos2d::CCLabelTTF* channelText;
        string s_text;
        string s_channel;
        string s_user;
		pc_client_t *client;
		CCArray* messageQueue;
		CCArray* userQueue;
    public:
        virtual bool init();
        //static cocos2d::CCScene* scene();
        void menuCallback(cocos2d::CCObject *sender);
        void backCallback(cocos2d::CCObject *sender);
        void userCallback(cocos2d::CCObject *sender);
        void sendCallback(cocos2d::CCObject *sender);

		void updateUICallback(float delta);
		void addMessage(const char* event, void* data);
		static CCLayer* sharedMItemLayer();
		
		CREATE_FUNC(Chat);

        void setChannel(string channel);
        void setUser(string user);
		void setClient(pc_client_t* client);
		void setMessageQueue(CCArray* queue);
		void setUserQueue(CCArray* queue);

		virtual void onEnter();
        virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
        virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
        virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);

        virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
        virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
        virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
        virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);

        // CCTextFieldDelegate
        virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * pSender);
        virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
        virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
        virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);
};

#endif  // __CHAT_SCENE_H__