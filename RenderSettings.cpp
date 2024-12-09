#define _USE_MATH_DEFINES
#include "pch.h"
#include "FirstTouchTrainer.h"
#include "RenderingTools/RenderingTools.h"
#include <iostream>
#include <math.h>

void FirstTouchTrainer::RenderSettings()
{
	ImGui::TextUnformatted("Plugin to see the speed differential between your car, and the ball!");

	static bool pluginEnabled = *bEnabled;
	static bool sessionTimerEnabled = *sSessionTimerEnabled;
	static bool touchZoneEnabled = *zTouchZoneEnabled;
	static bool shadowEnabled = *tDropShadow;

	static bool centerSphereEnabled = *zTouchZoneSphereEnabled;


	/*ImGui::Separator;*/
	if (ImGui::Checkbox("Enable Speed Differential", &pluginEnabled)) {
		*bEnabled = pluginEnabled;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Turn on speed differntial on screen display");
	}
	if (ImGui::Checkbox("Enable Drop Shadows", &shadowEnabled)) {
		*tDropShadow = shadowEnabled;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Turn drop shadows on for all text");
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
	if (ImGui::Checkbox("Enable Center Sphere", &centerSphereEnabled)) {
		*zTouchZoneSphereEnabled = centerSphereEnabled;
	}
	if (ImGui::IsItemHovered()) {
		ImGui::SetTooltip("Turn on ball center sphere");
	}

	if (ImGui::CollapsingHeader("Speed Display Settings"))
	{
		static bool xyEnabled = *bXYZEnabled;

		if (ImGui::Checkbox("Enable XYZ Velocity ONLY", &xyEnabled)) {
			*bEnabled = xyEnabled;
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Include calculation of Z velocity for speed differential");
		}

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
	}

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
		if (ImGui::ColorEdit4("Session Timer Color", &textSessionTimerColor.R, 1 << 5)) {
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

	if (ImGui::CollapsingHeader("Touch Zone Display Settings"))
	{
		index = std::make_shared<int>(0);
		bool touchZoneColorMatchEnabled = *zTouchZoneMatchColor;
		float touchZoneCircleRadius = *zTouchZoneCircleRadius;

		const char* items[] = { "Default", "Velocity Driven", "Keep it Up" };
		static int item_selected_idx = 0; // Here we store our selection data as an index.

		// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
		const char* combo_preview_value = items[item_selected_idx];

		*index = item_selected_idx;
		int startingValue = 0;
		item_selected_idx = startingValue;
		if (ImGui::BeginCombo("Touch Zone Behavior", combo_preview_value, ImGuiComboFlags_HeightSmall))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_selected_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_selected_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		if (ImGui::SliderFloat("Circle Radius", &touchZoneCircleRadius, 30.f, 60.f)) {
			*zTouchZoneCircleRadius = touchZoneCircleRadius;
		}
		if (ImGui::IsItemHovered()) {
			std::string hoverCircleRadius = "Circle Radius is " + std::to_string(touchZoneCircleRadius);
			ImGui::SetTooltip(hoverCircleRadius.c_str());
		}
		int touchZoneCircleThicc = *zTouchZoneCircleThicc;
		if (ImGui::SliderInt("Circle Thickness", &touchZoneCircleThicc, 1.f, 5.f)) {
			*zTouchZoneCircleThicc = touchZoneCircleThicc;
		}
		if (ImGui::IsItemHovered()) {
			std::string hoverCircleThicc = "Circle Thickness is " + std::to_string(touchZoneCircleThicc);
			ImGui::SetTooltip(hoverCircleThicc.c_str());
		}

		float touchZoneCircleRadiusDefault = 47.f;
		int touchZoneCircleThiccDefault = 1;
		//reset sliders to default
		if (ImGui::Button("Reset Defaults##3")) {
			*zTouchZoneCircleRadius = touchZoneCircleRadiusDefault;
			*zTouchZoneCircleThicc = touchZoneCircleThiccDefault;
		}

		//Getting color data from user for the session timer
		LinearColor touchZoneColor = *zTouchZoneColor / 255;
		if (ImGui::ColorEdit4("Touch Zone Color", &touchZoneColor.R, 1 << 5)) {
			*zTouchZoneColor = touchZoneColor * 255;
			if (touchZoneColorMatchEnabled) { *zTouchZoneMatchColor = false; }
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Set color of the touch zone");
		}
		LinearColor defaultTouchZoneColor(1, 0, 0, 1);
		if (ImGui::Button("Default Colors##3")) {
			*zTouchZoneColor = defaultTouchZoneColor * 255;
			*zTouchZoneMatchColor = true;
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Reset the colors to the default value assigned to them");
		}
		if (ImGui::Checkbox("Touch Zone Match Color", &touchZoneColorMatchEnabled)) {
			*zTouchZoneMatchColor = touchZoneColorMatchEnabled;
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Enable/Disable Enable this to have the drawn circle match the color of the speed indicator within the ranges");
		}
	}

	if (ImGui::CollapsingHeader("Center Sphere Settings"))
	{

		bool touchZoneSphereColorMatchEnabled = *zTouchZoneSphereMatchColor;
		float touchZoneSphereRadius = *zTouchZoneSphereRadius;

		if (ImGui::SliderFloat("Sphere Radius", &touchZoneSphereRadius, 1.f, 10.f)) {
			*zTouchZoneSphereRadius = touchZoneSphereRadius;
		}
		if (ImGui::IsItemHovered()) {
			std::string hoverSphereRadius = "Sphere Radius is " + std::to_string(touchZoneSphereRadius);
			ImGui::SetTooltip(hoverSphereRadius.c_str());
		}

		float touchZoneSphereRadiusDefault = 5.f;
		if (ImGui::Button("Reset Defaults##4")) {
			*zTouchZoneSphereRadius = touchZoneSphereRadiusDefault;
		}

		//Getting color data from user for the session timer
		LinearColor touchZoneSphereColor = *zTouchZoneSphereColor / 255;
		if (ImGui::ColorEdit4("Sphere Color", &touchZoneSphereColor.R, 1 << 5)) {
			*zTouchZoneSphereColor = touchZoneSphereColor * 255;
			if (touchZoneSphereColorMatchEnabled) { *zTouchZoneSphereMatchColor = false; }
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Set color of the sphere");
		}
		LinearColor defaultTouchZoneSphereColor(1, 0, 0, 1);
		if (ImGui::Button("Default Colors##4")) {
			*zTouchZoneSphereColor = defaultTouchZoneSphereColor * 255;
			*zTouchZoneSphereMatchColor = true;
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Reset the colors to the default value assigned to them");
		}

		if (ImGui::Checkbox("Sphere Match Color", &touchZoneSphereColorMatchEnabled)) {
			*zTouchZoneSphereMatchColor = touchZoneSphereColorMatchEnabled;
		}
		if (ImGui::IsItemHovered()) {
			ImGui::SetTooltip("Enable/Disable Enable this to have the drawn sphere match the color of the speed indicator within the ranges");
		}
	}
	if (ImGui::CollapsingHeader("Speed Zone Color Options"))
	{

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

	if (*bEnabled)
	{
		if (!gameWrapper->IsInGame()) { return; }
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
	if (*sSessionTimerEnabled)
	{
		if (!gameWrapper->IsInGame()) { return; }
		//get seconds elapsed
		ServerWrapper server = gameWrapper->GetGameEventAsServer();
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
	int ingame = (gameWrapper->IsInGame()) ? 1 : (gameWrapper->IsInReplay()) ? 2 : 0;
	if (*zTouchZoneEnabled && ingame)
	{
		if (gameWrapper->IsInOnlineGame() && ingame != 2) { return; }

		ServerWrapper server = (ingame == 1) ? gameWrapper->GetGameEventAsServer() : gameWrapper->GetGameEventAsReplay();
		if (server.IsNull()) { return; }

		CameraWrapper camera = gameWrapper->GetCamera();
		if (camera.IsNull()) { return; }

		BallWrapper ball = server.GetBall();
		if (ball.IsNull()) { return; }

		CarWrapper car = gameWrapper->GetLocalCar();
		if (car.IsNull()) { return; }

		RT::Frustum frust{ canvas, camera };

		Vector ballLocation = ball.GetLocation();
		Rotator ballRotation = ball.GetRotation();
		Quat ballRotQuat = RotatorToQuat(ballRotation);
		float ballRadius = ball.GetRadius();
		Vector ballVelocity = ball.GetVelocity();
		Vector ballVelocityNormalized = ballVelocity.getNormalized();

		float ballSpeed = ballVelocity.magnitude();

		Vector oppositeDirection(-ballVelocityNormalized.X, -ballVelocityNormalized.Y, -ballVelocityNormalized.Z);
		Vector sameDirection(ballVelocityNormalized.X, ballVelocityNormalized.Y, ballVelocityNormalized.Z);

		float minSpeed = 800;
		float maxSpeed = 2100;

		float minTiltDegrees = 0.f;
		float maxTiltDegrees = .75f;

		float minPosition = 0.f;
		float maxPosition = 40.f;
		float maxZPosition = 29.71f;

		float tiltAngle = 0.0f;

		float angleToOppositeDir = atan2(oppositeDirection.Y, oppositeDirection.X);
		float angleToSameDir = atan2(sameDirection.Y, sameDirection.X);
		Vector staticOffset(ballLocation.X, ballLocation.Y, ballLocation.Z + 89.13);
		Vector offset(0.f, 0.f, -89.13);

		float xPosition = 0.0f;
		float yPosition = 0.0f;
		float zPosition = 0.0f;

		if (*index == 1) {

			if (ballSpeed > minSpeed) {
				float normalizedSpeed = (ballSpeed - minSpeed) / (maxSpeed - minSpeed);

				offset = Vector(cos(angleToOppositeDir) * ballRadius, sin(angleToOppositeDir) * ballRadius, -89.13);

				tiltAngle = minTiltDegrees + (maxTiltDegrees - minTiltDegrees) * normalizedSpeed;

				xPosition = minPosition + (maxPosition - minPosition) * normalizedSpeed;
				yPosition = minPosition + (maxPosition - minPosition) * normalizedSpeed;
				zPosition = minPosition + (maxPosition - minPosition) * normalizedSpeed;
			}
		}

		if (*index == 2) {
			if (ballSpeed > minSpeed) {
				float normalizedSpeed = (ballSpeed - minSpeed) / (maxSpeed - minSpeed);

				offset = Vector(cos(angleToSameDir) * ballRadius, sin(angleToSameDir) * ballRadius, -89.13);

				tiltAngle = minTiltDegrees + (maxTiltDegrees - minTiltDegrees) * normalizedSpeed;

				xPosition = minPosition + (maxPosition - minPosition) * normalizedSpeed;
				yPosition = minPosition + (maxPosition - minPosition) * normalizedSpeed;
				zPosition = minPosition + (maxPosition - minPosition) * normalizedSpeed;
			}
		}

		tiltAngle = std::clamp(tiltAngle, minTiltDegrees, maxTiltDegrees);

		xPosition = std::clamp(xPosition, minPosition, maxPosition);
		yPosition = std::clamp(yPosition, minPosition, maxPosition);
		zPosition = std::clamp(zPosition, minPosition, maxZPosition);

		float pitch = tiltAngle * oppositeDirection.Y;
		float yaw = tiltAngle * oppositeDirection.X;
		float roll = tiltAngle * oppositeDirection.X * oppositeDirection.Y;

		float x = xPosition * ballVelocityNormalized.X;
		float y = yPosition * ballVelocityNormalized.Y;
		float z = zPosition * ballVelocityNormalized.X * ballVelocityNormalized.Y;


		Quat circleRotation = FirstTouchTrainer::fromEuler(pitch, -yaw, roll);
		Quat circleEDRotation = FirstTouchTrainer::fromEuler(-pitch, yaw, -roll);
		Quat noRotation(0.f, 1.f, 0.f, 0.f);
		Vector circlePosition(x, y, z);

		canvas.SetColor(*zTouchZoneColor);
		if (*zTouchZoneMatchColor) {
			canvas.SetColor(CanvasColor());
		}

		if (IsBallInAir() == 1) {
			Vector circleStaticLocation = ballLocation - staticOffset;
			circleStaticLocation = circleStaticLocation + ballLocation;

			if (*index == 0) {
				RT::Circle circ{ circleStaticLocation, noRotation, *zTouchZoneCircleRadius, *zTouchZoneCircleThicc, 1, 32 };

				if (car.GetLocation().Z < ballLocation.Z) {
					circ.Draw(canvas, frust);
				}
			}

			Vector circleDynamicLocation = ballLocation + offset;
			circleDynamicLocation = circleDynamicLocation + circlePosition;

			if (*index == 1) {
				RT::Circle circ{ circleDynamicLocation, circleRotation.normalize(), *zTouchZoneCircleRadius, *zTouchZoneCircleThicc, 1, 32 };

				if (car.GetLocation().Z < ballLocation.Z) {
					circ.Draw(canvas, frust);
				}
			}
			Vector circleEDLocation = ballLocation + offset;
			circleEDLocation = circleEDLocation + circlePosition;

			if (*index == 2)
			{
				RT::Circle circ{ circleEDLocation, circleEDRotation.normalize(), *zTouchZoneCircleRadius, *zTouchZoneCircleThicc, 1, 32 };

				if (car.GetLocation().Z < ballLocation.Z) {
					circ.Draw(canvas, frust);
				}
			}
		}
	}
	return;
}

void FirstTouchTrainer::RenderSphere(CanvasWrapper canvas)
{
	if (*zTouchZoneSphereEnabled)
	{
		if (!gameWrapper->IsInGame()) { return; }

		ServerWrapper server = gameWrapper->GetCurrentGameState();
		if (server.IsNull()) { return; }

		CameraWrapper camera = gameWrapper->GetCamera();
		if (camera.IsNull()) { return; }

		BallWrapper ball = server.GetBall();
		if (ball.IsNull()) { return; }

		CarWrapper car = gameWrapper->GetLocalCar();
		if (car.IsNull()) { return; }

		RT::Frustum frust{ canvas, camera };

		Vector ballLocation = ball.GetLocation();
		Rotator ballRotation = ball.GetRotation();

		Quat ballRotQuat = RotatorToQuat(ballRotation);

		canvas.SetColor(*zTouchZoneSphereColor);
		if (*zTouchZoneSphereMatchColor) {
			canvas.SetColor(CanvasColor());
		}

		if (IsBallInAir() == 1) {

			if (*zTouchZoneSphereEnabled) {
				RT::Sphere(ballLocation, ballRotQuat.normalize(), *zTouchZoneSphereRadius).Draw(canvas, frust, camera.GetLocation(), 18);
			}
		}
	}
	return;
}

Quat FirstTouchTrainer::fromEuler(float x, float y, float z)
{
	x = x * 0.5;
	y = y * 0.5;
	z = z * 0.5;

	float cX = cosf(x);
	float cY = cosf(y);
	float cZ = cosf(z);

	float sX = sinf(x);
	float sY = sinf(y);
	float sZ = sinf(z);

	return Quat(
		cX * cY * cZ - sX * sY * sZ,
		sX * cY * cZ + sY * sZ * cX,
		sY * cX * cZ - sX * sZ * cY,
		sX * sY * cZ + sZ * cX * cY);
}