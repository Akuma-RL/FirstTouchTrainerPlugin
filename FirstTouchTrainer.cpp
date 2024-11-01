#include "pch.h"
#include "FirstTouchTrainer.h"
#include<chrono>
#include<thread>


BAKKESMOD_PLUGIN(FirstTouchTrainer, "First Touch Trainer", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void FirstTouchTrainer::onLoad()
{

	_globalCvarManager = cvarManager;
	bEnabled = std::make_shared<bool>(false);
	bDropShadow = std::make_shared<bool>(false);
	speedLog = std::make_shared<bool>(false);

	XPos = std::make_shared<int>(0);
	YPos = std::make_shared<int>(0);
	TextSize = std::make_shared<int>(0);

	goodColor = std::make_shared<LinearColor>();
	alrightColor = std::make_shared<LinearColor>();
	badColor = std::make_shared<LinearColor>();
	gDrawnColor = std::make_shared<LinearColor>();

	cvarManager->registerNotifier("FTTPlugin", [this](std::vector<std::string> args) {
		firstTouchTrainer();
		}, "", PERMISSION_ALL);

	cvarManager->registerCvar("FTT_Enable", "1", "Show First Touch Trainer", true, true, 0, true, 1).bindTo(bEnabled);

	cvarManager->registerCvar("FTT_X_Position", "880", "Change Text X Position", true, true, 0, true, 1920).bindTo(XPos);
	cvarManager->registerCvar("FTT_Y_Position", "1030", "Change Text Y Position", true, true, 0, true, 1080).bindTo(YPos);
	cvarManager->registerCvar("FTT_Text_Size", "3", "Change Text Size", true, true, 0, true, 5).bindTo(TextSize);
	cvarManager->registerCvar("FTT_Shadow", "1", "Enable text drop shadows", true, true, 0, true, 1).bindTo(bDropShadow);

	cvarManager->registerCvar("FTT_Good_Range", "#00FF00", "Good Range Color", true).bindTo(goodColor);
	cvarManager->registerCvar("FTT_Alright_Range", "#FFFF00", "Alright Range Color").bindTo(alrightColor);
	cvarManager->registerCvar("FTT_Bad_Range", "#FF0000FF", "Bad Range Color").bindTo(badColor);

	cvarManager->registerCvar("FTT_Speed_Log", "1", "Enables Speed Logging", true, true, 0, true, 1).bindTo(speedLog);

	cvarManager->getCvar("FTT_Speed_Log").addOnValueChanged([this](std::string oldValue, CVarWrapper cvar) {
		if (cvar.getBoolValue()) {
			gameWrapper->HookEvent("Function TAGame.Car_TA.SetVehicleInput", std::bind(&FirstTouchTrainer::Tick, this));
		}
		else {
			gameWrapper->UnhookEvent("Function TAGamee.Car_TA.SetVehicleInput");
		}
	});

	cvarManager->registerNotifier("FTT_Log_Reset", [this](std::vector<std::string> params)
		{
			this->measurements.clear();
		}, "Reset the log.", PERMISSION_ALL);

	cvarManager->registerNotifier("FTT_Log_Save", [this](std::vector<std::string> params)
		{
			std::ofstream logfile ( gameWrapper->GetBakkesModPath() / "data" / "ftt.csv" );
			logfile.open(gameWrapper->GetDataFolderW() + L"ftt.csv");

			for (auto it : this->measurements) {
				logfile << std::to_string(it) << std::endl;
			}

			logfile.close();
		}, "Save the Measurements to BakkesModFolder/data/ftt.csv", PERMISSION_ALL);

	//call game every frame to render canvas
	gameWrapper->RegisterDrawable([this](CanvasWrapper canvas)
		{
			Render(canvas);
		});
}

int FirstTouchTrainer::checkConditions()
{
	if (!(*bEnabled)) { return 1; }

	if (!gameWrapper->IsInGame()) { return 1; }

	else
	{
		return 0;
	}
}

std::tuple<float> FirstTouchTrainer::firstTouchTrainer()
{

	if (checkConditions() == 1) { return 0.0f; }

	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (server.IsNull()) { return 0.0f; }

	BallWrapper ball = server.GetBall();
	if (ball.IsNull()) { return 0.0f; }

	CarWrapper car = gameWrapper->GetLocalCar();
	if (car.IsNull()) { return 0.0f; }

	float velocityDifference = car.GetVelocity().magnitude() - ball.GetVelocity().magnitude();

	//grab cvars made for text position
	CVarWrapper fttXPositionCVar = cvarManager->getCvar("FTT_X_Position");
	if (!fttXPositionCVar) { return std::make_tuple(0.0f); }
	float fttXPosition = fttXPositionCVar.getFloatValue();

	CVarWrapper fttYPositionCVar = cvarManager->getCvar("FTT_Y_Position");
	if (!fttYPositionCVar) { return std::make_tuple(0.0f); }
	float fttYPosition = fttYPositionCVar.getFloatValue();


	return { velocityDifference };
}

void FirstTouchTrainer::onBallHit(std::string eventName)
{
	float timerSeconds = 1.5;

	if (*speedLog) { return; }

	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (server.IsNull()) { return; }

	BallWrapper ball = server.GetBall();
	if (ball.IsNull()) { return; }

	CarWrapper car = gameWrapper->GetLocalCar();
	if (car.IsNull()) { return; }

	if (car.IsOnGround() && ball.GetLocation().Z < 300) { return; }

	if (car.IsOnGround() && ball.GetLocation().Z > 300) { timerSeconds = 0.0; }

	if (!car.IsOnGround() && car.HasFlip() && ball.GetLocation().Z > 300) { timerSeconds = 1.5; }

	if (!car.IsOnGround() && !car.IsOnWall() && ball.GetLocation().Z > 300) { timerSeconds = 0.0; }

	if (car.IsOnWall() && ball.GetLocation().Z > 300) { timerSeconds = 0.0; }

}

void FirstTouchTrainer::Tick()
{
	if (*speedLog) { return; }

	ServerWrapper server = gameWrapper->GetCurrentGameState();
	if (server.IsNull()) { return; }

	BallWrapper ball = server.GetBall();
	if (ball.IsNull()) { return; }

	CarWrapper car = gameWrapper->GetLocalCar();
	if (car.IsNull()) { return; }

	if (car.IsOnGround()) { return; }
}

std::string FirstTouchTrainer::toStringPrecision(float InValue, int Precision)
{
	std::ostringstream out;
	out << std::fixed << std::setprecision(Precision) << InValue;
	return out.str();
}

void FirstTouchTrainer::onUnload() { }