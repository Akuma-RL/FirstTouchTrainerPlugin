#include "pch.h"
#include "FirstTouchTrainer.h"

BAKKESMOD_PLUGIN(FirstTouchTrainer, "First Touch Trainer", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void FirstTouchTrainer::onLoad()
{
	_globalCvarManager = cvarManager;

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

	cvarManager->registerCvar("FTT_Enable", "1", "Show First Touch Trainer", true, true, 0, true, 1).bindTo(bEnabled);

	/////////////////////////X SETTINGS////////////////////////////////

	tXPos = std::make_shared<int>(0);
	tXPosDefault = std::make_shared<float>(0.0f);

	float speedDefaultX = screenSize.X / 2.18;
	*tXPosDefault = speedDefaultX;
	std::string speedDefaultXString = toStringPrecision(speedDefaultX, 2);

	cvarManager->registerCvar("FTT_X_Position", speedDefaultXString, "Change Text X Position", true, true, 0, true, *bScreenSizeX).bindTo(tXPos);

	/////////////////////////Y SETTINGS////////////////////////////////

	tYPos = std::make_shared<int>(0);
	tYPosDefault = std::make_shared<float>(0.0f);

	float speedDefaultY = screenSize.Y / 1.05;
	*tYPosDefault = speedDefaultY;
	std::string speedDefaultYString = toStringPrecision(speedDefaultY, 2);

	cvarManager->registerCvar("FTT_Y_Position", speedDefaultYString, "Change Text Y Position", true, true, 0, true, *bScreenSizeY).bindTo(tYPos);

	////////////////////////COLOR SETTINGS//////////////////////////////

	//GOOD//
	cGoodColor = std::make_shared<LinearColor>();

	cvarManager->registerCvar("FTT_Good_Range", "#00FF00", "Good Range Color", true).bindTo(cGoodColor);

	//ALRIGHT//
	cAlrightColor = std::make_shared<LinearColor>();

	cvarManager->registerCvar("FTT_Alright_Range", "#FFFF00", "Alright Range Color", true).bindTo(cAlrightColor);

	//BAD//
	cBadColor = std::make_shared<LinearColor>();

	cvarManager->registerCvar("FTT_Bad_Range", "#FF0000", "Bad Range Color", true).bindTo(cBadColor);

	/////////////////////GLOBAL TEXT SETTINGS/////////////////////////

	//SIZE//
	tTextSize = std::make_shared<int>(0);

	cvarManager->registerCvar("FTT_Text_Size", "3", "Change Text Size", true, true, 1, true, 10).bindTo(tTextSize);

	//DROP SHADOW//
	tDropShadow = std::make_shared<bool>(false);

	cvarManager->registerCvar("FTT_Shadow", "1", "Enable text drop shadows", true, true, 0, true, 1).bindTo(tDropShadow);

	//////////////////////////////////////////////////////////////////
	///////////////SESSION TIMER OSD SETTINGS/////////////////////////
	//////////////////////////////////////////////////////////////////

	sSessionTimerEnabled = std::make_shared<bool>(false);

	cvarManager->registerCvar("FTT_SessionTimer", "0", "Turn on session timer to keep track of how long you've been in this training session", true, true, 0, true, 1).bindTo(sSessionTimerEnabled);

	/////////////////////////X SETTINGS////////////////////////////////

	sSessionTimerX = std::make_shared<int>(0);
	sSessionTimerXDefault = std::make_shared<float>(0.0f);

	float timerDefaultX = screenSize.X / 2.13;
	*sSessionTimerXDefault = timerDefaultX;
	std::string timerDefaultXString = toStringPrecision(timerDefaultX, 2);

	cvarManager->registerCvar("FTT_SessionTimerX", timerDefaultXString, "Change session timer X position", true, true, 0, true, *bScreenSizeX).bindTo(sSessionTimerX);

	/////////////////////////Y SETTINGS////////////////////////////////

	sSessionTimerY = std::make_shared<int>(0);
	sSessionTimerYDefault = std::make_shared<float>(0.0f);

	float timerDefaultY = screenSize.Y / 108;
	*sSessionTimerYDefault = timerDefaultY;
	std::string timerDefaultYString = toStringPrecision(timerDefaultY, 2);

	cvarManager->registerCvar("FTT_SessionTimerY", timerDefaultYString, "Change session timer Y position", true, true, 0, true, *bScreenSizeY).bindTo(sSessionTimerY);

	///////////////////////COLOR SETTINGS//////////////////////////////

	sSessionTimerColor = std::make_shared<LinearColor>();

	cvarManager->registerCvar("FTT_SessionTimerColor", "#FFFFFF", "Change timer color", true).bindTo(sSessionTimerColor);

	//////////////////////////////////////////////////////////////////
	////////////////////TOUCH ZONE SETTINGS///////////////////////////
	//////////////////////////////////////////////////////////////////

	zTouchZoneEnabled = std::make_shared<bool>(false);
	zTouchZoneSphereEnabled = std::make_shared<bool>(false);
	zTouchZoneMatchColor = std::make_shared<bool>(false);

	cvarManager->registerCvar("FTT_TouchZoneEnabled", "0", "Enable/Disable Touch Zone", true, true, 0, true, 1).bindTo(zTouchZoneEnabled);
	cvarManager->registerCvar("FTT_TouchZoneSphereEnabled", "0", "Enable/Disable Touch Zone Sphere", true, true, 0, true, 1).bindTo(zTouchZoneSphereEnabled);
	cvarManager->registerCvar("FTT_TouchZoneMatchSpeed", "0", "Enable to match color of speed indicator", true, true, 0, true, 1).bindTo(zTouchZoneMatchColor);

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

			if (*zTouchZoneEnabled)
			{
				RenderTouchZone(canvas);
			}
		});
}

