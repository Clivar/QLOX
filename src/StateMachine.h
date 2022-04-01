#include <string>
#include <array>
#include <Arduino.h>

enum State
{
    DigitalTime,
    WrittenTime,
    WrittenDate,
    Rain
};

class StateMachine
{
private:
    const std::array<State, 4> states{{WrittenTime, WrittenDate, DigitalTime, Rain}};
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