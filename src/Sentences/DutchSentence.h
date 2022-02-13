#include "Sentence.h"

class DutchSentence : public Sentence
{
public:
    const std::string getTime(int hour, int minutes) override
    {
        return "HET IS " + timeToLiteral(hour, minutes);
    }
    const std::string getDate(int day, int month) override
    {
        return "HET IS " + numberToText(day) + " " + monthNumberToText(month);
    }

private:
    const std::string timeToLiteral(int hour, int minutes) override
    {
        hour = hour % 12;
        int nextHour = hour + 1;
        if (hour == 0)
        {
            hour = 12;
            nextHour = 1;
        }

        if (minutes == 0)
        {
            return numberToText(hour) + " UUR";
        }
        if (0 < minutes && minutes < 15)
        {
            return numberToText(minutes) + " OVER " + numberToText(hour);
        }
        if (minutes == 15)
        {
            return "KWART OVER " + numberToText(hour);
        }
        if (15 < minutes && minutes < 30)
        {
            return numberToText(30 - minutes) + " VOOR HALF " + numberToText(nextHour);
        }
        if (minutes == 30)
        {
            return "HALF " + numberToText(nextHour);
        }
        if (30 < minutes && minutes < 45)
        {
            return numberToText(minutes - 30) + " OVER HALF " + numberToText(nextHour);
        }
        if (minutes == 45)
        {
            return "KWART VOOR " + numberToText(nextHour);
        }
        if (45 < minutes)
        {
            return numberToText(60 - minutes) + " VOOR " + numberToText(nextHour);
        }
#ifdef DEBUG
        Serial.println("Invalid minutes: " + minutes);
#endif
        return "";
    }
    const std::string numberToText(int number) override
    {
        switch (number)
        {
        case 0:
            return "TWAALF";
        case 1:
            return "EEN";
        case 2:
            return "TWEE";
        case 3:
            return "DRIE";
        case 4:
            return "VIER";
        case 5:
            return "VIJF";
        case 6:
            return "ZES";
        case 7:
            return "ZEVEN";
        case 8:
            return "ACHT";
        case 9:
            return "NEGEN";
        case 10:
            return "TIEN";
        case 11:
            return "ELF";
        case 12:
            return "TWAALF";
        case 13:
            return "DERTIEN";
        case 14:
            return "VEERTIEN";
        case 15:
            return "VIJF TIEN";
        case 16:
            return "ZES TIEN";
        case 17:
            return "ZEVEN TIEN";
        case 18:
            return "ACHT TIEN";
        case 19:
            return "NEGEN TIEN";
        case 20:
            return "TWINTIG";
        case 21:
            return "EEN EN TWINTIG";
        case 22:
            return "TWEE EN TWINTIG";
        case 23:
            return "DRIE EN TWINTIG";
        case 24:
            return "VIER EN TWINTIG";
        case 25:
            return "VIJF EN TWINTIG";
        case 26:
            return "ZES EN TWINTIG";
        case 27:
            return "ZEVEN EN TWINTIG";
        case 28:
            return "ACHT EN TWINTIG";
        case 29:
            return "NEGEN EN TWINTIG";
        case 30:
            return "DERTIG";
        case 31:
            return "EEN EN DERTIG";
        }
#ifdef DEBUG
        Serial.println("Unknown number: " + number);
#endif
        return "";
    }
    const std::string monthNumberToText(int number) override
    {
        switch (number)
        {
        case 1:
            return "JANUARI";
        case 2:
            return "FEBRUARI";
        case 3:
            return "MAART";
        case 4:
            return "APRIL";
        case 5:
            return "MEI";
        case 6:
            return "JUNI";
        case 7:
            return "JULI";
        case 8:
            return "AUGUSTUS";
        case 9:
            return "SEPTEMBER";
        case 10:
            return "OKTOBER";
        case 11:
            return "NOVEMBER";
        case 12:
            return "DECEMBER";
        }
#ifdef DEBUG
        Serial.println("Unknown number: " + number);
#endif
        return "";
    }
};