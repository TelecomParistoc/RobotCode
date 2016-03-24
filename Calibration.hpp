#ifndef CALIBRATION_HPP
#define CALIBRATION_HPP


class Calibration
{
    public:
        Calibration(bool launchInAnyCondition = false);

        void launch();
        void reset(std::shared_ptr<Situation> s);

        void setLaunchBehaviour(bool launchInAnyCondition);
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


#endif
