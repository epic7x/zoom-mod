#ifdef GEODE_IS_DESKTOP
#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class DesktopZoomController {
public:
	static DesktopZoomController* get();

	void update();
	void togglePauseMenu();
	void onResume();
	void onPause();
	void onScroll(float y, float x);

	void setPanning(bool panning);

private:
	CCPoint m_lastMousePosition = ccp(0.f, 0.f);
	bool m_isPaused = false;
	bool m_isPanning = false;

	DesktopZoomController() = default;

	void updatePanState();
	void resetView();
	void setPauseMenuVisible(bool visible);
	void zoomAroundCursor(float delta);
};
#endif
