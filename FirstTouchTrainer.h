#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include<iostream>
#include<string>
#include<chrono>
#include<tuple>
#include<format>
#include<fstream>
#include<chrono>
#include<thread>

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

struct Color
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char A;
};
struct Popup
{
	std::string popupText = "";
	Color color = { 255, 255, 255, 255 };
	Vector2 startLocation = { -1, -1 };
};
struct Time
{
	std::string time = "";
	float normalTime = -1;
};

class FirstTouchTrainer : public BakkesMod::Plugin::BakkesModPlugin
	, public SettingsWindowBase
{
protected:
	std::shared_ptr<bool>bEnabled;
	std::shared_ptr<bool>bDropShadow;
	std::shared_ptr<bool>speedLog;


	std::shared_ptr<int>XPos;
	std::shared_ptr<int>YPos;
	std::shared_ptr<int>TextSize;

	std::shared_ptr<float>minSpeed;
	std::vector<float> measurements;
	float old_speed_sq = -1.0f;

	std::shared_ptr<LinearColor>goodColor;
	std::shared_ptr<LinearColor>alrightColor;
	std::shared_ptr<LinearColor>badColor;
	std::shared_ptr<LinearColor>gDrawnColor;

public:
	void onLoad() override;
	void onUnload() override;

	virtual void RenderSettings() override;
	void Render(CanvasWrapper canvas);
	std::tuple<float> firstTouchTrainer();

	void onBallHit(std::string eventName);
	void Tick();

	int checkConditions();
	std::string toStringPrecision(float InValue, int Precision);
};
