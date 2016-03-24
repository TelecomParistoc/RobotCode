#ifndef DRIVER_ABSTRACTION_HPP
#define DRIVER_ABSTRACTION_HPP


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

        static void setCalibrationCallback(std::function<void(void)> calibrationCallback);
        static void setLoopCallback(std::function<void(void)> loopCallback);
        static void reactOnJack();

        static bool toggleLogging(bool logging = true);
        static bool toggleCalibration(bool calibrate = true);

        static void init();

    private:
        static std::function<void(void)> wallDetectedFrontCallback, wallDetectedBehindCallback, robotDetectedFrontCallback, robotDetectedBehindCallback;
        static std::function<void(void)> calibrationCallback;
        static std::function<void(void)> loopCallback;

        static bool calibrationFinished;
        static bool logging;
        static bool calibrate;
};


#endif
