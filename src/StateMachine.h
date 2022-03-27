#include <string>
#include <array>
#include <Arduino.h>

enum State
{
    Custom,
    Time,
    WrittenTime,
    WrittenDate,
    Rainbow
};

class StateMachine
{
private:
    const std::array<State, 2> states{{WrittenTime, WrittenDate}};
    int i = 0;

public:
    State next()
    {
        if (i++ > states.size())
        {
            i = 0;
        }
        return states[i++];
    }
};