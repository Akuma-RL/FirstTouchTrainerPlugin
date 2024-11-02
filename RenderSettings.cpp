#include "pch.h"
#include "FirstTouchTrainer.h"


void FirstTouchTrainer::RenderSettings()
{
	ImGui::TextUnformatted("Plugin to see the speed differential between your car, and the ball!");

	//enable plugin checkbox
	bool pluginEnabled = *bEnabled;
	if (ImGui::Checkbox("Enable/Disable OSD", &pluginEnabled)) {
		*bEnabled = pluginEnabled;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Enable/Disable On Screen Display");
	}
	if (pluginEnabled)
	{
		//checkbox for drop shadows
		bool shadowEnabled = *tDropShadow;
		if (ImGui::Checkbox("Enable/Disable Drop Shadow", &shadowEnabled)) {
			*tDropShadow = shadowEnabled;
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Enable/Disable Text Drop Shadows");
		}

		if (ImGui::TreeNode("Speed Display Settings"))
		{
			static bool closable_group = true;

			//text size slider
			ImGui::TextUnformatted("Text Size Slider");

			int textSize = *tTextSize;
			if (ImGui::SliderInt("Text Size", &textSize, 1, 10)) {
				*tTextSize = textSize;
			}
			if (ImGui::IsItemHovered()) {
				std::string hoverTextSize = "Text Size is " + std::to_string(textSize);
				ImGui::SetTooltip(hoverTextSize.c_str());
			}

			//Speed text X Position Slider
			ImGui::TextUnformatted("Speed Text Position");
			float posX = *tXPos;
			if (ImGui::SliderFloat("Horizontal Position", &posX, 0.0f, *bScreenSizeX)) {
				*tXPos = posX;
			}
			if (ImGui::IsItemHovered()) {
				std::string hoverXText = "Speed Text X position is " + std::to_string(posX);
				ImGui::SetTooltip(hoverXText.c_str());
			}

			//Speed text Y Position Slider
			float posY = *tYPos;
			if (ImGui::SliderFloat("Vertical Position", &posY, 0.0f, *bScreenSizeY)) {
				*tYPos = posY;
			}
			if (ImGui::IsItemHovered()) {
				std::string hoverYText = "Speed Text Y position is " + std::to_string(posY);
				ImGui::SetTooltip(hoverYText.c_str());
			}
			int textDefaultSize = 3;
			bool textDefaultDropShadow = true;

			//reset sliders to default
			if (ImGui::Button("Reset Defaults")) {
				*tXPos = *tDefaultXPos;
				*tYPos = *tDefaultYPos;
				*tTextSize = textDefaultSize;
				*tDropShadow = textDefaultDropShadow;
			}

			//Getting color data from user for the "good color"
			LinearColor textGoodColor = *cGoodColor / 255;
			if (ImGui::ColorEdit4("Good Range Color", &textGoodColor.R)) {
				*cGoodColor = textGoodColor * 255;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Set color of the good touch speed range");
			}

			//Getting color data from user for the "alright color"
			LinearColor textAlrightColor = *cAlrightColor / 255;
			if (ImGui::ColorEdit4("Alright Range Color", &textAlrightColor.R)) {
				*cAlrightColor = textAlrightColor * 255;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Set color of the alright touch speed range");
			}

			//Getting color data from user for the "bad color"
			LinearColor textBadColor = *cBadColor / 255;
			if (ImGui::ColorEdit4("Bad Range Color", &textBadColor.R)) {
				*cBadColor = textBadColor * 255;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Set color of the bad touch speed range");
			}

			LinearColor defaultGoodColor(0, 1, 0, 1);
			LinearColor defaultAlrightColor(1, 1, 0, 1);
			LinearColor defaultBadColor(1, 0, 0, 1);

			if (ImGui::Button("Default Colors")) {
				*cGoodColor = defaultGoodColor * 255;
				*cAlrightColor = defaultAlrightColor * 255;
				*cBadColor = defaultBadColor * 255;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Reset the colors to the default value assigned to them");
			}
			ImGui::TreePop();
		}
	}

	ImGui::TextUnformatted("\n");

	//enable session timer
	bool sessionTimerEnabled = *sSessionTimerEnabled;
	if (ImGui::Checkbox("Enable/Disable Session Timer", &sessionTimerEnabled)) {
		*sSessionTimerEnabled = sessionTimerEnabled;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Enable/Disable Session Timer");
	}
	if (sessionTimerEnabled)
	{
		if (ImGui::TreeNode("Session Timer Display Settings"))
		{
			static bool closable_group = true;



			float sessionXPos = *sSessionTimerX;
			if (ImGui::SliderFloat("Horizontal Position##2", &sessionXPos, 0.0f, *bScreenSizeX)) {
				*sSessionTimerX = sessionXPos;
			}
			if (ImGui::IsItemHovered()) {
				std::string timerXText = "Session timer X position is " + std::to_string(sessionXPos);
				ImGui::SetTooltip(timerXText.c_str());
			}

			float sessionYPos = *sSessionTimerY;
			if (ImGui::SliderFloat("Vertical Position##2", &sessionYPos, 0.0f, *bScreenSizeY)) {
				*sSessionTimerY = sessionYPos;
			}
			if (ImGui::IsItemHovered()) {
				std::string timerYText = "Session timer Y position is " + std::to_string(sessionYPos);
				ImGui::SetTooltip(timerYText.c_str());
			}
			//reset sliders to default
			if (ImGui::Button("Reset Defaults##2")) {
				*sSessionTimerX = *sDefaultXPos;
				*sSessionTimerY = *sDefaultYPos;
			}

			//Getting color data from user for the session timer
			LinearColor textSessionTimerColor = *sSessionTimerColor / 255;
			if (ImGui::ColorEdit4("Sesion Timer Color", &textSessionTimerColor.R)) {
				*sSessionTimerColor = textSessionTimerColor * 255;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Set color of the session timer");
			}
			LinearColor defaultSessionTimercolor(1, 1, 1, 1);
			if (ImGui::Button("Default Colors##2")) {
				*sSessionTimerColor = defaultSessionTimercolor * 255;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Reset the colors to the default value assigned to them");
			}

			ImGui::TreePop();
		}
	}
	return;
}


