#include "Comparison.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string.h>

void Comparison::CompareFilesLineByLine(const std::string& filePath1,const std::string& filePath2 )
{
    std::ifstream file1;
    std::ifstream file2;
    file1.open(filePath1);
    file2.open(filePath2);
    int lineCounter = 0;

    if(!file1.good() || !file2.good())
    {
        std::cout << "ERROR: Could not open the files." << std::endl;
        return;
    }

    std::string file1Line;
    std::string file2Line;
    unsigned int differenceCounter = 0; 

    while(true)
    {
        std::getline(file1,file1Line);
        std::getline(file2,file2Line);
        lineCounter++;
        
        if(file1Line != file2Line)
        {
            if(file1.peek() != EOF)
                std::cout << "First file: Line " << lineCounter <<" :" << file1Line<< std::endl;
            
            if(file2.peek() != EOF)
                std::cout << "Second file: Line " << lineCounter <<" :" << file2Line << std::endl;

            differenceCounter++;
        }
        
        if (file1.peek() == EOF && file2.peek() == EOF)
        {
            break;
        }

    }

    if(differenceCounter != 0)
    {
        std::cout << differenceCounter << " Different lines found." << std::endl;
    }
    else
    {
        std::cout << "Two files are identical." << std::endl;
    }
}

void Comparison::CompareFilesByBytes(const std::string& filePath1,const std::string& filePath2 )
{
    std::ifstream file1;
    std::ifstream file2;

    file1.open(filePath1, std::ios::binary | std::ios::in);
    file2.open(filePath2, std::ios::binary | std::ios::in);

    
    unsigned int counter = 0;
    
    if(!file1.good() || !file2.good())
    {
        std::cout << "Could not open"  << std::endl;
        return;
    }

    int i = 0;
    char file1Str;
    char file2Str; 

    while(file1 && file2)
    {   
        file1.read(&file1Str, sizeof(char));
        file2.read(&file2Str, sizeof(char));
        
        //std::cout << i++ << ") " << *file1Str << " <---> " << *file2Str << std::endl;

        
        if(file2Str != file1Str)
        {   
            //std::cout << i++ << ") " << file1Str << " <---> " << file2Str << std::endl;
            counter++;
        }

    }

    while(file1){
        file1.read(&file1Str, sizeof(char));
        counter++;
    }
    while(file2){
        file2.read(&file2Str, sizeof(char));
        counter++;
    }

    if (counter != 0)
    {
        std::cout << "The files differ in " << counter << " byte." << std::endl;
    }
    else
    {
        std::cout << "THe files are completely safe." << std::endl;
    }
}
