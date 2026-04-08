#include <Geode/Geode.hpp>

#include "settings.hpp"

$execute {
	geode::log::info("Zoom mod loaded on {}", GEODE_PLATFORM_NAME);
	SettingsManager::get()->init();
}
