


class Automata
{
    public:

    private:
        bool detectWall;
        bool detectRobot;
        float counterBlock;
        int unpredictedWallsCounter;
        int thresholdUnpredictedWalls;

        std::mutex mutex;

        std::shared_ptr<Situation> situation;
        std::shared_ptr<ActionBoard> actions;
        std::shared_ptr<Recalibration> recalibration;
}
