#include "pch.h"
#include "FirstTouchTrainer.h"


BAKKESMOD_PLUGIN(FirstTouchTrainer, "First Touch Trainer", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void FirstTouchTrainer::onLoad()
{

	_globalCvarManager = cvarManager;

	//OSD enabled or disabled
	bEnabled = std::make_shared<bool>(false);

	//text settings
	XPos = std::make_shared<int>(0);
	YPos = std::make_shared<int>(0);
	TextSize = std::make_shared<int>(0);
	bDropShadow = std::make_shared<bool>(false);

	//text color settings
	goodColor = std::make_shared<LinearColor>();
	alrightColor = std::make_shared<LinearColor>();
	badColor = std::make_shared<LinearColor>();

	cvarManager->registerNotifier("FTTPlugin", [this](std::vector<std::string> args) {
		firstTouchTrainer();
		}, "", PERMISSION_ALL);

	//osd cvar
	cvarManager->registerCvar("FTT_Enable", "1", "Show First Touch Trainer", true, true, 0, true, 1).bindTo(bEnabled);

	//text settings cvars
	cvarManager->registerCvar("FTT_X_Position", "880", "Change Text X Position", true, true, 0, true, 3840).bindTo(XPos);
	cvarManager->registerCvar("FTT_Y_Position", "1030", "Change Text Y Position", true, true, 0, true, 2160).bindTo(YPos);
	cvarManager->registerCvar("FTT_Text_Size", "3", "Change Text Size", true, true, 0, true, 5).bindTo(TextSize);
	cvarManager->registerCvar("FTT_Shadow", "1", "Enable text drop shadows", true, true, 0, true, 1).bindTo(bDropShadow);

	//color settings cvars
	cvarManager->registerCvar("FTT_Good_Range", "#00FF00", "Good Range Color", true).bindTo(goodColor);
	cvarManager->registerCvar("FTT_Alright_Range", "#FFFF00", "Alright Range Color", true).bindTo(alrightColor);
	cvarManager->registerCvar("FTT_Bad_Range", "#FF0000", "Bad Range Color", true).bindTo(badColor);

	//call game every frame to render canvas
	gameWrapper->RegisterDrawable([this](CanvasWrapper canvas)
		{
			Render(canvas);
		});
}

std::tuple<float> FirstTouchTrainer::firstTouchTrainer()
{
	if (checkConditions() == 1) { return 0.0f; }

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
	if (!(*bEnabled)) { return 1; }

	//check if player is in live game or not
	if (!gameWrapper->IsInGame()) { return 1; }

	return 0;
}

void FirstTouchTrainer::onUnload() { }