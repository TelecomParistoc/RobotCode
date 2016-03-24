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

        static void init();
        static void resetMapDescription(const std::string& path);
        static void resetRobotsDescription(const std::string& path);
        static Vector<2,double> getNearestSideOnTable(const Vector<2,double>& v);
        static Vector<2,double> getNearestCornerOnTable(const Vector<2,double>& v);

    private:
        static Vector<2,double> currentPosition;
        static Vector<2,double> currentDirection;
        static double currentAngle;

        static float robotRadius;
        static AStar shortestPathMatrix;

        static std::shared_ptr<Info_Warning_Error_Logger> logger;
};

float Situation::robotRadius = 1;
AStar Situation::shortestPathMatrix;
std::shared_ptr<Info_Warning_Error_Logger> Situation::logger;

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

void Situation::init()
{
    logger = Easy_Log_In_File::getInfoLog();
}

void Situation::resetMapDescription(const std::string& path)
{
    std::ifstream ifs(path.c_str(),std::ios::in);
    std::ifs>>tableWidth;
    std::ifs>>tableHeight;

    (*logger)<<"Table loaded with width "<<tableWidth<<" and height "<<tableHeight;
    logger->endline();

    float tmp;
    unsigned int counter=0;
    std::vector<Obstacle> obstacles;
    std::array<float,5> obstacleConstruction;
    while(ifs>>tmp)
    {
        obstacleConstruction[counter%5] = tmp;
        counter++;
        if(!(counter%5))
            obstacles.push_back(Obstacle(obstacleConstruction[0],obstacleConstruction[1]+robotRadius,obstacleConstruction[2],obstacleConstruction[3]+robotRadius,obstacleConstruction[4]));
    }

    shortestPathMatrix.resize(tableWidth,tableHeight);
    shortestPathMatrix.update(obstacles);
}

void Situation::resetRobotsDescription(const std::string& path)
{
    std::ifstream ifs(path.c_str(),std::ios::in);
    ifs>>robotRadius;
    (*logger)<<"Robot radius loaded : "<<robotRadius;
    logger->endline();
}

Vector<2,double> Situation::getNearestSideOnTable(const Vector<2,double>& v)
{}

Vector<2,double> Situation::getNearestCornerOnTable(const Vector<2,double>& v)
{}


#endif
