#include "settings.hpp"

#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

SettingsManager* SettingsManager::get() {
	static auto* instance = new SettingsManager();
	return instance;
}

void SettingsManager::init() {
	if (m_initialized) {
		return;
	}
	m_initialized = true;

#ifdef GEODE_IS_DESKTOP
	autoHideMenu = Mod::get()->getSettingValue<bool>("auto-hide-menu");
	listenForSettingChanges<bool>("auto-hide-menu", [this](bool value) {
		autoHideMenu = value;
	});

	autoShowMenu = Mod::get()->getSettingValue<bool>("auto-show-menu");
	listenForSettingChanges<bool>("auto-show-menu", [this](bool value) {
		autoShowMenu = value;
	});

	altDisablesZoom = Mod::get()->getSettingValue<bool>("alt-disables-zoom");
	listenForSettingChanges<bool>("alt-disables-zoom", [this](bool value) {
		altDisablesZoom = value;
	});

	zoomSensitivity = Mod::get()->getSettingValue<double>("zoom-sensitivity");
	listenForSettingChanges<double>("zoom-sensitivity", [this](double value) {
		zoomSensitivity = value;
	});
#endif
}
