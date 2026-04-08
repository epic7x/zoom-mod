#include "zoom_shared.hpp"

#include <algorithm>

namespace zoom {
	namespace {
		CCPoint anchorFromScreen(CCNode* playLayer, CCPoint screenAnchor) {
			auto const size = playLayer->getContentSize();
			return ccp(
				screenAnchor.x - size.width / 2.f,
				screenAnchor.y - size.height / 2.f
			);
		}
	}

	SceneTargets findSceneTargets() {
		auto* scene = CCScene::get();
		if (!scene) {
			return {};
		}

		return {
			scene,
			scene->getChildByID("PlayLayer"),
			scene->getChildByID("PauseLayer")
		};
	}

	CCSize getViewportSize() {
		auto* director = CCDirector::sharedDirector();
		return {
			director->getScreenRight() - director->getScreenLeft(),
			director->getScreenTop() - director->getScreenBottom()
		};
	}

	void clampPlayLayer(CCNode* playLayer) {
		if (!playLayer) return;

		auto position = playLayer->getPosition();
		auto const viewport = getViewportSize();
		auto const content = playLayer->getContentSize();
		auto const scale = playLayer->getScale();

		auto const xLimit = (content.width * scale - viewport.width) * 0.5f;
		auto const yLimit = (content.height * scale - viewport.height) * 0.5f;

		position.x = std::clamp(position.x, -xLimit, xLimit);
		position.y = std::clamp(position.y, -yLimit, yLimit);

		playLayer->setPosition(position);
	}

	void applyZoom(CCNode* playLayer, float delta, CCPoint screenAnchor) {
		if (!playLayer) return;

		auto const previousScale = playLayer->getScale();
		auto const nextScale = std::max(
			1.f,
			delta < 0.f ? previousScale / (1.f - delta) : previousScale * (1.f + delta)
		);

		auto const anchor = anchorFromScreen(playLayer, screenAnchor);
		auto const offset = playLayer->getPosition() - anchor;

		playLayer->setPosition(anchor);
		playLayer->setScale(nextScale);
		playLayer->setPosition(anchor + offset * nextScale / previousScale);
		clampPlayLayer(playLayer);
	}

	void resetView(CCNode* playLayer) {
		if (!playLayer) return;
		playLayer->setScale(1.f);
		playLayer->setPosition(ccp(0.f, 0.f));
	}

	void setPauseMenuVisible(CCNode* pauseLayer, bool visible) {
		if (pauseLayer) {
			pauseLayer->setVisible(visible);
		}
	}

	void togglePauseMenu(CCNode* pauseLayer) {
		if (pauseLayer) {
			pauseLayer->setVisible(!pauseLayer->isVisible());
		}
	}

	void panPlayLayer(CCNode* playLayer, CCPoint delta) {
		if (!playLayer) return;
		playLayer->setPosition(playLayer->getPosition() + delta);
		clampPlayLayer(playLayer);
	}
}
