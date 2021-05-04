#pragma once
#include <exception>
#include <iostream>
#include <unistd.h>

namespace SS
{
    const unsigned int PWBUFSIZE = 100;

    class SystemHandler
    {
    public:
        SystemHandler() = delete;

        static void forkWithWait(const char* name, const char** args);
        static void getHomeDirectory(std::string& input);
        static void setCurrentDirectory(const std::string& newDirectory);
        static void setCurrentDirectory(const std::string& newDirectory, std::string& oldDirectory);
        static void moveCurrentDirectory(const std::string& relativeDirectory, std::string& oldDirectory);
        static void getPresentWorkingDirectory(std::string& presentDir);
        static void getExecutablePath(std::string& execPath);

        //Writing a string to a file;
        static void appendToFile(const std::string& filePath, const std::string& data);
        static void truncateToFile(const std::string& filePath, const std::string& data);
        static void readFromFile(const std::string& filePath, std::string& data);

        //checking if a file exists
        static bool doesFileExists(const std::string& parentFolder, const std::string& relativeFilePath);
    private:
    };
}