void FirstTouchTrainer::Render(CanvasWrapper canvas)
{
	if (checkConditions() == 1)
	{

		canvas.SetPosition(Vector2{ *tXPos, *tYPos });

		float drawVelocity = std::get<0>(firstTouchTrainer());

		//set colors at certain velocity
		if (-150.0f <= drawVelocity && drawVelocity <= 150.0f) {
			canvas.SetColor(*cGoodColor);
		}
		else if (-300.0f <= drawVelocity && drawVelocity <= 300.0f) {
			canvas.SetColor(*cAlrightColor);
		}
		else {
			canvas.SetColor(*cBadColor);
		}

		if (gameWrapper->GetbMetric()) {
			canvas.DrawString(toStringPrecision(drawVelocity * .036f, 2) + " KPH", *tTextSize, *tTextSize, *tDropShadow);
		}
		else {
			canvas.DrawString(toStringPrecision(drawVelocity / 44.704f, 2) + " MPH", *tTextSize, *tTextSize, *tDropShadow);
		}
	}

	//check if session timer is enabled
	if (timerCheckConditions() == 1)
	{

		//get seconds elapsed
		auto server = gameWrapper->GetGameEventAsServer();
		int startSessionTimer = server.GetSecondsElapsed();

		//set session timer color
		canvas.SetColor(*sSessionTimerColor);

		//convert timer to int
		int t = startSessionTimer;

		//3600 seconds in an hour
		t %= 3600;

		//divide by 60 to get 60 minutes in the hour
		int timerMinutes = t / 60;

		//eveery 60 seconds convert to 1 minute
		t %= 60;
		int timerSeconds = t;

		char timerBuffer[50];
		sprintf_s(timerBuffer, "%dm%ds", timerMinutes, timerSeconds);
		std::string timerText = timerBuffer;

		canvas.SetPosition(Vector2{ *sSessionTimerX, *sSessionTimerY });
		canvas.DrawString(timerText, *tTextSize, *tTextSize, *tDropShadow);
	}
	return;
}