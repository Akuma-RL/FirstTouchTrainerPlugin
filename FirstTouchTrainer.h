#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class FirstTouchTrainer : public BakkesMod::Plugin::BakkesModPlugin
	, public SettingsWindowBase
{
protected:

	////////////////////////////////////////////
	//////////////////CVARS/////////////////////
	////////////////////////////////////////////
	
	//ENABLE FEATURES
	std::shared_ptr<bool>bEnabled;
	std::shared_ptr<bool>sSessionTimerEnabled;
	std::shared_ptr<bool>zTouchZoneEnabled;

	//GET SCREEN SIZE
	std::shared_ptr<float>bScreenSizeX;
	std::shared_ptr<float>bScreenSizeY;

	//FTT SETTINGS
	std::shared_ptr<int>tXPos;
	std::shared_ptr<int>tYPos;
	std::shared_ptr<int>tTextSize;
	std::shared_ptr<bool>tDropShadow;
	//FTT DEFAULT SETTINGS
	std::shared_ptr<float>tXPosDefault;
	std::shared_ptr<float>tYPosDefault;
	//FTT COLOR SETTINGS
	std::shared_ptr<LinearColor>cGoodColor;
	std::shared_ptr<LinearColor>cAlrightColor;
	std::shared_ptr<LinearColor>cBadColor;

	//SESSION TIMER SETTINGS
	std::shared_ptr<int>sSessionTimerX;
	std::shared_ptr<int>sSessionTimerY;
	//SESSION TIMER DEFAULTS
	std::shared_ptr<float>sSessionTimerXDefault;
	std::shared_ptr<float>sSessionTimerYDefault;
	//SESSION TIMER COLOR
	std::shared_ptr<LinearColor>sSessionTimerColor;

	//TOUCH ZONE SETTINGS
	std::shared_ptr<bool>zTouchZoneSphereEnabled;
	std::shared_ptr<bool>zTouchZoneMatchColor;
	//TOUCH ZONE COLOR
	std::shared_ptr<LinearColor>zTouchZoneColor;

	//GET FTT DRAWN COLOR
	std::shared_ptr<LinearColor>gDrawnColor;
	////////////////////////////////////////////

	std::vector<Vector> circles;

public:

	void onLoad() override;
	void onUnload() override;

	void OnFreeplayLoad(std::string eventName);
	void OnFreeplayDestroy(std::string eventName);
	std::tuple<float, float, float, float> firstTouchTrainer();

	virtual void RenderSettings() override;

	void RenderFTT(CanvasWrapper canvas);
	void RenderSessionTimer(CanvasWrapper canvas);
	void RenderTouchZone(CanvasWrapper canvas);

	LinearColor CanvasColor();
	
	int checkConditions();
	int timerCheckConditions();
	int touchZoneCheckConditions();
	float IsBallInAir();

	std::string toStringPrecision(float InValue, int Precision);
};