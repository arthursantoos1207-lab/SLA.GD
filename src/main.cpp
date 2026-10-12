
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/AppDelegate.hpp>

using namespace geode::prelude;

static bool g_noClip = false;

struct $modify(PlayerObject) {
    bool hitObject(GameObject* obj, bool p1) {
        if (g_noClip) return false;
        return PlayerObject::hitObject(obj, p1);
    }
    void killPlayer() {
        if (g_noClip) return;
        PlayerObject::killPlayer();
    }
};

struct $modify(AppDelegate) {
    void applicationDidFinishLaunching() {
        AppDelegate::applicationDidFinishLaunching();
        CCDirector::get()->setAnimationInterval(1.0 / 240.0);
    }
};

class NoClipButton : public CCNode {
public:
    CCSprite* m_circle = nullptr;

    static NoClipButton* create() {
        auto ret = new NoClipButton();
        if (ret->init()) { ret->autorelease(); return ret; }
        delete ret; return nullptr;
    }

    bool init() {
        if (!CCNode::init()) return false;

        m_circle = CCSprite::create("GJ_button_01.png");
        if (!m_circle) {
            auto draw = CCDrawNode::create();
            draw->drawDot({ 0, 0 }, 20.f, { 0.2f, 0.8f, 0.2f, 0.9f });
            this->addChild(draw);
        } else {
            m_circle->setScale(0.6f);
            this->addChild(m_circle);
        }

        auto label = CCLabelTTF::create("NC", "bigFont.fnt", 14.f);
        label->setPosition({ 0, 0 });
        this->addChild(label, 1);

        auto touchNode = CCMenuItemImage::create(
            "GJ_button_01.png", "GJ_button_02.png",
            this, menu_selector(NoClipButton::onToggle)
        );
        touchNode->setScale(0.6f);

        auto menu = CCMenu::create();
        menu->addChild(touchNode);
        menu->setPosition(CCPointZero);
        this->addChild(menu, 2);

        updateColor();
        return true;
    }

    void onToggle(CCObject*) {
        g_noClip = !g_noClip;
        updateColor();
    }

    void updateColor() {
        if (m_circle) {
            m_circle->setColor(g_noClip ? ccColor3B{0, 255, 0} : ccColor3B{255, 0, 0});
        }
    }
};

struct $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto btn = NoClipButton::create();
        btn->setPosition({ 35.f, 35.f });
        this->addChild(btn, 999);

        return true;
    }
};
