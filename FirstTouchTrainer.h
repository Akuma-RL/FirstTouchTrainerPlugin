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

	//text settings
	std::shared_ptr<int>XPos;
	std::shared_ptr<int>YPos;
	std::shared_ptr<int>TextSize;
	std::shared_ptr<bool>bDropShadow;

	//text color settings
	std::shared_ptr<LinearColor>goodColor;
	std::shared_ptr<LinearColor>alrightColor;
	std::shared_ptr<LinearColor>badColor;

public:
	void onLoad() override;
	void onUnload() override;

	virtual void RenderSettings() override;
	void Render(CanvasWrapper canvas);
	std::tuple<float> firstTouchTrainer();

	int checkConditions();
	std::string toStringPrecision(float InValue, int Precision);
};
