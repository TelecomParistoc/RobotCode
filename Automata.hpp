

enum State{ComputeSituation, MoveToAction, Action, BlockedByRobot, BlockedByRobotNotInterruptible, CheckWallPredictedMoving, CheckWallPredictedActing, Recalibrate};

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
    mutex.lock();
    situation->update();
    bool wait = false;
    if(detectRobot)
    {
        wait = true;
        currentState = BlockedByRobot;
        if(timeSinceBlocked>thresholdTimeBlockedByRobot)
        {
            wait = false;
            timeSincePreviousBlocking = //curTime
            actions->computeBestGoal(situation);
            situation->addFrontObstacleForCurrentRobot();
        }
    }
    else
    {
        situation->deleteAllRobotObstacles();
        actions->computeBestGoal(situation);
    }
    mutex.unlock();

    if(!wait&&actions->isOkToGo())
    {
        timeSinceBlocked = -1;
        DriverAbstraction::setGoal(goal);
        currentState = MoveToAction;
    }
    else if(!wait&&timeSincePreviousBlocking>thresholdDeleteObstacles)
        situation->deleteAllRobotObstacles(); //provoquera un nouveau calcul dans le computeSituation suivant
}

void Automata::moveToActionState()
{
    mutex.lock();
    if(detectRobot)
    {
        currentState = BlockedByRobot;
        return;
    }
    mutex.unlock();

    if(DriverAbstraction::hasReachedTheGoal())
    {
        actions->executeComputedAction();
        currentState = Action;
    }
}

void Automata::actionState()
{
    mutex.lock();
    if(detectRobot)
    {
        if(actions->currentActionInterruptible())
        {
            actions->interrupt();
            currentState = BlockedByRobot;
        }
        else
            currentState = BlockedByRobotNotInterruptible;
        return;
    }
    mutex.unlock();

    if(actions->isFinished())
        currentState = ComputeSituation;
}

void Automata::blockedByRobotState()
{
    currentState = ComputeSituation;
    usleep(100000);
}

void Automata::checkWallPredictedWhenMoving();
void Automata::checkWallPredictedWhenActing();
void Automata::blockedByRobotDuringNotInterruptibleAction();
void Automata::recalibrate();

void Automata::launch()
{
    while(!timeOver)
    {
        chooseWay(currentState);
        usleep(2000);
    }
}
