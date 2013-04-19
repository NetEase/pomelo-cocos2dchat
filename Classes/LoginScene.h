#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "VisibleRect.h"
#include "pomelo.h"

class Login : public cocos2d::CCLayer, public cocos2d::extension::CCEditBoxDelegate
{
public:
    virtual bool init();  
    static cocos2d::CCScene* scene();
	CREATE_FUNC(Login);
    
	// cocos2d-x schedule handle callback
	void dispatchLoginCallbacks(float delta);
	
	// cocos2d-x events callback
	void onLogin(CCObject* pSender);
	virtual void onEnter();
	virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
    virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
    virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);

	// pomelo handle
	void doLogin();
	// pomelo events callback
	static void onChatCallback(pc_client_t *client, const char *event, void *data);
	static void onAddCallback(pc_client_t *client, const char *event, void *data);
	static void onLeaveCallback(pc_client_t *client, const char *event, void *data);
	static void onDisconnectCallback(pc_client_t *client, const char *event, void *data);
	static void requstGateCallback(pc_request_t *req, int status, json_t *resp);
	static void requstConnectorCallback(pc_request_t *req, int status, json_t *resp);

private:
	std::string s_EditName;
	std::string s_EditChannel;
	cocos2d::extension::CCEditBox* m_pEditName;
    cocos2d::extension::CCEditBox* m_pEditChannel;
};

#endif  // __LOGIN_SCENE_H__