#include "LoginScene.h"
#include "ChatScene.h"
#ifdef _WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>

USING_NS_CC;
USING_NS_CC_EXT;

#define GATE_HOST "114.113.202.141"
#define GATE_PORT 3088
//#define GATE_HOST "127.0.0.1"
//#define GATE_PORT 3014

static const char* connectorHost = "";
static int connectorPort = 0;
static std::string username = "";
static std::string channel = "";
static pc_client_t* pomelo_client = NULL;
static CCArray* messageQueue = NULL;
static CCArray* userQueue = NULL;
static int error = 0;
static json_t* userList = NULL;

static void on_close(pc_client_t *client, const char *event, void *data);

void Login::onChatCallback(pc_client_t *client, const char *event, void *data)
{
    json_t* json = (json_t* )data;
    const char* msg = json_dumps(json, 0);
    CCLOG("%s %s", event, msg);
    messageQueue->addObject(CCString::create(msg));
    return;
}

void Login::onAddCallback(pc_client_t *client, const char *event, void *data)
{
    if (!userQueue) {
        return;
    }
    json_t* json = (json_t* )data;
    json_t* user = json_object_get(json, "user");
    const char* msg = json_string_value(user);
    CCLOG("%s %s", event, msg);
    userQueue->addObject(CCString::create(msg));
    return;
}

void Login::onLeaveCallback(pc_client_t *client, const char *event, void *data)
{
    if (!userQueue) {
        return;
    }
    json_t* json = (json_t* )data;
    json_t* user = json_object_get(json, "user");
    const char* msg = json_string_value(user);
    CCLOG("%s %s", event,msg);
    for (unsigned int i=0; i<userQueue->count(); i++) {
        CCString* cstring = (CCString* )userQueue->objectAtIndex(i);
        if (strcmp(cstring->getCString(),msg) == 0) {
            userQueue->removeObjectAtIndex(i);
            break;
        }
    }
    return;
}

void Login::onDisconnectCallback(pc_client_t *client, const char *event, void *data)
{
    CCLOG("%s", event);
    return;
}

void Login::requstGateCallback(pc_request_t *req, int status, json_t *resp)
{
    if(status == -1) {
        CCLOG("Fail to send request to server.\n");
    } else if(status == 0) {
        connectorHost = json_string_value(json_object_get(resp, "host"));
        connectorPort = json_number_value(json_object_get(resp, "port"));
        CCLOG("%s %d", connectorHost,connectorPort);
        pc_client_t *client = pc_client_new();

        struct sockaddr_in address;

        memset(&address, 0, sizeof(struct sockaddr_in));
        address.sin_family = AF_INET;
        address.sin_port = htons(connectorPort);
        address.sin_addr.s_addr = inet_addr(connectorHost);

        // add pomelo events listener
        void (*on_disconnect)(pc_client_t *client, const char *event, void *data) = &Login::onDisconnectCallback;
        void (*on_chat)(pc_client_t *client, const char *event, void *data) = &Login::onChatCallback;
        void (*on_add)(pc_client_t *client, const char *event, void *data) = &Login::onAddCallback;
        void (*on_leave)(pc_client_t *client, const char *event, void *data) = &Login::onLeaveCallback;

        pc_add_listener(client, "disconnect", on_disconnect);
        pc_add_listener(client, "onChat", on_chat);
        pc_add_listener(client, "onAdd", on_add);
        pc_add_listener(client, "onLeave", on_leave);

        // try to connect to server.
        if(pc_client_connect(client, &address)) {
            CCLOG("fail to connect server.\n");
            pc_client_destroy(client);
            return ;
        }

        const char *route = "connector.entryHandler.enter";
        json_t *msg = json_object();
        json_t *str = json_string(username.c_str());
        json_t *channel_str = json_string(channel.c_str());
        json_object_set(msg, "username", str);
        json_object_set(msg, "rid", channel_str);
        // decref for json object
        json_decref(str);
        json_decref(channel_str);

        pc_request_t *request = pc_request_new();
        void (*connect_cb)(pc_request_t *req, int status, json_t *resp )= &Login::requstConnectorCallback;
        pc_request(client, request, route, msg, connect_cb);
		
		/*
        char *json_str = json_dumps(resp, 0);
        if(json_str != NULL) {
            CCLOG("server response: %s %d\n", connectorHost, connectorPort);
            free(json_str);
        }
		*/
    }

    // release relative resource with pc_request_t
    json_t *pc_msg = req->msg;
    pc_client_t *pc_client = req->client;
    json_decref(pc_msg);
    pc_request_destroy(req);

    pc_client_stop(pc_client);
}

