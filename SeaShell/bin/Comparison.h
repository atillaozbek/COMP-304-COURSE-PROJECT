#pragma once
#include <string>

class Comparison
{
public: 

    static void CompareFilesLineByLine(const std::string& filePath1,const std::string& filePath2 );
    static void CompareFilesByBytes(const std::string& filePath1,const std::string& filePath2 );

private:

};