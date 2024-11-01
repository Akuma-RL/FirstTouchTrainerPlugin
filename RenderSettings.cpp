#include "pch.h"
#include "FirstTouchTrainer.h"


void FirstTouchTrainer::RenderSettings()
{
	ImGui::TextUnformatted("Plugin to see the speed differential between your car, and the ball!");

	//enable plugin checkbox
	CVarWrapper enablePlugin = cvarManager->getCvar("FTT_Enable");
	if (!enablePlugin) { return; }
	bool pluginEnabled = enablePlugin.getBoolValue();
	if (ImGui::Checkbox("Enable/Disable OSD", &pluginEnabled)) {
		enablePlugin.setValue(pluginEnabled);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Enable/Disable On Screen Display");
	}

	//text X Position Slider
	ImGui::TextUnformatted("Text Position Sliders");
	CVarWrapper xPosCvar = cvarManager->getCvar("FTT_X_Position");
	if (!xPosCvar) { return; }
	float posX = xPosCvar.getFloatValue();
	if (ImGui::SliderFloat("Horizontal Position", &posX, 0.0, 3840)) {
		xPosCvar.setValue(posX);
	}
	if (ImGui::IsItemHovered()) {
		std::string hoverXText = "X position is " + std::to_string(posX);
		ImGui::SetTooltip(hoverXText.c_str());
	}

	//text Y Position Slider
	CVarWrapper yPosCvar = cvarManager->getCvar("FTT_Y_Position");
	if (!yPosCvar) { return; }
	float posY = yPosCvar.getFloatValue();
	if (ImGui::SliderFloat("Vertical Position", &posY, 0.0, 2160)) {
		yPosCvar.setValue(posY);
	}
	if (ImGui::IsItemHovered()) {
		std::string hoverYText = "Y position is " + std::to_string(posY);
		ImGui::SetTooltip(hoverYText.c_str());
	}

	//text size slider
	ImGui::TextUnformatted("Text Size Slider");
	CVarWrapper textSizeCvar = cvarManager->getCvar("FTT_Text_Size");
	if (!textSizeCvar) { return; }
	int textSize = textSizeCvar.getIntValue();
	if (ImGui::SliderInt("Text Size", &textSize, 1, 10)) {
		textSizeCvar.setValue(textSize);
	}
	if (ImGui::IsItemHovered()) {
		std::string hoverTextSize = "Text Size is " + std::to_string(textSize);
		ImGui::SetTooltip(hoverTextSize.c_str());
	}

	//checkbox for drop shadows
	CVarWrapper enableShadow = cvarManager->getCvar("FTT_Shadow");
	if (!enableShadow) { return; }
	bool shadowEnabled = enableShadow.getBoolValue();
	if (ImGui::Checkbox("Enable/Disable Drop Shadow", &shadowEnabled)) {
		enableShadow.setValue(shadowEnabled);
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Enable/Disable Text Drop Shadows");
	}

	//reset sliders to default
	if (ImGui::Button("Reset Defaults")) {
		xPosCvar.setValue(880.0f);
		yPosCvar.setValue(1030.0f);
		textSizeCvar.setValue(3);
		enableShadow.setValue(true);
	}

	ImGui::TextUnformatted("\n");

	//Getting color data from user for the "good color"
	CVarWrapper goodRange = cvarManager->getCvar("FTT_Good_Range");
	if (!goodRange) { return; }
	LinearColor textGoodColor = goodRange.getColorValue() / 255;
	if (ImGui::ColorEdit4("Good Range Color", &textGoodColor.R)) {
		goodRange.setValue(textGoodColor * 255);
	}
	*goodColor = textGoodColor * 255;
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Set color of the good touch speed range");
	}

	//Getting color data from user for the "alright color"
	CVarWrapper alrightRange = cvarManager->getCvar("FTT_Alright_Range");
	if (!alrightRange) { return; }
	LinearColor textAlrightColor = alrightRange.getColorValue() / 255;
	if (ImGui::ColorEdit4("Alright Range Color", &textAlrightColor.R)) {
		alrightRange.setValue(textAlrightColor * 255);
	}
	*alrightColor = textAlrightColor * 255;
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Set color of the alright touch speed range");
	}

	//Getting color data from user for the "bad color"
	CVarWrapper badRange = cvarManager->getCvar("FTT_Bad_Range");
	if (!badRange) { return; }
	LinearColor textBadColor = badRange.getColorValue() / 255;
	if (ImGui::ColorEdit4("Bad Range Color", &textBadColor.R)) {
		badRange.setValue(textBadColor * 255);
	} 
	*badColor = textBadColor * 255;
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Set color of the bad touch speed range");
	}

	return;
}

void FirstTouchTrainer::Render(CanvasWrapper canvas)
{
	if (checkConditions() == 1) { return; }

	auto screenSize = canvas.GetSize();
	canvas.SetPosition(Vector2{ *XPos, *YPos });

	float drawVelocity = std::get<0>(firstTouchTrainer());

	//set colors at certain velocity
	if (-150.0f <= drawVelocity && drawVelocity <= 150.0f) {
		canvas.SetColor(*goodColor);
	}
	else if (-300.0f <= drawVelocity && drawVelocity <= 300.0f) {
		canvas.SetColor(*alrightColor);
	}
	else {
		canvas.SetColor(*badColor);
	}

	if (gameWrapper->GetbMetric()) {
		canvas.DrawString(toStringPrecision(drawVelocity * .036f, 2) + " KPH", *TextSize, *TextSize, *bDropShadow);
	}
	else {
		canvas.DrawString(toStringPrecision(drawVelocity / 44.704f, 2) + " MPH", *TextSize, *TextSize, *bDropShadow);
	}

}