void FirstTouchTrainer::OnFreeplayLoad(std::string eventName)
{
	//circles.clear();
	//cvarManager->log(std::string("OnFreeplayLoad") + eventName);
	//if (*zTouchZoneEnabled) {
	//	gameWrapper->RegisterDrawable(std::bind(&FirstTouchTrainer::RenderTouchZone, this, std::placeholders::_1));
	//}
}

void FirstTouchTrainer::OnFreeplayDestroy(std::string eventName)
{
	//gameWrapper->UnregisterDrawables();
}

std::tuple<float, float, float, float> FirstTouchTrainer::firstTouchTrainer()
{
	if (checkConditions() == 0) { return std::make_tuple(0.0f, 0.0f, 0.0f, 93.14f); }

	//create server wrapper to get ball info
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (server.IsNull()) { return std::make_tuple(0.0f, 0.0f, 0.0f, 93.14f); }

	//get ball information and assign to ball
	BallWrapper ball = server.GetBall();
	if (ball.IsNull()) { return std::make_tuple(0.0f, 0.0f, 0.0f, 93.14f); }

	//get car information and assign to car
	CarWrapper car = gameWrapper->GetLocalCar();
	if (car.IsNull()) { return std::make_tuple(0.0f, 0.0f, 0.0f, 93.14f); }

	Vector ballXYZ = ball.GetCurrentRBLocation();
	Vector carXYZ = car.GetVelocity().magnitude();

	float ballX = ballXYZ.X;
	float ballY = ballXYZ.Y;
	float ballZ = ballXYZ.Z;

	//make float and get the ball and vehicle magnitude and subtract one from the other
	float velocityDifference = car.GetVelocity().magnitude() - ball.GetVelocity().magnitude();
	if (-3 <= velocityDifference && velocityDifference <= 3) { velocityDifference = 0; }
	//return the outcome of previous line
	return { std::make_tuple(velocityDifference, ballX, ballY, ballZ) };
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
		if (!*zTouchZoneEnabled) { return 0; }
	}
	//check if player is in online match
	if (!gameWrapper->IsInGame()) { return 0; }

	return 1;
}

int FirstTouchTrainer::timerCheckConditions()
{
	//check if session timer is enabled
	if (*sSessionTimerEnabled == 0) { return 0; }

	//check if player is in online match
	if (!gameWrapper->IsInGame()) { return 0; }

	return 1;
}

int FirstTouchTrainer::touchZoneCheckConditions()
{
	if (*zTouchZoneEnabled == 0) { return 0; }


	if (!gameWrapper->IsInGame()) { return 0; }

	return 1;
}

float FirstTouchTrainer::IsBallInAir()
{
	if (!*zTouchZoneEnabled) { return 0; }

	float ballZ = std::get<3>(firstTouchTrainer());

	if (ballZ <= 400.0f) { return 0; }
	return 1;
}

void FirstTouchTrainer::onUnload() { }