#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <string>

#include "Action.hpp"

static std::vector<std::string> ways;
static std::vector<Action> actions;
static std::pair<double,double> curPos = std::pair<double,double>(41,1003);


static bool isMovingToAction = false;
static bool started = false;
static bool pausePath = false;
static bool blocked = false;
static bool seeBlocked = false;

static int way = -1;

void checkCollisionAndReact(int)
{seeBlocked = true;}

void endWay()
{
	std::cout<<"End of way "<<way<<std::endl;
	way++;
	if(way<(int)ways.size())
		actions[way].start();
	isMovingToAction = false;
}

void endAction()
{
	std::cout<<"End of action "<<way<<std::endl;
	curPos = PathFollower::getCurrentPos();
	PathFollower::setCurrentPosition(curPos.first,curPos.second);
	if(way<(int)ways.size())
		ffollow(ways[way].c_str(), &endWay);
	isMovingToAction = true;
}

static bool start = false;

void go()
{
	std::cout<<"GO !"<<std::endl;
	start = true;
	endWay();
}

void exitAndClean()
{
    std::cout<<"Exiting because of jack"<<std::endl;
    exit(0);
}

void check_mode()
{
    std::cout<<"We are in mode "<<getMode()<<std::endl;
	if(getMode()==TEST_MODE)
	{
		int ret = system("ps -Al | grep loop");

	    if(!ret)
	    {
			std::cout<<"Stopping service. Warning it will exit program."<<std::endl;
	        setuid(0);
	    	system("service launchLoop.sh stop");
			exit(0);
	    }
	    else
	        printf("No loop service running, robot in test mode, no problem.\n");
	}
}
