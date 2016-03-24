#ifndef SITUATION_HPP
#define SITUATION_HPP


#include "pathFollower.hpp"


class Situation
{
    public:
        Situation();

        bool hasStarted() const;
        void avoidPreemption();
        void allowPreemption();

        void resetPosition(const Vector<2,double>& v);
        Vector<2,double> getCurrentPos();
        Vector<2,double> getCurrentDirection();

        static void updateAngleStartingMove();
        static void updatePositionEndingMove();

        static void resetMapDescription(const std::string& path);
        static void resetRobotsDescription(const std::string& path);
        static Vector<2,double> getNearestSideOnTable(const Vector<2,double>& v);
        static Vector<2,double> getNearestCornerOnTable(const Vector<2,double>& v);

    private:
        static Vector<2,double> currentPosition;
        static Vector<2,double> currentDirection;
        static double currentAngle;
};

void Situation::resetPosition(const Vector<2,double>& v)
{
    updateAngleStartingMove();
    currentPosition = v;
}

Vector<2,double> Situation::getCurrentPos()
{return currentPosition;}

Vector<2,double> Situation::getCurrentDirection()
{return currentDirection;}

void Situation::updateAngleStartingMove()
{
    currentAngle = getRobotHeading();
    currentDirection[0] = cos(currentAngle/180.0*M_PI);
    currentDirection[1] = sin(currentAngle/180.0*M_PI);
}

void Situation::updatePositionEndingMove()
{
    double d = getDistanceSinceMoveStart();
    currentPosition[0] = currentPosition[0]+currentDirection[0]*d;
    currentPosition[1] = currentPosition[1]+currentDirection[1]*d;
}

void Situation::resetMapDescription(const std::string& path)
{}

void Situation::resetRobotsDescription(const std::string& path)
{}

Vector<2,double> Situation::getNearestSideOnTable(const Vector<2,double>& v)
{}

Vector<2,double> Situation::getNearestCornerOnTable(const Vector<2,double>& v)
{}


#endif
