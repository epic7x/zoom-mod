#ifdef GEODE_IS_DESKTOP

#include "desktop.hpp"
#include "settings.hpp"
#include "zoom_shared.hpp"

#include <Geode/Geode.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/CCMouseDispatcher.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

#ifdef GEODE_IS_WINDOWS
#include <Windows.h>
#else
#include <objc/message.h>
#endif

using namespace geode::prelude;

DesktopZoomController* DesktopZoomController::get() {
	static auto* instance = new DesktopZoomController();
	return instance;
}

void DesktopZoomController::setPanning(bool panning) {
	m_isPanning = panning;
}

void DesktopZoomController::updatePanState() {
#ifdef GEODE_IS_WINDOWS
	m_isPanning = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
#endif
}

void DesktopZoomController::resetView() {
	auto const targets = zoom::findSceneTargets();
	zoom::resetView(targets.play);
}

void DesktopZoomController::setPauseMenuVisible(bool visible) {
	auto const targets = zoom::findSceneTargets();
	zoom::setPauseMenuVisible(targets.pause, visible);
}

void DesktopZoomController::zoomAroundCursor(float delta) {
	auto const targets = zoom::findSceneTargets();
	if (!targets) return;
	zoom::applyZoom(targets.play, delta, getMousePos());
}

void DesktopZoomController::update() {
	auto const currentMouse = getMousePos();
	auto const mouseDelta = ccp(
		currentMouse.x - m_lastMousePosition.x,
		currentMouse.y - m_lastMousePosition.y
	);
	m_lastMousePosition = currentMouse;

	updatePanState();

	if (!m_isPaused || !m_isPanning) {
		return;
	}

	auto const targets = zoom::findSceneTargets();
	if (!targets) return;
	zoom::panPlayLayer(targets.play, mouseDelta);
}

void DesktopZoomController::togglePauseMenu() {
	if (!m_isPaused) return;
	auto const targets = zoom::findSceneTargets();
	zoom::togglePauseMenu(targets.pause);
}

void DesktopZoomController::onResume() {
	resetView();
	m_isPaused = false;
	m_isPanning = false;
}

void DesktopZoomController::onPause() {
	m_isPaused = true;
	m_isPanning = false;
}

void DesktopZoomController::onScroll(float y, float) {
	if (!m_isPaused) return;

	auto const targets = zoom::findSceneTargets();
	if (!targets) return;

	if (SettingsManager::get()->altDisablesZoom) {
		if (auto* keyboard = CCKeyboardDispatcher::get(); keyboard && keyboard->getAltKeyPressed()) {
			return;
		}
	}

	auto const step = static_cast<float>(SettingsManager::get()->zoomSensitivity * 0.1);
	if (Loader::get()->isModLoaded("prevter.smooth-scroll")) {
		zoomAroundCursor(-y * step * 0.1f);
	}
	else if (y > 0.f) {
		zoomAroundCursor(-step);
	}
	else {
		zoomAroundCursor(step);
	}

	if (y > 0.f) {
		if (SettingsManager::get()->autoShowMenu && targets.play->getScale() <= 1.01f) {
			setPauseMenuVisible(true);
		}
	}
	else if (SettingsManager::get()->autoHideMenu) {
		setPauseMenuVisible(false);
	}
}

class $modify(PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		this->addEventListener(
			KeybindSettingPressedEventV3(Mod::get(), "toggle-menu-keybind"),
			[](Keybind const&, bool down, bool repeat, double) {
				if (down && !repeat) {
					DesktopZoomController::get()->togglePauseMenu();
				}
			}
		);
	}

	void onResume(CCObject* sender) {
		DesktopZoomController::get()->onResume();
		PauseLayer::onResume(sender);
	}

	void onRestart(CCObject* sender) {
		DesktopZoomController::get()->onResume();
		PauseLayer::onRestart(sender);
	}

	void onRestartFull(CCObject* sender) {
		DesktopZoomController::get()->onResume();
		PauseLayer::onRestartFull(sender);
	}

	void onNormalMode(CCObject* sender) {
		DesktopZoomController::get()->onResume();
		PauseLayer::onNormalMode(sender);
	}

	void onPracticeMode(CCObject* sender) {
		DesktopZoomController::get()->onResume();
		PauseLayer::onPracticeMode(sender);
	}
};

class $modify(PlayLayer) {
	void pauseGame(bool value) {
		DesktopZoomController::get()->onPause();
		PlayLayer::pauseGame(value);
	}

	void startGame() {
		DesktopZoomController::get()->onResume();
		PlayLayer::startGame();
	}

	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		DesktopZoomController::get()->onResume();
		return PlayLayer::init(level, useReplay, dontCreateObjects);
	}
};

class $modify(CCScheduler) {
	void update(float dt) {
		DesktopZoomController::get()->update();
		CCScheduler::update(dt);
	}
};

#ifndef GEODE_IS_WINDOWS
void otherMouseDownHook(void* self, SEL sel, void* event) {
	DesktopZoomController::get()->setPanning(true);
	reinterpret_cast<void(*)(void*, SEL, void*)>(objc_msgSend)(self, sel, event);
}

void otherMouseUpHook(void* self, SEL sel, void* event) {
	DesktopZoomController::get()->setPanning(false);
	reinterpret_cast<void(*)(void*, SEL, void*)>(objc_msgSend)(self, sel, event);
}

$execute {
	if (auto hook = ObjcHook::create("EAGLView", "otherMouseDown:", &otherMouseDownHook)) {
		(void) Mod::get()->claimHook(hook.unwrap());
	}

	if (auto hook = ObjcHook::create("EAGLView", "otherMouseUp:", &otherMouseUpHook)) {
		(void) Mod::get()->claimHook(hook.unwrap());
	}
}
#endif

class $modify(CCMouseDispatcher) {
	bool dispatchScrollMSG(float y, float x) {
		DesktopZoomController::get()->onScroll(y, x);
		return CCMouseDispatcher::dispatchScrollMSG(y, x);
	}
};

#endif
