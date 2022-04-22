#include <iostream>
using namespace std;
enum State_Robot 
{
    Off,
    Waiting,
    Rotate,
    Move,
    Clean,
    Error
};
class Robot
{
    State_Robot state;
    double angle;
    double distance;
    GetCommand* robot_command;
public:
    Robot(GetCommand* _mgmt): robot_command(_mgmt) {};
    ~Robot()
    { 
        delete robot_command;
    };
    State_Robot getState();
    void setState(State_Robot s_state);
    void processEvent(State_Robot state);
    void rotate(double angle);
    void moveStraight(double distance);
    void clean();
    void getInput();
};
void Robot::getInput() 
{
    robot_command->manual_control();
}
void Robot::moveStraight(double distance) 
{
    cout << "Robot moved straight for " << distance << " units\n";
};
void Robot::rotate(double angle) 
{
    cout << "Robot rotated to the following direction: " << angle << " degrees\n";
};
void Robot::clean() 
{
    cout << "Robot has finished cleaning the area\n";
};
void Robot::processEvent(State_Robot state) 
{
    while (state != Off) 
    {
        Robot::getInput();
        switch (state) 
        {
            case Waiting:  
                Robot::getInput();
                break;

            case Rotate:
                double dir;
                cin >> dir;
                Robot::rotate(dir);
                state = Waiting;
                break;

            case Move:
                double dst;
                cin >> dst;
                Robot::moveStraight(dst);
                state = Waiting;
                break;

            case Clean:
                Robot::clean();
                state = Waiting;
                break;

            case Error:
                state = Off;
                break;

            default:
                state = Waiting;
                break;
        }
    }
};
State_Robot Robot::getState() 
{
    return state;
};
void Robot::setState(State_Robot s_state) 
{
    state = s_state;
};
class GetCommand 
{
public:
virtual ~GetCommand() {};
    virtual void manual_control() = 0;
};
class KeyboardControl : public GetCommand 
{
    void manual_control() override
    {
        int str;
        cin >> str;
        if (str >= Off && str <= Error) 
        {
            State_Robot state = static_cast<State_Robot>(str);
        }
        else
        {
            cout << "Incorrect input";
        }

    }
};
int main() 
{
    Robot paintRobot = new Robot(new KeyboardControl);
    paintRobot.setState(Waiting);
    
    while (paintRobot.getState() != Off) 
    {
        paintRobot.processEvent(paintRobot.getState());
    }

    return 0;
}
