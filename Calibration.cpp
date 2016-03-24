#include "Calibration.hpp"


Calibration::Calibration(bool launchInAnyCondition)
{
    this->launchInAnyCondition = launchInAnyCondition;
}

void Calibration::launch()
{
    if(situation&&(launchInAnyCondition||(!launchInAnyCondition&&!situation->hasStarted())))
    {
        mutex.lock();
        situation->avoidPreemption();
        Vector<2,float> pos = situation->getCurrentPos();
        Vector<2,float> direction = situation->getCurrentDirection();
        mutex.unlock();

        Vector<2,float> target = Situation::getNearestSideOnTable(pos);
        if(Util::dotProduct(direction,target-pos)>=0)
        {
            DriverAbstraction::setWallFrontCallback(std::bind(&Calibration::wallDetectedFront,this));
            DriverAbstraction::setRobotFrontCallback(std::bind(&Calibration::robotDetectedFront,this));
        }
        else
        {
            DriverAbstraction::setWallBehindCallback(std::bind(&Calibration::wallDetectedBehind,this));
            DriverAbstraction::setRobotBehindCallback(std::bind(&Calibration::robotDetectedBehind,this));
        }
        DriverAbstraction::moveBlocking(pos,target,direction);
        DriverAbstraction::moveUntilWallBlocking();
        DriverAbstraction::back(/*un petit peu*/);

        Vector<2,float> target = Situation::getNearestCornerOnTable(pos);
        if(Util::dotProduct(direction,target-pos)>=0)
        {
            DriverAbstraction::setWallFrontCallback(std::bind(&Calibration::wallDetectedFront,this));
            DriverAbstraction::setRobotFrontCallback(std::bind(&Calibration::robotDetectedFront,this));
        }
        else
        {
            DriverAbstraction::setWallBehindCallback(std::bind(&Calibration::wallDetectedBehind,this));
            DriverAbstraction::setRobotBehindCallback(std::bind(&Calibration::robotDetectedBehind,this));
        }
        DriverAbstraction::moveBlocking(pos,target,direction);
        DriverAbstraction::moveUntilWallBlocking();
        DriverAbstraction::back(/*un petit peu*/);

        mutex.lock();
        situation->resetCurrentPosition(Vector<2,float>(target));
        situation->allowPreemption();
        mutex.unlock();
    }
}

void Calibration::reset(std::shared_ptr<Situation> s)
{
    situation = s;
}

void Calibration::setLaunchBehaviour(bool launchInAnyCondition)
{
    this->launchInAnyCondition = launchInAnyCondition;
}

void Calibration::wallDetectedFront()
{
    DriverAbstraction::ackMoving();
}

void Calibration::wallDetectedBehind()
{
    DriverAbstraction::ackMoving();
}

void Calibration::robotDetectedFront()
{
    if(!Situation::isCurrentPosInGrantedAreaForDetection())
    {
        DriverAbstraction::stopMoving();
        DriverAbstraction::setNoRobotFrontCallback(std::bind(&Calibration::NoMoreRobotDetectedFront,this));
    }
}

void Calibration::robotDetectedBehind()
{
    if(!Situation::isCurrentPosInGrantedAreaForDetection())
    {
        DriverAbstraction::stopMoving();
        DriverAbstraction::setNoRobotBehindCallback(std::bind(&Calibration::NoMoreRobotDetectedBehind,this));
    }
}

void Calibration::NoMoreRobotDetectedFront()
{DriverAbstraction::continueMoving();}

void Calibration::NoMoreRobotDetectedBehind()
{DriverAbstraction::continueMoving();}
