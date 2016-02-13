#include "DriverAbstraction.hpp"
#include "Calibration.hpp"
#include "Automata.hpp"


int main()
{
    Situation::resetMapDescription("mapDescription.txt");
    Situation::resetRobotsDescription("robotsDescription.txt");

    std::shared_ptr<Situation> situation(new Situation());

    Calibration calibration(true);
    calibration.reset(situation);
    calibration.setGoal(Situation::goalCalibration());

    Automata::reset(situation);

    DriverAbstraction::setCalibrationCallback(std::bind(&Calibration::launch,&calibration));
    DriverAbstraction::setLoopCallback(Automata::launch);
}
