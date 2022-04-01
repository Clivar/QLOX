#include <string>
#include <array>
#include <Arduino.h>

enum State
{
    DigitalTime,
    WrittenTime,
    WrittenDate,
};

class StateMachine
{
private:
    const std::array<State, 3> states{{WrittenTime, WrittenDate, DigitalTime}};
    int i = 0;

public:
    State next()
    {
        if (++i >= states.size())
        {
            i = 0;
        }

        return states[i];
    }
};