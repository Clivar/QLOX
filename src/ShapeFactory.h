#include <vector>
#include "Matrix.h"

class ShapeFactory
{
private:
    std::vector<ShapeColor> createSolidFromBitMatrix(std::string matrix, ShapeColor color)
    {
        std::vector<ShapeColor> result;
        for (size_t i = 0; i < matrix.size(); i++)
        {
            ShapeColor c;
            if (matrix[0] == 0)
            {
                c.r = 0;
                c.g = 0;
                c.b = 0;
            }
            else
            {
                c = color;
            }
            result.push_back(c);
        }
    }

public:
    std::vector<ShapeColor> one(ShapeColor color)
    {
        std::string bit = "00100"
                          "01100"
                          "00100"
                          "00100"
                          "01110";
        return createSolidFromBitMatrix(bit, color);
    }
};