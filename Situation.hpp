#include "pathFollower.hpp"


class Situation
{
    public:
        Situation();

        void updatePosition(); //params a remplir en fonction de ce qu'Arnaud renvoie dans son callback

        bool hasStarted() const;
        void avoidPreemption();
        void allowPreemption();

        Vector<2,float> getCurrentPos();
        Vector<2,float> getCurrentDirection();

        static void resetMapDescription(const std::string& path);
        static void resetRobotsDescription(const std::string& path);
        static Vector<2,float> getNearestSideOnTable(const Vector<2,float>& v);
        static Vector<2,float> getNearestCornerOnTable(const Vector<2,float>& v);

    private:
        Vector<2,float> currentPosition;
};
