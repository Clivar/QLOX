#include <string>
#include <array>
#include <Arduino.h>

enum State
{
    DigitalTime,
    WrittenTime,
    WrittenDate,
    Rain,
    Rainbow
};

class StateMachine
{
private:
    const std::array<State, 5> states{{WrittenTime, WrittenDate, DigitalTime, Rain, Rainbow}};
    int i = 0;

public:
    State next()
    {
        i = (i + 1) % states.size();
        return states[i];
    }
};