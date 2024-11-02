#include "pch.h"
#include "FirstTouchTrainer.h"


BAKKESMOD_PLUGIN(FirstTouchTrainer, "First Touch Trainer", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void FirstTouchTrainer::onLoad()
{
	_globalCvarManager = cvarManager;

	//global screen size limit
	bScreenSizeX = std::make_shared<float>(0.0f);
	bScreenSizeY = std::make_shared<float>(0.0f);

	Vector2 screenSize = gameWrapper->GetScreenSize();
	*bScreenSizeX = screenSize.X;
	*bScreenSizeY = screenSize.Y;

	//OSD enabled or disabled
	bEnabled = std::make_shared<bool>(false);

	//text settings
	tXPos = std::make_shared<int>(0);
	tYPos = std::make_shared<int>(0);
	tTextSize = std::make_shared<int>(0);
	tDropShadow = std::make_shared<bool>(false);

	float speedDefaultX = screenSize.X / 2.18;
	float speedDefaultY = screenSize.Y / 1.05;
	
	tDefaultXPos = std::make_shared<float>(0.0f);
	tDefaultYPos = std::make_shared<float>(0.0f);

	*tDefaultXPos = speedDefaultX;
	*tDefaultYPos = speedDefaultY;

	std::string speedDefaultXString = toStringPrecision(speedDefaultX, 2);
	std::string speedDefaultYString = toStringPrecision(speedDefaultY, 2);


	//session timer settings
	sSessionTimerEnabled = std::make_shared<bool>(false);
	sSessionTimerX = std::make_shared<int>(0);
	sSessionTimerY = std::make_shared<int>(0);

	sSessionTimerColor = std::make_shared<LinearColor>();

	float timerDefaultX = screenSize.X / 2.13;
	float timerDefaultY = screenSize.Y / 108;

	sDefaultXPos = std::make_shared<float>(0.0f);
	sDefaultYPos = std::make_shared<float>(0.0f);

	*sDefaultXPos = timerDefaultX;
	*sDefaultYPos = timerDefaultY;

	std::string timerDefaultXString = toStringPrecision(timerDefaultX, 2);
	std::string timerDefaultYString = toStringPrecision(timerDefaultY, 2);

	//text color settings
	cGoodColor = std::make_shared<LinearColor>();
	cAlrightColor = std::make_shared<LinearColor>();
	cBadColor = std::make_shared<LinearColor>();




	cvarManager->registerNotifier("FTTPlugin", [this](std::vector<std::string> args) {
		firstTouchTrainer();
		}, "", PERMISSION_ALL);

	//osd cvar
	cvarManager->registerCvar("FTT_Enable", "1", "Show First Touch Trainer", true, true, 0, true, 1).bindTo(bEnabled);

	//text settings cvars
	cvarManager->registerCvar("FTT_X_Position", speedDefaultXString, "Change Text X Position", true, true, 0, true, *bScreenSizeX).bindTo(tXPos);
	cvarManager->registerCvar("FTT_Y_Position", speedDefaultYString, "Change Text Y Position", true, true, 0, true, *bScreenSizeY).bindTo(tYPos);
	cvarManager->registerCvar("FTT_Text_Size", "3", "Change Text Size", true, true, 1, true, 10).bindTo(tTextSize);
	cvarManager->registerCvar("FTT_Shadow", "1", "Enable text drop shadows", true, true, 0, true, 1).bindTo(tDropShadow);

	//color settings cvars
	cvarManager->registerCvar("FTT_Good_Range", "#00FF00", "Good Range Color", true).bindTo(cGoodColor);
	cvarManager->registerCvar("FTT_Alright_Range", "#FFFF00", "Alright Range Color", true).bindTo(cAlrightColor);
	cvarManager->registerCvar("FTT_Bad_Range", "#FF0000", "Bad Range Color", true).bindTo(cBadColor);

	//session timer
	cvarManager->registerCvar("FTT_SessionTimer", "0", "Turn on session timer to keep track of how long you've been in this training session", true, true, 0, true, 1).bindTo(sSessionTimerEnabled);
	cvarManager->registerCvar("FTT_SessionTimerX", timerDefaultXString, "Change session timer X position", true, true, 0, true, *bScreenSizeX).bindTo(sSessionTimerX);
	cvarManager->registerCvar("FTT_SessionTimerY", timerDefaultYString, "Change session timer Y position", true, true, 0, true, *bScreenSizeY).bindTo(sSessionTimerY);
	cvarManager->registerCvar("FTT_SessionTimerColor", "#FFFFFF", "Change timer color", true).bindTo(sSessionTimerColor);

	//call game every frame to render canvas
	gameWrapper->RegisterDrawable([this](CanvasWrapper canvas)
		{
			Render(canvas);
		});
}

std::tuple<float> FirstTouchTrainer::firstTouchTrainer()
{
	if (checkConditions() == 0) { return 0.0f; }

	//create server wrapper to get ball info
	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (server.IsNull()) { return 0.0f; }

	//get ball information and assign to ball
	BallWrapper ball = server.GetBall();
	if (ball.IsNull()) { return 0.0f; }

	//get car information and assign to car
	CarWrapper car = gameWrapper->GetLocalCar();
	if (car.IsNull()) { return 0.0f; }

	//make float and get the ball and vehicle magnitude and subtract one from the other
	float velocityDifference = car.GetVelocity().magnitude() - ball.GetVelocity().magnitude();

	//return the outcome of previous line
	return { velocityDifference };
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
	if (!(*bEnabled)) { return 0; }

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


void FirstTouchTrainer::onUnload() { }