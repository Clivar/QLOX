#include <vector>
#include <string>
#include <iostream>

class Sentence
{
public:
    virtual ~Sentence() {}
    virtual const std::string getTime(int hour, int minutes);
    virtual const std::string getDate(int day, int month);

private:
    virtual const std::string timeToLiteral(int hour, int minutes);
    virtual const std::string numberToText(int number);
    virtual const std::string monthNumberToText(int number);
};