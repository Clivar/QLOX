#include <vector>
#include "Matrix.h"

class ShapeFactory
{
private:
    std::vector<CRGB> createSolidFromBitMatrix(std::string matrix, CRGB color)
    {
        std::vector<CRGB> result;
        for (size_t i = 0; i < matrix.size(); i++)
        {
            CRGB c;
            if (matrix[i] == '0')
            {
                c = CRGB::Black;
            }
            else
            {
                c = color;
            }
            result.push_back(c);
        }

        return result;
    }

public:
    std::vector<CRGB> one(CRGB color)
    {
        std::string bit = "00100"
                          "01100"
                          "00100"
                          "00100"
                          "01110";
        return createSolidFromBitMatrix(bit, color);
    }
};