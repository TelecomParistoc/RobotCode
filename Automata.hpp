

enum State{ComputeSituation, MoveToAction, Action, BlockedByRobot, BlockedByRobotNotInterruptible, countWallsUnpredictedMoving, countWallsUnpredictedActing, Recalibrate};

class Automata
{
    public:
        void launch();

        void computeSituationState();
        void moveToActionState();
        void actionState();
        void blockedByRobotState();
        void checkWallPredictedWhenMoving();
        void checkWallPredictedWhenActing();
        void blockedByRobotDuringNotInterruptibleAction();
        void recalibrate();

    private:
        bool detectWall;
        bool detectRobot;
        float counterBlock;
        int unpredictedWallsCounter;
        int thresholdUnpredictedWalls;

        std::map<State,std::function<void(void)> > chooseWay;

        State currentState;

        std::mutex mutex;

        std::shared_ptr<Situation> situation;
        std::shared_ptr<ActionBoard> actions;
        std::shared_ptr<Calibration> recalibration;
};

void Automata::computeSituationState()
{
    Logger::getStaticLogger()<<"State : computing situation"<<std::endl;
    mutex.lock();
    situation->update();
    bool wait = false;
    if(detectRobot)
    {
        Logger::getStaticLogger()<<"Robot is blocked by another robot"<<std::endl;
        wait = true;
        currentState = BlockedByRobot;
        if(beginBlocked<0)
            beginBlocked = //curTime
        if(timeSinceBlocked>thresholdTimeBlockedByRobot)
        {
            Logger::getStaticLogger()<<"Too long time since robot is blocked : obstacle is added, plan is recomputed"<<std::endl;
            wait = false;
            timeSincePreviousBlocking = //curTime
            situation->addFrontObstacleForCurrentRobot();
            actions->computeBestGoal(situation);
        }
    }
    else
    {
        beginBlocked = -1;
        situation->deleteAllRobotObstacles();
        actions->computeBestGoal(situation);
    }
    mutex.unlock();

    if(!wait&&actions->isOkToGo())
    {
        Logger::getStaticLogger()<<"Let's go to the goal following the path "<<actions->logPath()<<std::endl;
        timeSinceBlocked = -1;
        DriverAbstraction::setPath(actions->pathToGo());
        currentState = MoveToAction;
    }
    else if(!wait&&timeSincePreviousBlocking>thresholdDeleteObstacles)
    {
        Logger::getStaticLogger()<<"We can't move and we can't find another path, that's boring so we delete all obstacles"<<std::endl;
        situation->deleteAllRobotObstacles(); //provoquera un nouveau calcul dans le computeSituation suivant
    }
}

void Automata::moveToActionState()
{
    Logger::getStaticLogger()<<"State : moving to action"<<std::endl;
    mutex.lock();
    if(DriverAbstraction::robotForward())
    {
        Logger::getStaticLogger()<<"A robot in front of the robot has been detected, we return to previous point"<<std::endl;
        currentState = BlockedByRobot;
        return;
    }
    else if(DriverAbstraction::wallForward())
    {
        if(situation->isWallPredictedForward(DriverAbstraction::getDirX(),DriverAbstraction::getDirY()))
            Logger::getStaticLogger()<<"A wall in front of the robot has been detected and it was predicted"<<std::endl;
        else
        {
            Logger::getStaticLogger()<<"A wall in front of the robot has been detected but it hasn't been predicted"<<std::endl;
            currentState = countWallsUnpredictedMoving;
        }
        return;
    }
    mutex.unlock();

    if(DriverAbstraction::hasReachedTheGoal())
    {
        Logger::getStaticLogger()<<"We reached the goal, now try to execute the action"<<std::endl;
        actions->executeComputedAction();
        currentState = Action;
    }
}

void Automata::actionState()
{
    Logger::getStaticLogger()<<"State : acting"<<std::endl;
    mutex.lock();
    if(DriverAbstraction::robotForward())
    {
        if(actions->currentActionInterruptible())
        {
            Logger::getStaticLogger()<<"Another robot has been detected and action is interruptible, there will be a recomputation"<<std::endl;
            actions->interrupt();
            currentState = BlockedByRobot;
        }
        else
        {
            Logger::getStaticLogger()<<"Another robot has been detected but action is not interruptible :( must wait"<<std::endl;
            currentState = BlockedByRobotNotInterruptible;
        }
        return;
    }
    mutex.unlock();

    if(actions->isFinished())
    {
        Logger::getStaticLogger()<<"Action is finished ! Great ! Hope the points are in the pocket"<<std::endl;
        currentState = ComputeSituation;
    }
}

void Automata::blockedByRobotState()
{
    Logger::getStaticLogger()<<"State : blocked by ennemy or obstacle ?"<<std::endl;
    currentState = ComputeSituation;
    timeSinceBlocked = //curTime-beginBlocked
    usleep(100000);
}

void Automata::blockedByRobotDuringNotInterruptibleAction()
{
    Logger::getStaticLogger()<<"State : blocked by ennemy or obstacle ? during action"<<std::endl;
    currentState = actionState;
    usleep(200000);
}

void Automata::countWallUnpredictedWhenMoving()
{

}

void Automata::countWallUnpredictedWhenActing()
{

}

void Automata::recalibrate()
{
    Logger::getStaticLogger()<<"State : recalibrating (not yet implemented)"<<std::endl;
    currentState = ComputeSituation;
}

void Automata::launch()
{
    while(!timeOver)
    {
        chooseWay(currentState);
        usleep(2000);
    }
}
