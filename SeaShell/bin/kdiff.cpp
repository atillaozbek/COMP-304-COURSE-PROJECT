#include "Comparison.h"
#include <iostream>

int main(int argc, char**argv)
{
    if(argc != 4 && argc != 3)
    {
        std::cout << "ERROR: Invalid Number of Parameters. " << std::endl;
        return 0;
    }

    if(argc == 4)
    {
        std::string comparisonType(argv[1]);
        std::string filePath1(argv[2]);
        std::string filePath2(argv[3]);

        if(comparisonType != "-a" && comparisonType != "-b")
        {
            std::cout << "ERROR: Operation mode must be -a, -b or nothing" << std::endl;
            return 0;
        }
        
        if(comparisonType == "-a")
        {
            Comparison::CompareFilesLineByLine(filePath1, filePath2);
            return 0;
        }
        else if(comparisonType == "-b")
        {
            Comparison::CompareFilesByBytes(filePath1, filePath2);
            return 0;
        }
    }
    else
    {
        std::string filePath1(argv[1]);
        std::string filePath2(argv[2]);

        Comparison::CompareFilesLineByLine(filePath1, filePath2);

        //std::cout << "argc is " <<  argc <<std::endl;
        return 0;
    }
}