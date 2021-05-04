#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class ColoredPrinter
{
public:
    enum class ColorType
    {
        RED,
        GREEN,
        BLUE
    };

    static void printWordInFileWithColor(const std::string& filePath, const std::string& word, ColorType color)
    {
        std::ifstream file;

        file.open(filePath);

        if(!file.good())
        {
            std::cout << "Could not open given file." << std::endl;
            return;
        }

        std::string colorHead;
        std::string colorTail;
        std::string currentLine;

        switch(color)
        {
            case ColorType::RED:
            colorHead.assign("\e[1;31m");
            break;
            case ColorType::GREEN:
            colorHead.assign("\e[1;32m");
            break;
            case ColorType::BLUE:
            colorHead.assign("\e[1;34m");
            break;
        }

        colorTail.assign("\033[0m");

        while(std::getline(file, currentLine))
        {
            std::stringstream lineStream(currentLine);
            std::string currentWord;

            while(lineStream >> currentWord)
            {
                if(word == currentWord)
                {
                    std::cout << colorHead << currentWord << " " << colorTail;
                }
                else
                {
                    std::cout << currentWord << " ";
                }
            }

            std::cout << std::endl;
            
        }
        
    }
private:
};

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        std::cout << "ERROR: Parameters Entered Incorrectly" << std::endl;
        return 0;
    }

    const std::string searchWord(argv[1]);
    const std::string colorMode(argv[2]);
    const std::string filePath(argv[3]);

    ColoredPrinter::ColorType colorType;

    if(colorMode == "r")
    {
        colorType = ColoredPrinter::ColorType::RED;
    }
    else if(colorMode == "g")
    {
        colorType = ColoredPrinter::ColorType::GREEN;
    }
    else if(colorMode == "b")
    {
        colorType = ColoredPrinter::ColorType::BLUE;
    }
    else
    {
        std::cout << "ERROR: Invalid Color Type." << std::endl;
        std::cout << "Color Mode is " << colorMode << std::endl;
        return 0;
    }
    
    ColoredPrinter::printWordInFileWithColor(filePath, searchWord, colorType);

    return 0;
}