void Login::requstConnectorCallback(pc_request_t *req, int status, json_t *resp)
{
    error = 0;
    if(status == -1) {
        CCLOG("Fail to send request to server.\n");
    } else if(status == 0) {
        char *json_str = json_dumps(resp, 0);
        CCLOG("server response: %s \n", json_str);
		json_t* users = json_object_get(resp,"users");
        if(json_object_get(resp, "error") != NULL) {
            error = 1;
            CCLOG("connect error %s", json_str);
            free(json_str);
            return;
        }
        pomelo_client = req->client;
        for (unsigned int i=0; i<json_array_size(users); i++) {
            json_t* val = json_array_get(users,i);
            userQueue->addObject(CCString::create(json_string_value(val)));
        }
    }

    // release relative resource with pc_request_t
    json_t *msg = req->msg;
    pc_client_t *client = req->client;
    json_decref(msg);
    pc_request_destroy(req);
}

// disconnect event callback.
void on_close(pc_client_t *client, const char *event, void *data)
{
    CCLOG("client closed: %d.\n", client->state);
}

void Login::doLogin()
{	
    const char *ip = GATE_HOST;
    int port = GATE_PORT;

    pc_client_t *client = pc_client_new();

    struct sockaddr_in address;

    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);

    // try to connect to server.
    if(pc_client_connect(client, &address)) {
        CCLOG("fail to connect server.\n");
        pc_client_destroy(client);
        return ;
    }
	
	// add some event callback.
	pc_add_listener(client, PC_EVENT_DISCONNECT, on_close);

    const char *route = "gate.gateHandler.queryEntry";
    json_t *msg = json_object();
    json_t *str = json_string(username.c_str());
    json_object_set(msg, "uid", str);
    // decref for json object
    json_decref(str);

    pc_request_t *request = pc_request_new();
    void (*on_request_gate_cb)(pc_request_t *req, int status, json_t *resp) = &Login::requstGateCallback;
    pc_request(client, request, route, msg, on_request_gate_cb);

    // main thread has nothing to do and wait until child thread return.
    pc_client_join(client);

    // release the client
    pc_client_destroy(client);
}

CCScene *Login::scene()
{
    CCScene *scene = NULL;
    do {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        // 'layer' is an autorelease object
        Login *layer = Login::create();
        CC_BREAK_IF(! layer);
        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);
    // return the scene
    return scene;
}

void Login::onEnter()
{
    CCLayer::onEnter();
    CCLOG("Login onEnter");
    pomelo_client = NULL;
    messageQueue = new CCArray();
    messageQueue->init();

    userQueue = new CCArray();
    userQueue->init();

    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
        schedule_selector(Login::dispatchLoginCallbacks), this, 0, false);
}

// on "init" you need to initialize your instance
bool Login::init()
{
    bool bRet = false;
    do {
        CC_BREAK_IF(! CCLayer::init());
        CCPoint visibleOrigin = CCEGLView::sharedOpenGLView()->getVisibleOrigin();
        CCSize visibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();
        // top
        CCSize editBoxSize = CCSizeMake(visibleSize.width - 100, 60);
        m_pEditName = CCEditBox::create(editBoxSize , CCScale9Sprite::create("green_edit.png"));
        m_pEditName->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height * 3 / 4));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        m_pEditName->setFont("Paint Boy", 25);
