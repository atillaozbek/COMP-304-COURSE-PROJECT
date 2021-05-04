#pragma once 

#include <string>
#include <memory>
#include "Command.h"
#include "SystemHandler.h"

namespace SS
{
    const size_t MAX_HIST_COUNT = 10;

    enum specialCommandIndex : int
    {
        CD = 0,
        HISTORY = 1,
        SHORT_DIR = 2,
        HIGHTLIGTH = 3,
        KDIFF = 4,
        GOODMORNING = 5
    };

    class SeaShellCore
    {
    public:
        SeaShellCore();

        void parseAndExecute(const std::string& commandStr);
        const std::string& getCurrentPath();

        const bool isHistoryRequested();
    private:
        void execCommand(const std::unique_ptr<Command>& command);

        void handleCD(const std::string& target);
        void handleShortDir();
        void handleHighlight();
        void handleGoodMorning();

        std::string m_currentCommand;
	    std::string m_currentPath;
        std::string m_homePath;
        std::string m_currenBinDirectory;
        std::string m_executablePath;

        std::vector<std::string> m_specialCommands;
        std::string m_shortdirFile;

        bool m_historyFlag;
    };

}