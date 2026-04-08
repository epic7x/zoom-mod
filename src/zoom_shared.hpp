#pragma once

#include <Geode/Geode.hpp>

namespace zoom {
	using namespace geode::prelude;

	struct SceneTargets {
		CCNode* scene = nullptr;
		CCNode* play = nullptr;
		CCNode* pause = nullptr;

		explicit operator bool() const {
			return scene && play;
		}
	};

	SceneTargets findSceneTargets();
	CCSize getViewportSize();
	void clampPlayLayer(CCNode* playLayer);
	void applyZoom(CCNode* playLayer, float delta, CCPoint screenAnchor);
	void resetView(CCNode* playLayer);
	void setPauseMenuVisible(CCNode* pauseLayer, bool visible);
	void togglePauseMenu(CCNode* pauseLayer);
	void panPlayLayer(CCNode* playLayer, CCPoint delta);
}
