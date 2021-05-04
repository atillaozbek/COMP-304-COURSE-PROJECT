#include "SystemHandler.h"
#include <sys/wait.h>
#include <pwd.h>
#include <fstream>
#include <libgen.h>         
#include <unistd.h>         
#include <linux/limits.h>   

namespace SS
{
    void SystemHandler::forkWithWait(const char* name, const char** args)
    {
        {

            pid_t pid = fork();

            if(pid == -1) //Child process creation failed.
            {
                throw std::runtime_error("Could not create a new process");
            }
            else if(pid == 0) //Child process execution
            {
                execv(name, const_cast<char*const*>(args));

                throw std::runtime_error("Could not execute the given file.");
            }
            else if( pid > 0)
            {
                wait(NULL);
            }

        }
    }

    void SystemHandler::getHomeDirectory(std::string& input)
    {
        const char* homeDirectory = getenv("HOME");

        if(homeDirectory == nullptr)
        {
            homeDirectory = getpwuid(getuid())->pw_dir;
        }

        input.clear();
        input.assign(homeDirectory);

    }

    void SystemHandler::setCurrentDirectory(const std::string& newDirectory)
    {
        int returnVal = chdir(newDirectory.c_str());

        if(returnVal)
        {
            throw std::runtime_error("Error: Specified Directory does not exists.");
        }
    }

    void SystemHandler::setCurrentDirectory(const std::string& newDirectory, std::string& oldDirectory)
    {
        int returnVal = chdir(newDirectory.c_str());

        if(returnVal)
        {
            throw std::runtime_error("Error: Specified Directory does not exists.");
        }
        else
        {
            char* currentPathBuf = static_cast<char*>(alloca(sizeof(char) * PWBUFSIZE));
            
            getcwd(currentPathBuf, PWBUFSIZE);

            oldDirectory.assign(currentPathBuf);
        }
    }

    void SystemHandler::moveCurrentDirectory(const std::string& relativeDirectory, std::string& oldDirectory)
    {
        static std::string temp; 

        if(oldDirectory.back() != '/') //adding '/' to the end of the string if it is not already there
        {
            oldDirectory.push_back('/');
        }

        temp.assign(oldDirectory + relativeDirectory);

        int returnVal = chdir(temp.c_str());

        if(returnVal)
        {
            throw std::runtime_error("Error: No such file or directory exists.");
        }
        else
        {
            char* currentPathBuf = static_cast<char*>(alloca(sizeof(char) * SS::PWBUFSIZE));
            
            getcwd(currentPathBuf, PWBUFSIZE);

            oldDirectory.assign(currentPathBuf);
        }

    }

    void SystemHandler::getPresentWorkingDirectory(std::string& presentDir)
    {
        char* currentPathBuf = static_cast<char*>(alloca(sizeof(char) * SS::PWBUFSIZE));

        getcwd(currentPathBuf, PWBUFSIZE);

        presentDir.assign(currentPathBuf);
    }

    void SystemHandler::getExecutablePath(std::string& execPath)
    {
        char result[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        const char *path;
        if (count != -1) {
            path = dirname(result);
        }  

        execPath.assign(path);
    }

    void SystemHandler::appendToFile(const std::string& filePath, const std::string& data)
    {
        std::ofstream file;

        file.open(filePath, std::fstream::app);

        file << data;
        file << std::endl;
        file.close();
    }

    void SystemHandler::truncateToFile(const std::string& filePath, const std::string& data)
    {
        std::ofstream file;

        file.open(filePath, std::fstream::trunc);

        file << data;
        file << std::endl;
        file.close();
    }

    bool SystemHandler::doesFileExists(const std::string& parentFolder, const std::string& relativeFilePath)
    {
        std::ifstream file;

        if(relativeFilePath[0] == '/')
        {
            file.open(relativeFilePath);
        }
        else
        {
            file.open(parentFolder + "/" + relativeFilePath);
        }

        return file.good();
    }

}