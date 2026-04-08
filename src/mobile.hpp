#ifdef GEODE_IS_MOBILE
#pragma once

#include <Geode/Geode.hpp>
#include <vector>

using namespace geode::prelude;

class AndroidZoomLayer : public CCLayer {
public:
	static AndroidZoomLayer* instance;
	static AndroidZoomLayer* create(CCNode* sceneLayer);

	bool init(CCNode* sceneLayer);
	bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
	void ccTouchMoved(CCTouch* touch, CCEvent* event) override;
	void ccTouchEnded(CCTouch* touch, CCEvent* event) override;
	void ccTouchCancelled(CCTouch* touch, CCEvent* event) override;
	void onBackButton(CCObject* sender);
	void removeTouchEvent(CCTouch* touch, CCEvent* event);

private:
	CCPoint getAnchorPoint(CCTouch* touch1, CCTouch* touch2);

	bool m_isZooming = false;
	CCPoint m_ZoomAnchor = ccp(0, 0);
	CCNode* m_sceneLayer = nullptr;
	CCNode* m_playLayer = nullptr;
	CCNode* m_pauseLayer = nullptr;
	std::vector<CCTouch*> m_touches;
};

#endif
