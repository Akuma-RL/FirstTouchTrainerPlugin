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
	//osd enabled/disabled
	std::shared_ptr<bool>bEnabled;

	//global screen size
	std::shared_ptr<float>bScreenSizeX;
	std::shared_ptr<float>bScreenSizeY;

	//text settings
	std::shared_ptr<int>tXPos;
	std::shared_ptr<int>tYPos;
	std::shared_ptr<int>tTextSize;
	std::shared_ptr<bool>tDropShadow;

	std::shared_ptr<float>tXPosDefault;
	std::shared_ptr<float>tYPosDefault;

	//text color settings
	std::shared_ptr<LinearColor>cGoodColor;
	std::shared_ptr<LinearColor>cAlrightColor;
	std::shared_ptr<LinearColor>cBadColor;

	//session enabled
	std::shared_ptr<bool>sSessionTimerEnabled;
	std::shared_ptr<int>sSessionTimerX;
	std::shared_ptr<int>sSessionTimerY;
	std::shared_ptr<LinearColor>sSessionTimerColor;


	std::shared_ptr<float>sSessionTimerXDefault;
	std::shared_ptr<float>sSessionTimerYDefault;

	std::shared_ptr<bool>zTouchZoneEnabled;
	std::shared_ptr<LinearColor>zTouchZoneColor;

	std::vector<Vector> circles;


public:
	void onLoad() override;
	void onUnload() override;

	void OnFreeplayLoad(std::string eventName);
	void OnFreeplayDestroy(std::string eventName);

	virtual void RenderSettings() override;

	void RenderFTT(CanvasWrapper canvas);
	void RenderSessionTimer(CanvasWrapper canvas);
	void RenderTouchZone(CanvasWrapper canvas);
	std::tuple<float> firstTouchTrainer();

	int checkConditions();
	std::string toStringPrecision(float InValue, int Precision);

	int timerCheckConditions();

	int touchZoneCheckConditions();
};

class Circle2D
{
public:
	Vector2F location;
	float radius;
	int steps;
	float lineThickness;

	// CONSTRUCTORS
	Circle2D();
	Circle2D(Vector2F loc, float rad);
	Circle2D(Vector2F loc, float rad, int step);
	Circle2D(Vector2F loc, float rad, int step, float lineThicc);

	// FUNCTIONS
	void Draw(CanvasWrapper canvas) const;
};