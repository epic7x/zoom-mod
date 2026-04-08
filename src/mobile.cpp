#ifdef GEODE_IS_MOBILE

#include "mobile.hpp"
#include "zoom_shared.hpp"

#include <algorithm>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

AndroidZoomLayer* AndroidZoomLayer::instance = nullptr;

AndroidZoomLayer* AndroidZoomLayer::create(CCNode* sceneLayer) {
	if (instance) {
		return instance;
	}

	auto* layer = new AndroidZoomLayer();
	if (layer && layer->init(sceneLayer)) {
		layer->autorelease();
		instance = layer;
		return layer;
	}

	delete layer;
	return nullptr;
}

bool AndroidZoomLayer::init(CCNode* sceneLayer) {
	if (!CCLayer::init()) return false;
	if (!sceneLayer) return false;
	if (sceneLayer->getChildByID("AndroidZoomLayer"_spr)) return false;

	m_sceneLayer = sceneLayer;
	m_sceneLayer->addChild(this);

	auto const targets = zoom::findSceneTargets();
	m_playLayer = targets.play;
	m_pauseLayer = targets.pause;

	if (!m_playLayer || !m_pauseLayer) {
		return false;
	}

	zoom::setPauseMenuVisible(m_pauseLayer, false);

	auto* backMenu = CCMenu::create();
	backMenu->ignoreAnchorPointForPosition(false);
	backMenu->setContentSize(ccp(0, 0));
	backMenu->setPosition(ccp(0, CCDirector::get()->getWinSize().height));
	backMenu->setID("back-menu");
	this->addChild(backMenu);

	auto* backSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	backSprite->setOpacity(100);

	auto* backButton = CCMenuItemSpriteExtra::create(backSprite, this, menu_selector(AndroidZoomLayer::onBackButton));
	backButton->setPosition(ccp(24, -23));
	backButton->setSizeMult(1.15f);
	backMenu->addChild(backButton);

	this->setID("AndroidZoomLayer"_spr);
	this->setZOrder(11);

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -250, true);
	this->setTouchEnabled(true);
	backMenu->setTouchPriority(CCDirector::sharedDirector()->getTouchDispatcher()->getTargetPrio());
	CCDirector::sharedDirector()->getTouchDispatcher()->registerForcePrio(backMenu, 2);
	return true;
}

void AndroidZoomLayer::onBackButton(CCObject*) {
	zoom::resetView(m_playLayer);
	zoom::setPauseMenuVisible(m_pauseLayer, true);
	this->removeFromParentAndCleanup(true);
	AndroidZoomLayer::instance = nullptr;
}

bool AndroidZoomLayer::ccTouchBegan(CCTouch* touch, CCEvent*) {
	m_touches.push_back(touch);
	if (m_touches.size() > 1 && !m_isZooming) {
		m_isZooming = true;
		m_ZoomAnchor = getAnchorPoint(m_touches[0], m_touches[1]);
	}
	return true;
}

void AndroidZoomLayer::ccTouchMoved(CCTouch* touch, CCEvent*) {
	if (m_touches.size() == 1) {
		zoom::panPlayLayer(m_playLayer, m_touches[0]->getDelta());
		return;
	}

	if (!m_isZooming) return;

	auto* movingTouch = m_touches[0];
	auto* anchoredTouch = m_touches[1];

	if (touch != movingTouch && touch != anchoredTouch) return;
	if (touch == anchoredTouch) {
		movingTouch = m_touches[1];
		anchoredTouch = m_touches[0];
	}

	auto const newAnchor = getAnchorPoint(movingTouch, anchoredTouch);
	auto const anchorDelta = ccpSub(newAnchor, m_ZoomAnchor);
	m_ZoomAnchor = newAnchor;

	zoom::panPlayLayer(m_playLayer, anchorDelta);

	auto const moveDelta = movingTouch->getDelta();
	auto touchDisplacement = ccpSub(movingTouch->getLocation(), anchoredTouch->getLocation());
	float const scaleDelta = touchDisplacement.normalize().dot(moveDelta) / 100.f;

	zoom::applyZoom(m_playLayer, scaleDelta, m_ZoomAnchor);
}

void AndroidZoomLayer::ccTouchEnded(CCTouch* touch, CCEvent* event) {
	removeTouchEvent(touch, event);
}

void AndroidZoomLayer::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
	removeTouchEvent(touch, event);
}

void AndroidZoomLayer::removeTouchEvent(CCTouch* touch, CCEvent*) {
	m_touches.erase(std::remove(m_touches.begin(), m_touches.end(), touch), m_touches.end());

	if (m_touches.size() < 2) {
		m_isZooming = false;
		m_ZoomAnchor = ccp(0, 0);
	}
}

CCPoint AndroidZoomLayer::getAnchorPoint(CCTouch* movingTouch, CCTouch* anchoredTouch) {
	auto const moving = movingTouch->getLocation();
	auto const anchored = anchoredTouch->getLocation();
	return ccp((moving.x + anchored.x) / 2, (moving.y + anchored.y) / 2);
}

class $modify(AndroidZoomPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		if (auto* rightButtonMenu = getChildByID("right-button-menu")) {
			auto* sprite = CircleButtonSprite::createWithSprite(
				"zoom_button.png"_spr,
				1.0f,
				CircleBaseColor::Green,
				CircleBaseSize::MediumAlt
			);
			sprite->getTopNode()->setScale(1.0f);
			sprite->setScale(0.6f);

			auto* button = CCMenuItemSpriteExtra::create(
				sprite,
				this,
				menu_selector(AndroidZoomPauseLayer::onZoomButton)
			);
			button->setID("zoom-button"_spr);

			rightButtonMenu->addChild(button);
			rightButtonMenu->updateLayout();
		}
	}

	void onZoomButton(CCObject*) {
		AndroidZoomLayer::create(this->getParent());
	}
};

#endif
