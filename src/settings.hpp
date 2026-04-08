#pragma once

class SettingsManager {
public:
	static SettingsManager* get();
	void init();

#ifdef GEODE_IS_DESKTOP
	bool autoHideMenu = true;
	bool autoShowMenu = true;
	bool altDisablesZoom = true;
	double zoomSensitivity = 1.0;
#endif

private:
	bool m_initialized = false;
};