#else
        m_pEditName->setFont("Arial", 25);
#endif
        m_pEditName->setFontColor(ccBLACK);
        m_pEditName->setPlaceHolder("Name:");
        m_pEditName->setPlaceholderFontColor(ccWHITE);
        m_pEditName->setMaxLength(8);
        m_pEditName->setReturnType(kKeyboardReturnTypeDone);
        m_pEditName->setDelegate(this);
        this->addChild(m_pEditName, 1);
        // middle
        m_pEditChannel = CCEditBox::create(editBoxSize, CCScale9Sprite::create("green_edit.png"));
        m_pEditChannel->setPosition(ccp(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height / 2));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        m_pEditChannel->setFont("American Typewriter", 30);
#else
        m_pEditChannel->setFont("Arial", 25);
#endif
        m_pEditChannel->setFontColor(ccBLACK);
        m_pEditChannel->setPlaceHolder("Channel:");
        m_pEditChannel->setPlaceholderFontColor(ccWHITE);
        m_pEditChannel->setMaxLength(8);
        m_pEditChannel->setReturnType(kKeyboardReturnTypeDone);
        m_pEditChannel->setDelegate(this);
        this->addChild(m_pEditChannel, 1);
        // 1. Add a menu item with "X" image, which is clicked to quit the program.
        CCLabelTTF *label = CCLabelTTF::create("Login", "Arial", 50);
        //#endif
        CCMenuItemLabel *pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(Login::onLogin));
        CCMenu *pMenu = CCMenu::create(pMenuItem, NULL);
        pMenu->setPosition( CCPointZero );
        pMenuItem->setPosition( ccp(visibleOrigin.x + visibleSize.width / 2, visibleOrigin.y + visibleSize.height / 4) );
        //m_pEditEmail->setAnchorPoint(ccp(0.5, 1.0f));
        this->addChild(pMenu, 1);
        CCLabelTTF *pLabel = CCLabelTTF::create("pomelo-cocos2dchat", "Arial", 30);
        CC_BREAK_IF(! pLabel);
        // Get window size and place the label upper.
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width / 2, size.height - 30));
        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 1);
        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite *pSprite = CCSprite::create("HelloWorld.png");
        CC_BREAK_IF(! pSprite);
        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width / 2, size.height / 2));
        // Add the sprite to HelloWorld layer as a child layer.
        this->addChild(pSprite, 0);

        bRet = true;
    } while (0);
    return bRet;
}

void Login::dispatchLoginCallbacks(float delta)
{
    // wait for pomelo_client init from connector callback
    if(pomelo_client == NULL || error == 1) {
        return;
    }

    CCDirector::sharedDirector()->getScheduler()->pauseTarget(this);

    CCScene* pScene = CCScene::create();
    Chat* pLayer = new Chat();
    pLayer->setChannel(channel);
    pLayer->setUser(username);
    pLayer->setClient(pomelo_client);
    pLayer->setUserQueue(userQueue);
    pLayer->setMessageQueue(messageQueue);

    CCLOG("init player");
    if(pLayer && pLayer->init()) {
        //pLayer->autorelease();
        pScene->addChild(pLayer);
        CCLOG("director replaceScene");
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, pScene));
    } else {
        delete pLayer;
        pLayer = NULL;
    }
}

void Login::onLogin(CCObject *pSender)
{
    doLogin();
}

void Login::editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

void Login::editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void Login::editBoxTextChanged(cocos2d::extension::CCEditBox *editBox, const std::string &text)
{
    if (editBox == m_pEditName) {
        username = text;
        CCLog("name editBox %p TextChanged, text: %s ", editBox, text.c_str());
    } else {
        channel = text;
        CCLog("channel editBox %p TextChanged, text: %s ", editBox, text.c_str());
    }
}

void Login::editBoxReturn(cocos2d::extension::CCEditBox *editBox)
{
    CCLog("editBox %p was returned !");
}