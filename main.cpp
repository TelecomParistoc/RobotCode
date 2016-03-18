#include "DriverAbstraction.hpp"
#include "Calibration.hpp"
#include "Automata.hpp"


int main()
{
    Easy_Log_In_File::setFolderPath("logs/");
    std::shared_ptr<Info_Warning_Error_Logger> logger = Easy_Log_In_File::getInfoLog();
    logger->addHandler(std::shared_ptr<Stdout_Handler>(new Stdout_Handler())); //a commenter lors de la mise en prod

    Situation::resetMapDescription("data/mapDescription.txt");
    Situation::resetRobotsDescription("data/robotDescription.txt");

    std::shared_ptr<Situation> situation(new Situation());
    std::shared_ptr<ActionBoard> actions(new ActionBoard("data/actionsDescription.txt"));
    std::shared_ptr<Calibration> calibration(new Calibration(true));
    calibration->reset(situation);

    logger->info("Initializing automata with :");
    logger->info(*situation);
    logger->info(*actions);
    logger->info(*calibration);
    Automata::reset(situation,actions,calibration);

    DriverAbstraction::setCalibrationCallback(std::bind(&Calibration::launch,*calibration));
    DriverAbstraction::setLoopCallback(Automata::launch);

    logger->info("Beginning main loop");
    while(!Automata::isDone())
        usleep(1000000);
    logger->info("Automata is done, we reached the end of the game");

    return 0;
}
