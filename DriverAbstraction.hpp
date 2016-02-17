#include "robotdriver/speedcontroller.h"
#include "robotdriver/motioncontroller.h"
#include "robotdriver/headingcontroller.h"
#include "pathFollower.hpp"



class DriverAbstraction
{
    public:
        static void setWallFrontCallback(std::function<void(void)> wallDetectedFrontCallback);
        static void setWallBehindCallback(std::function<void(void)> wallDetectedBehindCallback);
        static void setRobotFrontCallback(std::function<void(void)> robotDetectedFrontCallback);
        static void setRobotBehindCallback(std::function<void(void)> robotDetectedBehindCallback);

        static void reactOnWall();
        static void reactOnRobot();

        static void moveBlocking(const Vector<2,float>& pos, const Vector<2,float>& target, const Vector<2,float>& direction);
        static void moveUntilWallBlocking();
        static void back(const Vector<2,float>& targetPos);

    private:
        static std::function<void(void)> wallDetectedFrontCallback, wallDetectedBehindCallback, robotDetectedFrontCallback, robotDetectedBehindCallback;
};


void DriverAbstraction::setWallFrontCallback(std::function<void(void)> wallDetectedFrontCallback)
{
    DriverAbstraction::wallDetectedFrontCallback = wallDetectedFrontCallback;
    setBlockingCallback(&DriverAbstraction::reactOnWall);
}

void DriverAbstraction::setWallBehindCallback(std::function<void(void)> wallDetectedBehindCallback)
{
    DriverAbstraction::wallDetectedBehindCallback = wallDetectedBehindCallback;
    setBlockingCallback(&DriverAbstraction::reactOnWall);
}

void DriverAbstraction::setRobotFrontCallback(std::function<void(void)> robotDetectedFrontCallback)
{
    DriverAbstraction::robotDetectedFrontCallback = robotDetectedFrontCallback;
    /*????? wait for spec*/(&DriverAbstraction::reactOnRobot);
}

void DriverAbstraction::setRobotBehindCallback(std::function<void(void)> robotDetectedBehindCallback)
{
    DriverAbstraction::robotDetectedBehindCallback = robotDetectedBehindCallback;
    /*????? wait for spec*/(&DriverAbstraction::reactOnRobot);
}

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
