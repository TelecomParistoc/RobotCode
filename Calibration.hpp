

class Calibration
{
    public:
        Calibration(bool launchInAnyCondition = false);

        void launch();
        void reset(std::shared_ptr<Situation> s);
        void setGoal(const Vector<2,float>& goal);

        void wallDetectedFront();
        void wallDetectedBehind();
        void robotDetectedFront();
        void robotDetectedBehind();
        void NoMoreRobotDetectedFront();
        void NoMoreRobotDetectedBehind();

    private:
        std::shared_ptr<Situation> situation;
        bool launchInAnyCondition;

        bool hasGoal;

        std::mutex mutex;
};

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
{
    DriverAbstraction::continueMoving();
}

void Calibration::NoMoreRobotDetectedBehind()
{
    DriverAbstraction::continueMoving();
}
