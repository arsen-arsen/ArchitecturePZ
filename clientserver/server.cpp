#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

enum RobotState { WAITING, MOVING, ROTATE, CLEANING, OFF };

class  CommandStrategy
{
public:
	virtual string getCommand() = 0;
	virtual ~CommandStrategy() {}
};

class  ConsoleCommandStrategy : public CommandStrategy
{
public:
	string getCommand() override
	{
		string command;
		cout << "Enter command: \t Command list:\n MOVING,\t ROTATE,\t CLEANING\n";
		cin >> command;
		return command;
	}
};

class NetCommandStrategy : public CommandStrategy
{
public:
	int sock;
	struct sockaddr_in addr;
	char buf[10];
	int bytes_read;
	NetCommandStrategy()
	{
		sock = socket(AF_INET, SOCK_DGRAM, 0);

		addr.sin_family = AF_INET;
		addr.sin_port = htons(3425);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		{
			perror("bind");
		}
	}

	string getCommand() override
	{
		cout << "Waiting for a command...\n";
		string command;
		bytes_read = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL);
		buf[bytes_read + 1] = '\0';
		command = buf;
		return command;
	}
};

class  OnTestCommandStrategy : public CommandStrategy
{
public:
	virtual string getCommand() override
	{
		return string("On");
	}
};

class Robot
{
	int angle;
	int distance;
	RobotState state;
	CommandStrategy* commandprocessor;
public:
	Robot(CommandStrategy* pcommandprocessor) : state(WAITING), commandprocessor(pcommandprocessor) {};

	~Robot()
	{
		delete commandprocessor;
	}

	void processEvent(string command);
	void WAITINGEventProcessing(string command);

	//Setters
	void setAngle(int n_angle);
	void setDistance(int n_distance);
	RobotState getState();
	void setState(RobotState n_state);

	void ROTATE(int n_angle);
	void MOVING(int n_distance);
	void CLEANING();
	int R(const string& com);

	string getCommand()
	{
		return commandprocessor->getCommand();
	}
};

RobotState Robot::getState()
{
	return state;
};

void Robot::setState(RobotState n_state)
{
	state = n_state;
};

void Robot::ROTATE(int n_angle)
{
	angle = n_angle;
	cout << "Robot turned to " << angle << " degree" << endl;
};

void Robot::MOVING(int n_distance)
{
	distance = n_distance;
	cout << "Robot moved straight to " << distance << endl;
};

void Robot::CLEANING()
{
	cout << "Robot cleaned the area" << endl;
};

void Robot::WAITINGEventProcessing(string command)
{
	if (command == "WAITING")
	{
		cout << "Robot is on. Enter command:\n";
	}
};

int Robot::R(const string& com)
{
	if (com == "WAITING")
	{
		return WAITING;
	}
	else if (com == "ROTATE")
	{
		return ROTATE;
	}
	else if (com == "CLEANING")
	{
		return CLEANING;
	}
	else if (com == "MOVING")
	{
		return MOVING;
	}
	else
	{
		return OFF;
	}
};

void Robot::processEvent(string command) {
	istringstream command(command);
	string response;
	command >> response;

	switch (state)
	{
	case WAITING:
		switch (R(response))
		{
		case MOVING:
			state = MOVING;
			break;
		case ROTATE:
			state = ROTATE;
			break;
		case CLEANING:
			state = CLEANING;
			break;
		case OFF:
			state = OFF;
			break;
		}
		break;

	case MOVING:
		int distance;
		command >> distance;
		MOVING(distance);
		state = WAITING;
		break;

	case ROTATE:
		double angl;
		cout << "Enter angle:\n";
		cin >> angl;
		ROTATE(angl);
		state = WAITING;
		break;

	case CLEANING:
		CLEANING();
		state = WAITING;
		break;

	case OFF:
		state = OFF;
		break;

	default:
		cout << "Incorrect command. Try again";
		state = WAITING;
		break;
	}
};

int main(void)
{
	Robot* paintRobot;
	char bind;
	cout << "Choose a way to receive commands: c - console,\tn-network\n";
	cin >> bind;
	if (bind == 'c')
		paintRobot = new Robot(new ConsoleCommandStrategy());
	else if (bind == 'n')
		paintRobot = new Robot(new NetCommandStrategy());

	string command;
	while (paintRobot->getState() != OFF)
	{
		if (paintRobot->getState() == WAITING)
			command = paintRobot->getCommand();
		paintRobot->processEvent(command);
	}
	return 0;
}
