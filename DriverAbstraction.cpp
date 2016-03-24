#include "DriverAbstraction.hpp"


bool DriverAbstraction::calibrationFinished = false;
bool DriverAbstraction::logging = true;
bool DriverAbstraction::calibrate = true;
std::shared_ptr<Info_Warning_Error_Logger> DriverAbstraction::logger;

void DriverAbstraction::setWallFrontCallback(std::function<void(void)> wallDetectedFrontCallback)
{DriverAbstraction::wallDetectedFrontCallback = wallDetectedFrontCallback;}

void DriverAbstraction::setWallBehindCallback(std::function<void(void)> wallDetectedBehindCallback)
{DriverAbstraction::wallDetectedBehindCallback = wallDetectedBehindCallback;}

void DriverAbstraction::setRobotFrontCallback(std::function<void(void)> robotDetectedFrontCallback)
{DriverAbstraction::robotDetectedFrontCallback = robotDetectedFrontCallback;}

void DriverAbstraction::setRobotBehindCallback(std::function<void(void)> robotDetectedBehindCallback)
{DriverAbstraction::robotDetectedBehindCallback = robotDetectedBehindCallback;}

void DriverAbstraction::reactOnWall()
{
    if(getTargetSpeed()>0&&wallDetectedFrontCallback)
        wallDetectedFrontCallback();
    else if(wallDetectedBehindCallback)
        wallDetectedBehindCallback();
}

void DriverAbstraction::reactOnRobot()
{
    if(getTargetSpeed()>0&&robotDetectedFrontCallback&&getCollisionDetector(FRONT_RIGHT_SENSOR)||getCollisionDetector(FRONT_LEFT_SENSOR))
        robotDetectedFrontCallback();
    else if(robotDetectedBehindCallback&&getCollisionDetector(BEHIND_SENSOR))
        robotDetectedBehindCallback();
}

void DriverAbstraction::moveBlocking(const Vector<2,float>& pos, const Vector<2,float>& target, const Vector<2,float>& direction)
{

}

void DriverAbstraction::moveUntilWallBlocking()
{

}

void DriverAbstraction::back(const Vector<2,float>& targetPos)
{

}


void DriverAbstraction::setCalibrationCallback(std::function<void(void)> calibrationCallback)
{DriverAbstraction::calibrationCallback = calibrationCallback;}

void DriverAbstraction::setLoopCallback(std::function<void(void)> loopCallback)
{DriverAbstraction::loopCallback = loopCallback;}

void DriverAbstraction::reactOnJack()
{
    bool testOrMatch = /**getStateButtonTestOrMatch**/;
    if(testOrMatch)
    {
        if(calibrate)
        {
            if(logging)
                (*logger)<<"Calibration begins in test mode "<<std::endl;
            calibrationCallback();
            if(logging)
                (*logger)<<"Calibration ends in test mode "<<std::endl;
            calibrationFinished = true;
        }
        if(logging)
            (*logger)<<"Loop begins in test mode "<<std::endl;
        loopCallback();
        if(logging)
            (*logger)<<"Loop ends in test mode "<<std::endl;
    }
    else
        if(calibrationFinished||!calibrate)
        {
            if(logging)
                (*logger)<<"Loop begins in match mode "<<std::endl;
            loopCallback();
            if(logging)
                (*logger)<<"Loop ends in match mode "<<std::endl;
        }
        else
        {
            if(logging)
                (*logger)<<"Calibration starts in match mode "<<std::endl;
            calibrationCallback();
            if(logging)
                (*logger)<<"Calibration ends in match mode "<<std::endl;
            calibrationFinished = true;
        }
}

bool DriverAbstraction::toggleLogging(bool logging)
{
    logger = Easy_Log_In_File::getInfoLog();
    DriverAbstraction::logging = logging;
}

void DriverAbstraction::toggleCalibration(bool calibrate)
{DriverAbstraction::calibrate = calibrate;}


void DriverAbstraction::init()
{
    //set callbacks
    onGameStart(&DriverAbstraction::reactOnJack);
    setBlockingCallback(&DriverAbstraction::reactOnWall);
    /*????? wait for spec*/(&DriverAbstraction::reactOnRobot);
    setMoveStartCallback(&Situation::updateAngleStartingMove);
    setMoveEndCallback(&Situation::updatePositionEndingMove);
}
