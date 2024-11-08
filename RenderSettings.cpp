#include "pch.h"
#include "FirstTouchTrainer.h"
#include "RenderingTools/RenderingTools.h"

void FirstTouchTrainer::RenderSettings()
{
	ImGui::TextUnformatted("Plugin to see the speed differential between your car, and the ball!");

	static bool pluginEnabled = *bEnabled;
	static bool sessionTimerEnabled = *sSessionTimerEnabled;
	static bool touchZoneEnabled = *zTouchZoneEnabled;
	static bool shadowEnabled = *tDropShadow;

	/*ImGui::Separator;*/
	if (ImGui::Checkbox("Enable Speed Differential", &pluginEnabled)) {
		*bEnabled = pluginEnabled;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Turn on speed differntial on screen display");
	}
	if (ImGui::Checkbox("Enable SessionTimer", &sessionTimerEnabled)) {
		*sSessionTimerEnabled = sessionTimerEnabled;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Turn on sesison timer on screen display");
	}
	if (ImGui::Checkbox("Enable Touch Zone", &touchZoneEnabled)) {
		*zTouchZoneEnabled = touchZoneEnabled;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Turn on ball touch zone");
	}
	if (ImGui::Checkbox("Enable Drop Shadows", &shadowEnabled)) {
		*tDropShadow = shadowEnabled;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Turn drop shadows on for all text");
	}

	if (pluginEnabled)
	{

		if (ImGui::CollapsingHeader("Speed Display Settings"))
		{
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
				*tXPos = *tXPosDefault;
				*tYPos = *tYPosDefault;
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
		}
	}

	ImGui::TextUnformatted("\n");

	if (sessionTimerEnabled)
	{
		if (ImGui::CollapsingHeader("Session Timer Display Settings"))
		{
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
				*sSessionTimerX = *sSessionTimerXDefault;
				*sSessionTimerY = *sSessionTimerYDefault;
			}

			//Getting color data from user for the session timer
			LinearColor textSessionTimerColor = *sSessionTimerColor / 255;
			if (ImGui::ColorEdit4("Session Timer Color", &textSessionTimerColor.R)) {
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
		}

	}

	ImGui::TextUnformatted("\n");

	if (touchZoneEnabled)
	{
		if (ImGui::CollapsingHeader("Touch Zone Display Settings"))
		{
			bool touchZoneMatchEnabled = *zTouchZoneMatchColor;
			bool centerSphereEnabled = *zTouchZoneSphereEnabled;

			if (ImGui::Checkbox("Enable 3D Sphere", &centerSphereEnabled)) {
				*zTouchZoneSphereEnabled = centerSphereEnabled;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Enables/Disables a 3D sphere in the center of the ball");
			}
			//Getting color data from user for the session timer
			LinearColor touchZoneColor = *zTouchZoneColor / 255;
			if (ImGui::ColorEdit4("Touch Zone Color", &touchZoneColor.R)) {
				*zTouchZoneColor = touchZoneColor * 255;
				if (touchZoneMatchEnabled) { *zTouchZoneMatchColor = false; }
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Set color of the touch zone");
			}
			LinearColor defaultTouchZoneColor(1, 0, 0, 1);
			if (ImGui::Button("Default Colors##3")) {
				*zTouchZoneColor = defaultTouchZoneColor * 255;
				*zTouchZoneMatchColor = false;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Reset the colors to the default value assigned to them");
			}

			if (ImGui::Checkbox("Enable/Disable Match Color", &touchZoneMatchEnabled)) {
				*zTouchZoneMatchColor = touchZoneMatchEnabled;
			}
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("Enable/Disable Enable this to have the drawn circle match the color of the speed indicator within the ranges");
			}
		}
	}
	return;
}

LinearColor FirstTouchTrainer::CanvasColor()
{
	LinearColor Colors;

	float drawVelocity = std::get<0>(firstTouchTrainer());

	//set colors at certain velocity
	if (-150.0f <= drawVelocity && drawVelocity <= 150.0f) {
		Colors = *cGoodColor;
		return Colors;
	}
	else if (-300.0f <= drawVelocity && drawVelocity <= 300.0f) {
		Colors = *cAlrightColor;
		return Colors;
	}
	else {
		Colors = *cBadColor;
		return Colors;
	}
}

void FirstTouchTrainer::RenderFTT(CanvasWrapper canvas)
{
	float drawVelocity = std::get<0>(firstTouchTrainer());

	canvas.SetColor(CanvasColor());

	if (checkConditions() == 1)
	{
		canvas.SetPosition(Vector2{ *tXPos, *tYPos });

		if (gameWrapper->GetbMetric()) {
			canvas.DrawString(toStringPrecision(drawVelocity * .036f, 2) + " KPH", *tTextSize, *tTextSize, *tDropShadow);
		}
		else {
			canvas.DrawString(toStringPrecision(drawVelocity / 44.704f, 2) + " MPH", *tTextSize, *tTextSize, *tDropShadow);
		}

	}
	return;
}

void FirstTouchTrainer::RenderSessionTimer(CanvasWrapper canvas)
{
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

		return;
	}
}

void FirstTouchTrainer::RenderTouchZone(CanvasWrapper canvas)
{
	if (touchZoneCheckConditions() == 1)
	{
		ServerWrapper server = gameWrapper->GetCurrentGameState();
		if (server.IsNull()) { return; }

		CameraWrapper camera = gameWrapper->GetCamera();
		if (camera.IsNull()) { return; }

		BallWrapper ball = server.GetBall();
		if (ball.IsNull()) { return; }

		CarWrapper car = gameWrapper->GetLocalCar();
		if (car.IsNull()) { return; }

		RT::Frustum frust{ canvas, camera };

		Vector v = ball.GetLocation();
		Rotator r = ball.GetRotation();

		Vector Circle_v(v.X, v.Y, v.Z + 89.13);
		Quat no_rot(0.f, 1.f, 0.f, 0.f);
		Quat ball_rot = RotatorToQuat(r);

		float diff = (camera.GetLocation() - v).magnitude();


		canvas.SetColor(*zTouchZoneColor);
		if (*zTouchZoneMatchColor) {
			canvas.SetColor(CanvasColor());
		}

		if (IsBallInAir() == 1) {

			if (diff < 1000.f && *zTouchZoneSphereEnabled) {
				RT::Sphere(v, ball_rot, 2.f).Draw(canvas, frust, camera.GetLocation(), 18);
			}

			float radius = 50.0f;

			Vector loc = v - Circle_v;
			loc = loc + v;

			RT::Circle circ{ loc, no_rot, radius };

			circ.Draw(canvas, frust);
		}
	}

	return;
}