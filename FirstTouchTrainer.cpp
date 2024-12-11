#include "pch.h"
#include "FirstTouchTrainer.h"

BAKKESMOD_PLUGIN(FirstTouchTrainer, "First Touch Trainer", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void FirstTouchTrainer::onLoad()
{
	_globalCvarManager = cvarManager;
	index = std::make_shared<int>(0);
	calculationIndex = std::make_shared<int>(0);


	gDrawnColor = std::make_shared<LinearColor>();

	//////////////////////////////////////////////////////////////////
	/////////////////GLOBAL SCREEN SIZE LIMIT/////////////////////////
	//////////////////////////////////////////////////////////////////
	bScreenSizeX = std::make_shared<float>(0.0f);
	bScreenSizeY = std::make_shared<float>(0.0f);

	Vector2 screenSize = gameWrapper->GetScreenSize();
	*bScreenSizeX = screenSize.X;
	*bScreenSizeY = screenSize.Y;


	cvarManager->registerNotifier("FTTPlugin", [this](std::vector<std::string> args) {
		firstTouchTrainer();
		}, "", PERMISSION_ALL);


	//////////////////////////////////////////////////////////////////
	////////////////////SPEED OSD SETTINGS////////////////////////////
	//////////////////////////////////////////////////////////////////
	bEnabled = std::make_shared<bool>(false);

	cvarManager->registerCvar("FTT_Enable", "0", "Show First Touch Trainer", true, true, 0, true, 1, true).bindTo(bEnabled);

	bXYZEnabled = std::make_shared<bool>(false);

	cvarManager->registerCvar("FTT_XYZEnabled", "0", "Calculate speed differential with XYZ Velocity", true, true, 0, true, 1, true).bindTo(bXYZEnabled);

	/////////////////////////X SETTINGS////////////////////////////////

	tXPos = std::make_shared<int>(0);
	tXPosDefault = std::make_shared<float>(0.0f);

	float speedDefaultX = screenSize.X / 2.18;
	*tXPosDefault = speedDefaultX;
	std::string speedDefaultXString = toStringPrecision(speedDefaultX, 2);

	cvarManager->registerCvar("FTT_X_Position", speedDefaultXString, "Change Text X Position", true, true, 0, true, *bScreenSizeX, true).bindTo(tXPos);

	/////////////////////////Y SETTINGS////////////////////////////////

	tYPos = std::make_shared<int>(0);
	tYPosDefault = std::make_shared<float>(0.0f);

	float speedDefaultY = screenSize.Y / 1.05;
	*tYPosDefault = speedDefaultY;
	std::string speedDefaultYString = toStringPrecision(speedDefaultY, 2);

	cvarManager->registerCvar("FTT_Y_Position", speedDefaultYString, "Change Text Y Position", true, true, 0, true, *bScreenSizeY, true).bindTo(tYPos);

	////////////////////////COLOR SETTINGS//////////////////////////////

	//GOOD//
	cGoodColor = std::make_shared<LinearColor>();

	cvarManager->registerCvar("FTT_Good_Range", "#00FF00", "Good Range Color", true, false, 0, false, 0, true).bindTo(cGoodColor);

	//ALRIGHT//
	cAlrightColor = std::make_shared<LinearColor>();

	cvarManager->registerCvar("FTT_Alright_Range", "#FFFF00", "Alright Range Color", true, false, 0, false, 0, true).bindTo(cAlrightColor);

	//BAD//
	cBadColor = std::make_shared<LinearColor>();

	cvarManager->registerCvar("FTT_Bad_Range", "#FF0000", "Bad Range Color", true, false, 0, false, 0, true).bindTo(cBadColor);

	/////////////////////GLOBAL TEXT SETTINGS/////////////////////////

	//SIZE//
	tTextSize = std::make_shared<int>(0);

	cvarManager->registerCvar("FTT_Text_Size", "3", "Change Text Size", true, true, 1, true, 10, true).bindTo(tTextSize);

	//DROP SHADOW//
	tDropShadow = std::make_shared<bool>(false);

	cvarManager->registerCvar("FTT_Shadow", "1", "Enable text drop shadows", true, true, 0, true, 1, true).bindTo(tDropShadow);

	//////////////////////////////////////////////////////////////////
	///////////////SESSION TIMER OSD SETTINGS/////////////////////////
	//////////////////////////////////////////////////////////////////

	sSessionTimerEnabled = std::make_shared<bool>(false);

	cvarManager->registerCvar("FTT_SessionTimer", "0", "Turn on session timer to keep track of how long you've been in this training session", true, true, 0, true, 1, true).bindTo(sSessionTimerEnabled);

	/////////////////////////X SETTINGS////////////////////////////////

	sSessionTimerX = std::make_shared<int>(0);
	sSessionTimerXDefault = std::make_shared<float>(0.0f);

	float timerDefaultX = screenSize.X / 2.13;
	*sSessionTimerXDefault = timerDefaultX;
	std::string timerDefaultXString = toStringPrecision(timerDefaultX, 2);

	cvarManager->registerCvar("FTT_SessionTimerX", timerDefaultXString, "Change session timer X position", true, true, 0, true, *bScreenSizeX, true).bindTo(sSessionTimerX);

	/////////////////////////Y SETTINGS////////////////////////////////

	sSessionTimerY = std::make_shared<int>(0);
	sSessionTimerYDefault = std::make_shared<float>(0.0f);

	float timerDefaultY = screenSize.Y / 108;
	*sSessionTimerYDefault = timerDefaultY;
	std::string timerDefaultYString = toStringPrecision(timerDefaultY, 2);

	cvarManager->registerCvar("FTT_SessionTimerY", timerDefaultYString, "Change session timer Y position", true, true, 0, true, *bScreenSizeY, true).bindTo(sSessionTimerY);

	///////////////////////COLOR SETTINGS//////////////////////////////

	sSessionTimerColor = std::make_shared<LinearColor>();

	cvarManager->registerCvar("FTT_SessionTimerColor", "#FFFFFF", "Change timer color", true, false, 0, false, 0, true).bindTo(sSessionTimerColor);

	//////////////////////////////////////////////////////////////////
	////////////////////TOUCH ZONE SETTINGS///////////////////////////
	//////////////////////////////////////////////////////////////////

	zTouchZoneEnabled = std::make_shared<bool>(false);
	cvarManager->registerCvar("FTT_TouchZoneEnabled", "0", "Enable/Disable Touch Zone", true, true, 0, true, 1, true).bindTo(zTouchZoneEnabled);

	zTouchZoneCircleEnabled = std::make_shared<bool>(false);
	cvarManager->registerCvar("FTT_TouchZoneCircleEnabled", "1", "Enable/Disable Touch Zone Circle", true, true, 0, true, 1, true).bindTo(zTouchZoneCircleEnabled);

	zTouchZoneCircleRadius = std::make_shared<float>(0);

	cvarManager->registerCvar("FTT_TouchZoneCircleRadius", "47", "Set the size of the radius of the drawn circle", true, true, 30, true, 60, true).bindTo(zTouchZoneCircleRadius);

	zTouchZoneCircleThicc = std::make_shared<int>(0);

	cvarManager->registerCvar("FTT_TouchZoneCircleThicc", "1", "Set the thickness of the drawn circle", true, true, 1, true, 3, true).bindTo(zTouchZoneCircleThicc);

	zTouchZoneSphereEnabled = std::make_shared<bool>(false);
	cvarManager->registerCvar("FTT_TouchZoneSphereEnabled", "0", "Enable/Disable Touch Zone Sphere", true, true, 0, true, 1, true).bindTo(zTouchZoneSphereEnabled);

	zTouchZoneSphereRadius = std::make_shared<float>(0);

	cvarManager->registerCvar("FTT_TouchZoneSphereRadius", "5", "Set the size of the radius of the drawn sphere", true, true, 1, true, 10, true).bindTo(zTouchZoneSphereRadius);

	zTouchZoneSphereColor = std::make_shared<LinearColor>();

	cvarManager->registerCvar("FTT_TouchZoneSphereColor", "#FF0000FF", "Set the color of the Sphere", true).bindTo(zTouchZoneSphereColor);

	zTouchZoneMatchColor = std::make_shared<bool>(false);
	cvarManager->registerCvar("FTT_TouchZoneMatchColor", "1", "Enable to match color of speed indicator", true, true, 0, true, 1, true).bindTo(zTouchZoneMatchColor);

	zTouchZoneSphereMatchColor = std::make_shared<bool>(false);
	cvarManager->registerCvar("FTT_TouchZoneSphereMatchColor", "1", "Enable to match color of speed indicator", true, true, 0, true, 1, true).bindTo(zTouchZoneSphereMatchColor);

	///////////////////////COLOR SETTINGS//////////////////////////////

	zTouchZoneColor = std::make_shared<LinearColor>();

	cvarManager->registerCvar("FTT_TouchZoneColor", "#FF0000FF", "Change the color of the Touch Zone", true).bindTo(zTouchZoneColor);

	gameWrapper->HookEvent("Function TAGame.Mutator_Freeplay_TA.Init", bind(&FirstTouchTrainer::OnFreeplayLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.Destroyed", bind(&FirstTouchTrainer::OnFreeplayDestroy, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_TrainingEditor_TA.StartPlayTest", bind(&FirstTouchTrainer::OnFreeplayLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_TrainingEditor_TA.Destroyed", bind(&FirstTouchTrainer::OnFreeplayDestroy, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameInfo_Replay_TA.InitGame", bind(&FirstTouchTrainer::OnFreeplayLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.Replay_TA.EventPostTimeSkip", bind(&FirstTouchTrainer::OnFreeplayLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameInfo_Replay_TA.Destroyed", bind(&FirstTouchTrainer::OnFreeplayDestroy, this, std::placeholders::_1));

	//////////////////////////RENDER///////////////////////////////////

	gameWrapper->RegisterDrawable([this](CanvasWrapper canvas)
		{
			if (*bEnabled)
			{
				RenderFTT(canvas);
			}

			if (*sSessionTimerEnabled)
			{
				RenderSessionTimer(canvas);
			}

			if (*zTouchZoneEnabled || *zTouchZoneSphereEnabled)
			{
				RenderTouchZone(canvas);
			}
			if (*zTouchZoneSphereEnabled)
			{
				RenderSphere(canvas);
			}
		});
}

void FirstTouchTrainer::OnFreeplayLoad(std::string eventName)
{

}

void FirstTouchTrainer::OnFreeplayDestroy(std::string eventName)
{

}

std::tuple<float, float, float, float, float> FirstTouchTrainer::firstTouchTrainer()
{
	if (checkConditions() == 0) { return std::make_tuple(0.0f, 0.0f, 0.0f, 93.14f, 0.0f); }

	//create server wrapper to get ball info
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (server.IsNull()) { return std::make_tuple(0.0f, 0.0f, 0.0f, 93.14f, 0.0f); }

	//get ball information and assign to ball
	BallWrapper ball = server.GetBall();
	if (ball.IsNull()) { return std::make_tuple(0.0f, 0.0f, 0.0f, 93.14f, 0.0f); }

	//get car information and assign to car
	CarWrapper car = gameWrapper->GetLocalCar();
	if (car.IsNull()) { return std::make_tuple(0.0f, 0.0f, 0.0f, 93.14f, 0.0f); }

	Vector ballXYZ = ball.GetCurrentRBLocation();
	Vector carPos = car.GetCurrentRBLocation();

	float ballX = ballXYZ.X;
	float ballY = ballXYZ.Y;
	float ballZ = ballXYZ.Z;

	Vector ballVel = ball.GetVelocity().magnitude();
	Vector carVel = car.GetVelocity().magnitude();

	//make float and get the ball and vehicle magnitude and subtract one from the other
	float carXYMag = GetMagnitude(car.GetVelocity().X, car.GetVelocity().Y);
	float ballXYMag = GetMagnitude(ball.GetVelocity().X, ball.GetVelocity().Y);

	float carZMag = std::abs(car.GetVelocity().Z);
	float ballZMag = std::abs(ball.GetVelocity().Z);

	float zVelDif = carZMag - ballZMag;

	float velocityDifference = 0;
	
	if (*calculationIndex < 0 || *calculationIndex > 2)
	{
		*calculationIndex = 0;
	}

	if (*calculationIndex == 0 || *calculationIndex == 2)
	{
		velocityDifference = carXYMag - ballXYMag;

		if (-1 <= velocityDifference && velocityDifference <= 1) {
			velocityDifference = 0;
		}
	}
	if (*calculationIndex == 1)
	{
		velocityDifference = car.GetVelocity().magnitude() - ball.GetVelocity().magnitude();

		if (-1 <= velocityDifference && velocityDifference <= 1) {
			velocityDifference = 0;
		}
	}

	if (-1 <= zVelDif && zVelDif <= 1) {
		zVelDif = 0;
	}

	//return the outcome of previous line
	return { std::make_tuple(velocityDifference, ballX, ballY, ballZ, zVelDif) };
}

float FirstTouchTrainer::GetMagnitude(float x, float y)
{
	return std::sqrt((x * x) + (y * y));
}

//function to transfer string to float with an input level of precision
std::string FirstTouchTrainer::toStringPrecision(float InValue, int Precision)
{
	std::ostringstream out;
	out << std::fixed << std::setprecision(Precision) << InValue;
	return out.str();
}

int FirstTouchTrainer::checkConditions()
{
	//check if OSD is enabled
	if (!*bEnabled) {
		if (!*zTouchZoneEnabled) { if (!*zTouchZoneSphereEnabled) { return 0; } }
	}
	//check if player is in online match
	if (gameWrapper->IsInOnlineGame()) { return 0; }

	return 1;
}

int FirstTouchTrainer::timerCheckConditions()
{
	//check if session timer is enabled
	if (!*sSessionTimerEnabled) { return 0; }

	//check if player is in online match
	if (!gameWrapper->IsInGame()) { return 0; }

	return 1;
}

int FirstTouchTrainer::touchZoneCheckConditions()
{
	if (!*zTouchZoneEnabled) { return 0; }


	if (!gameWrapper->IsInGame()) { return 0; }

	return 1;
}

float FirstTouchTrainer::IsBallInAir()
{
	float ballZ = std::get<3>(firstTouchTrainer());

	if (ballZ <= 400.0f) { return 0; }
	return 1;
}

int FirstTouchTrainer::InReplay()
{

}

void FirstTouchTrainer::onUnload() { }